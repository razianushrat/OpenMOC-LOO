/*
 * log.cpp
 *
 *  Created on: Jan 22, 2012
 *      Author: William Boyd
 *				MIT, Course 22
 *              wboyd@mit.edu
 */

#define LOG_C

#include "log.h"


/* Default logging level is the lowest (most verbose) level */
logLevel log_level = NORMAL;


/**
 * Set the minimum level which will be printed to the console
 * @param newlevel the logging level
 */
void log_setlevel(logLevel newlevel) {
    log_level = newlevel;

    switch (newlevel) {
    	case NORMAL:
    		log_printf(INFO, "Set the logging level to NORMAL\n");
    		break;
    	case INFO:
    		log_printf(INFO, "Set the logging level to INFO\n");
    		break;
    	case WARNING:
    		log_printf(INFO, "Set the logging level to WARNING\n");
    		break;
    	case CRITICAL:
    		log_printf(INFO, "Set the logging level to CRITICAL\n");
    		break;
    	case ERROR:
    		log_printf(INFO, "Set the logging level to ERROR\n");
    		break;
    	case DEBUG:
    		log_printf(INFO, "Set the logging level to DEBUG\n");
    		break;
    	case RESULT:
    		log_printf(INFO, "Set the logging level to RESULT\n");
    		break;
    }
}


/**
 * Set the logging level from a character string which must correspond
 * to one of the logLevel enum types (NORMAL, INFO, CRITICAL, WARNING,
 * ERROR, DEBUG, RESULT)
 * @param newlevel a character string loglevel
 */
void log_setlevel(char* newlevel) {

	if (strcmp("NORMAL", newlevel) == 0) {
		log_level = NORMAL;
		log_printf(INFO, "Set the logging level to NORMAL\n");
	}
	else if (strcmp("INFO", newlevel) == 0) {
		log_level = INFO;
		log_printf(INFO, "Set the logging level to INFO\n");
	}
	else if (strcmp("WARNING", newlevel) == 0) {
		log_level = WARNING;
		log_printf(INFO, "Set the logging level to WARNING\n");
	}
	else if (strcmp("CRITICAL", newlevel) == 0) {
		log_level = CRITICAL;
		log_printf(INFO, "Set the logging level to CRITICAL\n");
	}
	else if (strcmp("ERROR", newlevel) == 0) {
		log_level = ERROR;
		log_printf(INFO, "Set the logging level to ERROR\n");
	}
	else if (strcmp("DEBUG", newlevel) == 0) {
		log_level = DEBUG;
		log_printf(INFO, "Set the logging level to DEBUG\n");
	}
	else if (strcmp("RESULT", newlevel) == 0) {
		log_level = RESULT;
		log_printf(INFO, "Set the logging level to RESULT\n");
	}

	return;
}



/**
 * Print a formatted message to the console. If logging level is
 * ERROR, this function will end the program
 * @param level the logging level for this message
 * @param *format variable list of C++ formatted i/o
 */
void log_printf(logLevel level, const char *format, ...) {
    if (level >= log_level) {
    	va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
    }
    if (level == ERROR) {
    	printf("\nExiting program\n");
    	exit(1);
    }
}
