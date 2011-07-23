/* 
 * subsumption-music
 * Copyright (C) 2011  Andreas Jansson <andreas@jansson.me.uk>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

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
#define CONFIG_FILE     "source.cfg"

// not a very safe way of getting the signum, but i promise i'll
// never sign(x ++);
#define sign(x) (((x) > 0) - ((x) < 0))

#include "synth.h"
#include "system.h"
#include "agent.h"

void config_die(const char *variable);

extern config_t cfg;
extern double subdiv;

#endif
