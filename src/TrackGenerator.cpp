/*
 * TrackGenerator.cpp
 *
 *  Created on: Jan 23, 2012
 *      Author: William Boyd
 *				MIT, Course 22
 *              wboyd@mit.edu
 */

#include "TrackGenerator.h"


/**
 * TrackGenerator constructor
 * @param geom a pointer to a geometry object
 * @param num_azim number of azimuthal angles
 * @param spacing track spacing
 */
TrackGenerator::TrackGenerator(Geometry* geom, const int num_azim,
								const double spacing) {

	_geom = geom;
	_num_azim = num_azim/2.0;
	_spacing = spacing;

	try {
		_num_tracks = new int[_num_azim];
		_num_x = new int[_num_azim];
		_num_y = new int[_num_azim];
		_azim_weights = new double[_num_azim];
		_tracks = new Track*[_num_azim];
	}
	catch (std::exception &e) {
		log_printf(ERROR, "Unable to allocate memory for TrackGenerator. Backtrace:"
				"\n%s", e.what());
	}
}


/**
 * TrackGenerator destructor frees memory for all tracks
 */
TrackGenerator::~TrackGenerator() {
	delete [] _num_tracks;
	delete [] _azim_weights;

	for (int i = 0; i < _num_azim; i++)
		delete [] _tracks[i];
	delete [] _tracks;
}


/**
 * Return the azimuthal weights array
 * @return the array of azimuthal weights
 */
double* TrackGenerator::getAzimWeights() const {
    return _azim_weights;
}


/**
 * Return the number of azimuthal angles
 * @return the number of azimuthal angles
 */
int TrackGenerator::getNumAzim() const {
    return _num_azim*2.0;
}


/**
 * Return the number of tracks array
 * @return the number of tracks
 */
int *TrackGenerator::getNumTracks() const {
    return _num_tracks;
}


/**
 * Return the track spacing array
 * @return the track spacing array
 */
double TrackGenerator::getSpacing() const {
    return _spacing;
}


/**
 * Return the 2D jagged array of track pointers
 * @return the 2D jagged array of tracks
 */
Track **TrackGenerator::getTracks() const {
    return _tracks;
}


/**
 * Computes the effective angles and track spacings. Computes the number of tracks for each
 * azimuthal angle, allocates memory for all tracks at each angle and sets each track's
 * starting and ending points, azimuthal weight, and azimuthal angle.
 */
