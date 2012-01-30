/*
 * Options.cpp
 *
 *  Created on: Jan 21, 2012
 *      Author: Lulu Li
 *				MIT, Course 22
 *              lululi@mit.edu
 *
 *  Stores global program options
 *
 */

#include "Options.h"

#define LAST(str) (strcmp(argv[i-1], (str)) == 0)


/**
 * Options constructor
 * @param argc the number of command line arguments from console
 * @param argv a char array of command line arguments from console
 */
Options::Options(int argc, const char **argv) {

	/* Checks the working directory to set the relative path for input files
	 * This is important so that default input files work when program is run
	 * from both eclipse and the console */
	if (std::string(getenv("PWD")).find("Release") != std::string::npos)
		_relative_path = "../";
	else
		_relative_path = "";

	_geometry_file = _relative_path + "xml-sample/5/geometry.xml"; 	 /* Default geometry input file */
	_material_file = _relative_path + "xml-sample/5/material.xml";    /* Default material input file */
	_track_spacing = 0.05;					 /* Default track spacing */
	_num_azim = 128;					 /* Default number of azimuthal angles */
	_num_sectors = 0;					 /* Default number of sectors */
	_num_rings = 0;						 /* Default number of rings */
	_sector_offset = 0;					 /* Default sector offset */
	_verbosity = "NORMAL";				 /* Default logging level */
	_dump_geometry = false;									/* Default will not dump geometry */

	for (int i = 0; i < argc; i++) {
		if (i > 0) {
			if (LAST("--geometryfile") || LAST("-g")) {
				_geometry_file = argv[i];
			}
			else if (LAST("--materialfile") || LAST("-m")) {
				_material_file = argv[i];
			}
			else if (LAST("--trackspacing") || LAST("-ts"))
				_track_spacing = atof(argv[i]);
			else if (LAST("--numazimuthal") || LAST("-na"))
				_num_azim = atoi(argv[i]);
			else if (LAST("--numrings") || LAST("-nr"))
				_num_rings = atoi(argv[i]);
			else if (LAST("--numsectors") || LAST("-ns"))
				_num_sectors = atoi(argv[i]);
			else if (LAST("--sectoroffset") || LAST("--so"))
				_sector_offset = atof(argv[i]);
			else if (LAST("--verbosity") || LAST("-v"))
				_verbosity = strdup(argv[i]);
			else if (strcmp(argv[i], "-dg") == 0 ||
					strcmp(argv[i], "--dumpgeometry") == 0)
				_dump_geometry = true;
		}
	}
}

Options::~Options(void) { }

/**
 * Returns a character array with the path to the geometry input file. By default this
 * will return the path to /xml-sample/1/geometry.xml if not set at runtime from the
 * console
 * @return path to the geometry input file
 */
const char *Options::getGeometryFile() const {
    return _geometry_file.c_str();
}

/**
 * Returns a character array with the path to the material input file. By default this
 * will return the path to /xml-sample/1/material.xml if not set at runtime from the
 * console
 * @return path to the geometry input file
 */
const char *Options::getMaterialFile() const {
    return _material_file.c_str();
}


/**
 * Returns a boolean representing whether or not to dump the geometry to the
 * console. If true, the geometry will be printed out after parsing is complete
 * @return whether or not to dump the geometry to the console
 */
bool Options::dumpGeometry() const {
	return _dump_geometry;
}


/**
 * Returns the number of azimuthal angles. By default this will return 128 angles if
 * not set at runtime from the console
 * @return the number of azimuthal angles
 */
double Options::getNumAzim() const {
    return _num_azim;
}


/**
 * Returns the track spacing. By default this will return 0.05 if not set at runtime
 * from the console
 * @return the track spacing
 */
double Options::getTrackSpacing() const {
    return _track_spacing;
}


/**
 * Returns the numbe of rings used in subdividing flat source regions
 * @return the number of rings
 */
int Options::getNumRings() const {
    return _num_rings;
}


/**
 * Returns the number of sectors used in subdividing flat source regions
 * @return the number of sectors
 */
int Options::getNumSectors() const {
    return _num_sectors;
}


/**
 * Returns the angular offset of the sectors used in subdividing the flat
 * source regions
 * @return the angular offset in degrees
 */
double Options::getSectorOffset() const {
    return _sector_offset;
}

/**
 * Returns the verbosity logging level. By default this will return NORMAL if not set
 * at runtime from the console
 * @return the verbosity
 */
const char* Options::getVerbosity() const {
    return _verbosity.c_str();
}