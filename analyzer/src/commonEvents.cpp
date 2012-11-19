#include "commonEvents.h"

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!

WX_DEFINE_OBJARRAY(DatalogChannelSelectionSet);


DEFINE_EVENT_TYPE ( OPTIONS_CHANGED_EVENT )
DEFINE_EVENT_TYPE ( CONFIG_CHANGED_EVENT )
DEFINE_EVENT_TYPE ( CONFIG_STALE_EVENT )
DEFINE_EVENT_TYPE ( WRITE_CONFIG_EVENT )
DEFINE_EVENT_TYPE ( ADD_NEW_LINE_CHART_EVENT )
DEFINE_EVENT_TYPE ( ADD_NEW_ANALOG_GAUGE_EVENT )
DEFINE_EVENT_TYPE ( ADD_NEW_DIGITAL_GAUGE_EVENT )
DEFINE_EVENT_TYPE ( ADD_NEW_GPS_VIEW_EVENT )
DEFINE_EVENT_TYPE ( UPDATE_STATUS_EVENT )
DEFINE_EVENT_TYPE ( UPDATE_ACTIVITY_EVENT )
DEFINE_EVENT_TYPE ( PLAY_FWD_DATALOG_EVENT )
DEFINE_EVENT_TYPE ( PLAY_REV_DATALOG_EVENT )
DEFINE_EVENT_TYPE ( PAUSE_DATALOG_EVENT )
DEFINE_EVENT_TYPE ( JUMP_BEGINNING_DATALOG_EVENT )
DEFINE_EVENT_TYPE ( JUMP_END_DATALOG_EVENT )
DEFINE_EVENT_TYPE ( SEEK_REV_DATALOG_EVENT )
DEFINE_EVENT_TYPE ( SEEK_FWD_DATALOG_EVENT )