void TrackGenerator::generateTracks() {

	/* Check to make sure that height, width of the geometry are nonzero */
	if (_geom->getHeight() <= 0 || _geom->getHeight() <= 0)
		log_printf(ERROR, "The total height and width of the geometry must be"
				"nonzero for track generation. Please specify the height and "
				"width in the geometry input file.");


	try {
		log_printf(NORMAL, "Computing azimuthal angles and track spacings...");

		/* Each element in following lists corresponds to a track angle in phi_eff */
		/* Track spacing along x-axis, y-axis, and perpendicular to each track */
		double* dx_eff = new double[_num_azim];
		double* dy_eff = new double[_num_azim];
		double* d_eff = new double[_num_azim];

		/* Effective azimuthal angles with respect to positive x-axis */
		double* phi_eff = new double[_num_azim];

		double x1, x2;
		double iazim = _num_azim*2.0;
		double width = _geom->getWidth();
		double height = _geom->getHeight();


		/* Determine azimuthal angles and track spacing */
		for (int i = 0; i < _num_azim; i++) {

			double phi = 2.0 * M_PI / iazim * (0.5 + i); 				/* desired angle */
			_num_x[i] = (int) (fabs(width / _spacing * sin(phi))) + 1; 		/* num intersections with y-axis */
			_num_y[i] = (int) (fabs(height / _spacing * cos(phi))) + 1; 	/* num intersections with x-axis */
			_num_tracks[i] = _num_x[i] + _num_y[i]; 							/* total num of tracks */
			phi_eff[i] = atan((height * _num_x[i]) / (width * _num_y[i])); 		/* effective angle */

			/* fix angles in range(pi/2, pi) due to atan function return values */
			if (phi > M_PI / 2)
				phi_eff[i] = M_PI - phi_eff[i];

			/* Effective track spacing */
			dx_eff[i] = (width / _num_x[i]);
			dy_eff[i] = (height / _num_y[i]);
			d_eff[i] = (dx_eff[i] * sin(phi_eff[i]));
		}

		/* Compute azimuthal weights */
		for (int i = 0; i < _num_azim; i++) {

			if (i < _num_azim - 1)
				x1 = 0.5 * (phi_eff[i+1] - phi_eff[i]);
			else
				x1 = 2 * M_PI / 2.0 - phi_eff[i];

			if (i >= 1)
				x2 = 0.5 * (phi_eff[i] - phi_eff[i-1]);
			else
				x2 = phi_eff[i];

			/* Multiply weight by 2 because angles are in [0, Pi] */
			_azim_weights[i] = (x1 + x2) / (2 * M_PI) * d_eff[i] * 2;
		}

		log_printf(NORMAL, "Generating track start and end points...");

		/* Compute track starting and end points */
		for (int i = 0; i < _num_azim; i++) {

			/* Tracks for azimuthal angle i */
			_tracks[i] = new Track[_num_tracks[i]];

			/* Compute start points for tracks starting on x-axis */
			for (int j = 0; j < _num_x[i]; j++)
				_tracks[i][j].getStart()->setCoords(dx_eff[i] * (0.5 + j), 0);

			/* Compute start points for tracks starting on y-axis */
			for (int j = 0; j < _num_y[i]; j++) {

				/* If track points to the upper right */
				if (sin(phi_eff[i]) > 0 && cos(phi_eff[i]) > 0)
					_tracks[i][_num_x[i]+j].getStart()->setCoords(0, dy_eff[i] * (0.5 + j));

				/* If track points to the upper left */
				else if (sin(phi_eff[i]) > 0 && cos(phi_eff[i]) < 0)
					_tracks[i][_num_x[i]+j].getStart()->setCoords(width, dy_eff[i] * (0.5 + j));
			}

			/* Compute the exit points for each track */
			for (int j = 0; j < _num_tracks[i]; j++) {

				/* Set the track's end point */
				Point* start = _tracks[i][j].getStart();
				Point* end = _tracks[i][j].getEnd();
				computeEndPoint(start, end, phi_eff[i], width, height);

				/* Set the track's azimuthal weight */
				_tracks[i][j].setWeight(_azim_weights[i]);
			}
		}
		return;
	}

	catch (std::exception &e) {
		log_printf(ERROR, "Unable to allocate memory needed to generate tracks. "
				"Backtrace:\n%s", e.what());
	}

}


/**
 * This helper method for generateTracks finds the end point of a given track
 * with a defined start point and an angle from x-axis. This function does not
 * return a value but instead saves the x/y coordinates of the end point
 * directly within the track's end point
 * @param start pointer to the track start point
 * @param end pointer to where the end point should be stored
 * @param phi the azimuthal angle
 * @param width the width of the geometry
 * @param height the height of the geometry
 */
void TrackGenerator::computeEndPoint(Point* start, Point* end,  const double phi,
		const double width, const double height) {

	double m = sin(phi) / cos(phi); 		/* slope */
	double yin = start->getY(); 			/* y-coord */
	double xin = start->getX(); 			/* x-coord */

	try {
		Point *points = new Point[4];

		/* Determine all possible points */
		points[0].setCoords(0, yin - m * xin);
		points[1].setCoords(width, yin + m * (width - xin));
		points[2].setCoords(xin - yin / m, 0);
		points[3].setCoords(xin - (yin - height) / m, height);

		// For each of the possible intersection points
		for (int i = 0; i < 4; i++) {
			/* neglect the trivial point (xin, yin) */
			if (points[i].getX() == xin && points[i].getY() == yin) { }

			/* The point to return will be within the bounds of the cell */
			else if (points[i].getX() >= 0 && points[i].getX() <= width
					&& points[i].getY() >= 0 && points[i].getY() <= height) {
				end->setCoords(points[i].getX(), points[i].getY());
			}
		}

		delete[] points;
		return;
	}

	catch (std::exception &e) {
		log_printf(ERROR, "Unable to allocate memory for intersection points in "
				"computeEndPoint method of TrackGenerator. Backtrace:\n%s", e.what());
	}

}


