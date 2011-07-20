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
	unsigned short left, right;
  int i, j;

  config_init(&cfg);
  if(!config_read_file(&cfg, CONFIG_FILE))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  system_init();
  int agent_count = system_get_agent_count();
  int notes[agent_count];

  float *buffer_l = calloc(NOTE_LENGTH, sizeof(float));
  float *buffer_r = calloc(NOTE_LENGTH, sizeof(float));
  float *synth_buffer_l = calloc(NOTE_LENGTH, sizeof(float));
  float *synth_buffer_r = calloc(NOTE_LENGTH, sizeof(float));

  Synth **synths = calloc(agent_count, sizeof(Synth *));
  for(i = 0; i < agent_count; i ++)
    synths[i] = synth_create(SAMP_RATE, 100, .9999, .5, 50, 2);

  Synth *synth;
  float freq;
  while(1) {
    for(i = 0; i < NOTE_LENGTH; i ++)
      buffer_l[i] = buffer_r[i] = 0;

    system_get_notes(notes);
    for(i = 0; i < agent_count; i ++) {
      synth = synths[i];
      freq = 16.35 * pow(2, (float)notes[i] / 12);
      synth_set_note(synth, freq);
      synth_fill_buffers(synth, synth_buffer_l, synth_buffer_r, NOTE_LENGTH);

      // add to buffers. maybe multithread this?
      for(j = 0; j < NOTE_LENGTH; j ++) {
        buffer_l[j] += synth_buffer_l[j];
        buffer_r[j] += synth_buffer_r[j];
      }
    }

    for(i = 0; i < NOTE_LENGTH; i ++) {

      left = (unsigned short)((buffer_l[i] / agent_count) * (1 << 15));
      right = (unsigned short)((buffer_r[i] / agent_count) * (1 << 15));

      putchar((unsigned char)left);
      putchar(left >> 8);
      putchar((unsigned char)right);
      putchar(right >> 8);
    }
  }

  free(buffer_l);
  free(buffer_r);

  return(EXIT_SUCCESS);
}

void config_die(const char *variable)
{
  fprintf(stderr, "Could not find configuration variable: %s\n", variable);
  exit(EXIT_FAILURE);
}
