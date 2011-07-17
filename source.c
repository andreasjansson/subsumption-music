/* 
 * Icecast synth experiment
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "synth.h"

#define SAMP_RATE				44100.0
#define CHANNELS				1
#define BIT_RATE				16.0

#define NOTE_LENGTH     22100

int main(int argc, char **argv)
{
	unsigned short yn;
  int i;

  Synth *synth = synth_create(SAMP_RATE, 100, .9999, .5, 50, 2);
  float *buffer_l = calloc(NOTE_LENGTH, sizeof(float));
  float *buffer_r = calloc(NOTE_LENGTH, sizeof(float));

  int n = 0;

  while(1) {

    synth_set_note(synth, 110 * pow(2, (float)n / 12));

    n = (n + 1) % 12;

    synth_fill_buffers(synth, buffer_l, buffer_r, NOTE_LENGTH);

    for(i = 0; i < NOTE_LENGTH; i ++) {

      yn = (unsigned short)(buffer_l[i] * (1 << 15));

      putchar((unsigned char)yn);
      putchar(yn >> 8);
    }
  }

	return 0;
}
