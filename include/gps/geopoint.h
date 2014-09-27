/*
 * geopoint.h
 *
 * A simpler encapsulation of GPS data.
 *
 *  Created on: Jan 20, 2014
 *      Author: stieg
 */

#ifndef GEOPOINT_H_
#define GEOPOINT_H_

#include <stdbool.h>

typedef struct _GeoPoint {
	float latitude;
	float longitude;
} GeoPoint;

// Make into Enum?
#define GP_EARTH_RADIUS_KM	6371
#define GP_EARTH_RADIUS_M	6371000

/**
 * Populates a given GeoPoint structure with the current Latitude and Longitude data.
 * @param gp Pointer to the GeoPoint structure to populate
 * @return The pointer value passed in.
 */
GeoPoint* populateGeoPoint(GeoPoint *gp);

/**
 * Finds the distance between the two geopoints using the
 * basic Pythagoras' Theorem.  This is only useful for small distances as
 * the inaccuracies will increase as the distance does.  However this is
 * also one of the fastest methods of calculating this distance.
 * @param a Point a
 * @param b Point b
 * @return The distance between the two points in Meters
 */
float distPythag(const GeoPoint *a, const GeoPoint *b);

/**
 * Finds the distance between two points using the spherical law of cosines.
 * This method, while more accurate than distPythag is still not quite as accurate
 * as distHaversine but is less computationally expensive.
 * @param a Point a
 * @param b Point b
 * @return The distance between the two points in Meters
 */
float distSLCos(GeoPoint *a, GeoPoint *b);

/**
 * Finds the distance between two points using the Hasversine method.  This is one of
 * the more accurate methods of finding direct distance over long ranges.  Unfortunately
 * it is also the most computationally expensive.
 * @param a Point a
 * @param b Point b
 * @return The distance between the two points in Meters
 */
float distHaversine(GeoPoint *a, GeoPoint *b);

/**
 * Tells us if the given point is within the bound of a given GeoCircle.  Note
 * that this is a circle only and not a sphere.  In otherwords elevation has
 * no effect on this calculation.
 * @param point The point in quesiton
 * @param center The center of the GeoCircle
 * @param radius The radius of the circle.
 */
bool isPointInGeoCircle(const GeoPoint point, const GeoPoint center,
                        const float radius);

#endif /* GEOPOINT_H_ */