/**
 *  Implements reflective boundary conditions by setting the incoming
 *  and outgoing tracks for each track using a special indexing scheme
 *  into the trackgenerator's 2d jagged array of tracks
 */
void TrackGenerator::makeReflective() {
	log_printf(NORMAL, "Creating reflective boundary conditions...");

	int nxi, nyi, nti; /* nx, ny, nt for a particular angle */
	Track *curr;
	Track *refl;

	/* Loop over only half the angles since we will set the pointers for
	 * connecting tracks at the same time
	 */
	for (int i = 0; i < floor(_num_azim / 2); i++) {
		nxi = _num_x[i];
		nyi = _num_y[i];
		nti = _num_tracks[i];
		curr = _tracks[i];
		refl = _tracks[_num_azim - i - 1];

		/* Loop over all of the tracks for this angle */
		for (int j = 0; j < nti; j++) {
			/* More tracks starting along x-axis than y-axis */
			if (nxi <= nyi) {
				/* Bottom to right hand side */
				if (j < nxi) {
					curr[j].setTrackIn(&refl[j]);
					refl[j].setTrackIn(&curr[j]);
					curr[j].setReflIn(false);
					refl[j].setReflIn(false);

					curr[j].setTrackOut(&refl[2 * nxi - 1 - j]);
					refl[2 * nxi - 1 - j].setTrackIn(&curr[j]);
					curr[j].setReflOut(false);
					refl[2 * nxi - 1 - j].setReflIn(true);
				}

				/* Left hand side to right hand side */
				else if (j < nyi) {
					curr[j].setTrackIn(&refl[j - nxi]);
					refl[j - nxi].setTrackOut(&curr[j]);
					curr[j].setReflIn(true);
					refl[j - nxi].setReflOut(false);

					curr[j].setTrackOut(&refl[j + nxi]);
					refl[j + nxi].setTrackIn(&curr[j]);
					curr[j].setReflOut(false);
					refl[j + nxi].setReflIn(true);
				}

				/* Left hand side to top (j > ny) */
				else {
					curr[j].setTrackIn(&refl[j - nxi]);
					refl[j - nxi].setTrackOut(&curr[j]);
					curr[j].setReflIn(true);
					refl[j - nxi].setReflOut(false);

					curr[j].setTrackOut(&refl[2 * nti - nxi - j - 1]);
					refl[2 * nti - nxi - j - 1].setTrackOut(&curr[j]);
					curr[j].setReflOut(true);
					refl[2 * nti - nxi - j - 1].setReflOut(true);
				}
			}

			/* More tracks starting on y-axis than on x-axis */
			else {
				/* Bottom to top */
				if (j < nxi - nyi) {
					curr[j].setTrackIn(&refl[j]);
					refl[j].setTrackIn(&curr[j]);
					curr[j].setReflIn(false);
					refl[j].setReflIn(false);

					curr[j].setTrackOut(&refl[nti - (nxi - nyi) + j]);
					refl[nti - (nxi - nyi) + j].setTrackOut(&curr[j]);
					curr[j].setReflOut(true);
					refl[nti - (nxi - nyi) + j].setReflOut(true);
				}

				/* Bottom to right hand side */
				else if (j < nxi) {
					curr[j].setTrackIn(&refl[j]);
					refl[j].setTrackIn(&curr[j]);
					curr[j].setReflIn(false);
					refl[j].setReflIn(false);

					curr[j].setTrackOut(&refl[nxi + (nxi - j) - 1]);
					refl[nxi + (nxi - j) - 1].setTrackIn(&curr[j]);
					curr[j].setReflOut(false);
					refl[nxi + (nxi - j) - 1].setReflIn(true);
				}

				/* Left-hand side to top (j > nx) */
				else {
					curr[j].setTrackIn(&refl[j - nxi]);
					refl[j - nxi].setTrackOut(&curr[j]);
					curr[j].setReflIn(true);
					refl[j - nxi].setReflOut(false);

					curr[j].setTrackOut(&refl[nyi + (nti - j) - 1]);
					refl[nyi + (nti - j) - 1].setTrackOut(&curr[j]);
					curr[j].setReflOut(true);
					refl[nyi + (nti - j) - 1].setReflOut(true);
				}
			}
		}
	}

	return;
}
