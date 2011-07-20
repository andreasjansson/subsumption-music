#ifndef SOURCE_H
#define SOURCE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>

#define SAMP_RATE				44100.0
#define CHANNELS				1
#define BIT_RATE				16.0
#define NOTE_LENGTH     10000
#define CONFIG_FILE     "source.cfg"

// not a very safe way of getting the signum, but i promise i'll
// never sign(x ++);
#define sign(x) (((x) > 0) - ((x) < 0))

#include "synth.h"
#include "system.h"
#include "agent.h"

void config_die(const char *variable);

extern config_t cfg;

#endif
