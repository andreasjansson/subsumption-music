#ifndef SOURCE_H
#define SOURCE_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "synth.h"
#include "system.h"
#include "agent.h"

#define SAMP_RATE				44100.0
#define CHANNELS				1
#define BIT_RATE				16.0
#define NOTE_LENGTH     22100
#define CONFIG_FILE     "source.cfg"

void config_die(const char *variable);

extern config_t cfg;

#endif
