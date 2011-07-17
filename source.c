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

#include "source.h"

config_t cfg;

int main(int argc, char **argv)
{
	unsigned short yn;
  int i;

  Synth *synth = synth_create(SAMP_RATE, 100, .9999, .5, 50, 2);
  float *buffer_l = calloc(NOTE_LENGTH, sizeof(float));
  float *buffer_r = calloc(NOTE_LENGTH, sizeof(float));

  int n = 0;

  config_init(&cfg);
  if(!config_read_file(&cfg, CONFIG_FILE))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  system_init();

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

  free(buffer_l);
  free(buffer_r);

  return(EXIT_SUCCESS);
}

void config_die(const char *variable)
{
  fprintf(stderr, "Unknown configuration variable: %s\n", variable);
  exit(EXIT_FAILURE);
}
