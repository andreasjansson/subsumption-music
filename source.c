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

#include "source.h"

double subdiv;
config_t cfg;
int note_length;
int agent_count;
Synth **synths;

static void init(void)
{
  int i;
  double bpm;
  double decay;
  int samp_rate;
  int min_burst;
  int burst_increase;
  int min_gain;
  int gain_increase;
  int detune;

  config_init(&cfg);
  if(!config_read_file(&cfg, CONFIG_FILE))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    exit(EXIT_FAILURE);
  }

  if(!config_lookup_float(&cfg, "bpm", &bpm))
    config_die("bpm");
  if(!config_lookup_float(&cfg, "subdiv", &subdiv))
    config_die("subdiv");
  if(!config_lookup_int(&cfg, "samp_rate", &samp_rate))
    config_die("samp_rate");
  if(!config_lookup_int(&cfg, "min_burst", &min_burst))
    config_die("min_burst");
  if(!config_lookup_int(&cfg, "burst_increase", &burst_increase))
    config_die("burst_increase");
  if(!config_lookup_float(&cfg, "decay", &decay))
    config_die("decay");
  if(!config_lookup_int(&cfg, "min_gain", &min_gain))
    config_die("min_gain");
  if(!config_lookup_int(&cfg, "gain_increase", &gain_increase))
    config_die("gain_increase");
  if(!config_lookup_int(&cfg, "detune", &detune))
    config_die("detune");

  note_length = (float)samp_rate * 60 * subdiv / bpm;
  system_init();
  agent_count = system_get_agent_count();

  synths = calloc(agent_count, sizeof(Synth *));
  for(i = 0; i < agent_count; i ++) {
    float pan = ((float)i / 20) * ((i % 2) * 2 - 1) + .5;
    synths[i] = synth_create(samp_rate, min_burst + i * burst_increase,
                             decay, pan, min_gain + gain_increase * i, detune);
  }
}

static void loop(void)
{
  Synth *synth;
  float freq;
  float buffer_l[note_length];
  float buffer_r[note_length];
  float synth_buffer_l[note_length];
  float synth_buffer_r[note_length];
  int i, j;
  int notes[agent_count];
  unsigned short left, right;
  
  while(1) {

    for(i = 0; i < note_length; i ++)
      buffer_l[i] = buffer_r[i] = 0;

    system_get_notes(notes);
    for(i = 0; i < agent_count; i ++) {
      synth = synths[i];
      freq = 16.35 * pow(2, (float)notes[i] / 12);
      synth_set_note(synth, freq);
      synth_fill_buffers(synth, synth_buffer_l, synth_buffer_r, note_length);

      // add to buffers. maybe multithread this?
      for(j = 0; j < note_length; j ++) {
        buffer_l[j] += synth_buffer_l[j];
        buffer_r[j] += synth_buffer_r[j];
      }
    }

    for(i = 0; i < note_length; i ++) {
      left = (unsigned short)((buffer_l[i] / agent_count) * (1 << 15));
      right = (unsigned short)((buffer_r[i] / agent_count) * (1 << 15));

      putchar((unsigned char)left);
      putchar(left >> 8);
      putchar((unsigned char)right);
      putchar(right >> 8);
    }

    system_update();
  }
}

int main(int argc, char **argv)
{
  init();
  loop();

  exit(EXIT_SUCCESS);
}

void config_die(const char *variable)
{
  fprintf(stderr, "Could not find configuration variable: %s\n", variable);
  exit(EXIT_FAILURE);
}
