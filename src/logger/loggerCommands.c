/*
 * Race Capture Firmware
 *
 * Copyright (C) 2016 Autosport Labs
 *
 * This file is part of the Race Capture firmware suite
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should
 * have received a copy of the GNU General Public License along with
 * this code. If not, see <http://www.gnu.org/licenses/>.
 */

#include "cpu.h"
#include "gpsTask.h"
#include "loggerCommands.h"
#include "loggerConfig.h"
#include "loggerData.h"
#include "loggerHardware.h"
#include "loggerSampleData.h"
#include "loggerTaskEx.h"
#include "luaScript.h"
#include "mem_mang.h"
#include "mod_string.h"
#include "modp_atonum.h"
#include "modp_numtoa.h"
#include "printk.h"
#include "sampleRecord.h"
#include "sdcard.h"
#include "serial.h"
#include "taskUtil.h"
#include "tracks.h"
#include "usart.h"

#include <stddef.h>
#include <stdbool.h>

/* Time (ms) to wait for input before continuing */
#define TERM_WAIT_MS 5

static const char* enable_str(const bool enable)
{
        return enable ? "Enabled" : "Disabled";
}

void TestSD(Serial *serial, unsigned int argc, char **argv)
{
    /* TODO BAP - could not remove TestSD from command list b/c statically defined array, fix somehow */
#if defined(SDCARD_SUPPORT)
    int lines = 1;
    int doFlush = 0;
    int quiet = 0;
    if (argc > 1) lines = modp_atoi(argv[1]);
    if (argc > 2) doFlush = modp_atoi(argv[2]);
    if (argc > 3) quiet = modp_atoi(argv[3]);
    TestSDWrite(serial, lines, doFlush, quiet);
#endif

}


static void print_reset_status(Serial *s, const char *msg, const int status)
{
        char* status_word = status ? "FAILED" : "PASS";
        s->put_s(msg);
        s->put_s(": ");
        s->put_s(status_word);
        s->put_s("\r\n");
}

void ResetConfig(Serial *serial, unsigned int argc, char **argv)
{
        int tmp;
        int res = 0;

        tmp = flash_default_logger_config();
        res += tmp;
        print_reset_status(serial, "Flashing Default Logger Config", tmp);

#if defined(LUA_SUPPORT)
        tmp = flash_default_script();
        res += tmp;
        print_reset_status(serial, "Flashing Default Script", tmp);
#endif

        tmp = flash_default_tracks();
        res += tmp;
        print_reset_status(serial, "Flashing Default Tracks", tmp);

        if (res) {
                put_commandError(serial, ERROR_CODE_CRITICAL_ERROR);
        } else {
                put_commandOK(serial);
        }

        delayMs(500);
        cpu_reset(0);
}

static void StartTerminalSession(Serial *fromSerial, Serial *toSerial, uint8_t localEcho)
{
        bool stay = true;
        const size_t delay_ticks = msToTicks(TERM_WAIT_MS);
        while (stay) {
                char c;
                /* Delay so we don't starve other tasks (like watchdog) */
                while (fromSerial->get_c_wait(&c, delay_ticks)) {
                        if (c == 27)
                                stay = false;

                        if (localEcho)
                                fromSerial->put_c(c);

                        if (c == '\r' && localEcho)
                                fromSerial->put_c('\n');

                        toSerial->put_c(c);

                        if (c == '\r')
                                toSerial->put_c('\n');
                }

                while (toSerial->get_c_wait(&c, 0)) {
                        fromSerial->put_c(c);
                        if (c == '\r' && localEcho)
                                fromSerial->put_c('\n');
                }
        }
}

void StartTerminal(Serial *serial, unsigned int argc, char **argv)
{
        if (argc < 3) {
                put_commandError(serial, ERROR_CODE_MISSING_PARAMS);
                return;
        }

        serial->put_s("Entering Terminal. Press ESC to exit\r\n");

        uint32_t port = modp_atoui(argv[1]);
        uint32_t baud = modp_atoui(argv[2]);
        uint8_t localEcho = (argc > 3 ? modp_atoui(argv[3]) : 1);

        Serial *targetSerial = get_serial(port);
        if (!targetSerial) {
                put_commandError(serial, ERROR_CODE_INVALID_PARAM);
                return;
        }

        configure_serial(port, 8, 0, 1, baud);
        StartTerminalSession(serial, targetSerial, localEcho);
}

void ViewLog(Serial *serial, unsigned int argc, char **argv)
{
    serial->put_s("Starting logging mode.  Hit \"q\" to exit\r\n");

    while(1) {
        // Write log to serial
        read_log_to_serial(serial, 0);

        // Look for 'q' to exit.
        char c = 0;
        serial->get_c_wait(&c, msToTicks(5));
        if (c == 'q') break;
    }

    // Give a little space when we finish up with log watching.
    serial->put_s("\r\n\r\n");
    serial->flush();
}

void SetLogLevel(Serial *serial, unsigned int argc, char **argv)
{
    // XXX make this more robust maybe.
    if (argc < 1 || argv[1][0] < '0' || argv[1][0] > '8') {
        put_commandError(serial, ERROR_CODE_INVALID_PARAM);
        return;
    }

    enum log_level level = (enum log_level) modp_atoui(argv[1]);
    set_log_level(level);
    put_commandOK(serial);
}

void LogGpsData(Serial *serial, unsigned int argc, char **argv)
{
    if (argc != 2) {
        serial->put_s("Must pass one argument only.  Enter 0 to disable, "
                      "or non-zero to enable\r\n");
        put_commandError(serial, ERROR_CODE_INVALID_PARAM);
    } else {
        const bool enable = (argv[1][0] != '0');
        setGpsDataLogging(enable);
        serial->put_s(enable_str(enable));
        serial->put_s(" the printing of raw GPS data to the log.\r\n");
        put_commandOK(serial);
    }

    serial->flush();
}

void SetSerialLog(Serial *serial, unsigned int argc, char **argv)
{
        if (argc != 3) {
                serial->put_s("Two arguments required to specify serial port "
                              "and enable/disable option\r\n");
                put_commandError(serial, ERROR_CODE_INVALID_PARAM);
                goto done;
        }

        const serial_id_t port = (serial_id_t) atoi(argv[1]);
        Serial *s = get_serial(port);
        if (port == SERIAL_USB || !s) {
                serial->put_s("Invalid serial port.\r\n");
                put_commandError(serial, ERROR_CODE_INVALID_PARAM);
                goto done;
        }

        const bool enable = argv[2][0] != '0';
        const bool prev = serial_logging(s, enable);

        serial->put_s(enable_str(prev));
        serial->put_s(" -> ");
        serial->put_s(enable_str(enable));
        serial->put_s("\r\n");
        put_commandOK(serial);

done:
        serial->flush();
}

void FlashConfig(Serial *serial, unsigned int argc, char **argv)
{
        const bool success = flashLoggerConfig() == 0;
        serial->put_s(success ? "Success" : "Failed");
        serial->put_s("\r\n");
        put_commandOK(serial);
}
