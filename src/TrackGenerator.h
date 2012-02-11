/*
 * TrackGenerator.h
 *
 *  Created on: Jan 23, 2012
 *      Author: William Boyd
 *				MIT, Course 22
 *              wboyd@mit.edu
 */

#ifndef TRACKGENERATOR_H_
#define TRACKGENERATOR_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include "Point.h"
#include "Track.h"
#include "Geometry.h"
#include "Magick++.h"
#include "silo.h"

class Plotting;

class TrackGenerator {
private:
	int _num_azim;			/* number of azimuthal angles */
	double _spacing;		/* track spacing */
	int* _num_tracks;		/* number of tracks at each angle */
	int* _num_x;			/* number of tracks starting on x-axis */
	int* _num_y;			/* number of tracks starting on y-axis */
	double* _azim_weights;	/* azimuthal weights */
	double _width;
	double _height;
	Track** _tracks;
	Geometry* _geom;
	int _bit_length_x;
	int _bit_length_y;
	double _x_pixel;
	double _y_pixel;
	int* _pix_map_tracks;
	int* _pix_map_segments;
	int* _pix_map_visit;
public:
	TrackGenerator(Geometry* geom, const int num_azim,
			const double spacing, const int bit_dim);
	virtual ~TrackGenerator();
    double *getAzimWeights() const;
    int getNumAzim() const;
    int *getNumTracks() const;
    double getSpacing() const;
    Track **getTracks() const;
    void generateTracks();
	void computeEndPoint(Point* start, Point* end,  const double phi,
			const double width, const double height);
	void makeReflective();
	void segmentize();
	void plotTracksTiff();
	void plotSegmentsBitMap(Track* track, double sin_phi, double cos_phi);
	void plotSegmentsTiff();
	void LineFct(int x0, int y0, int x1, int y1, int* pixMap, int color = 1);
	void printTrackingTimers();
	void plotFSRs();
};

#endif /* TRACKGENERATOR_H_ */
