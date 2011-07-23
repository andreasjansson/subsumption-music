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

Synth *synth_create(unsigned int sample_rate, unsigned int burst,
                    float decay, float pan, float gain, float detune)
{
  // TODO: error handling
  Synth *synth = malloc(sizeof(Synth));
  memset(synth, 0, sizeof(Synth));
  synth->sample_rate = sample_rate;
  synth->history_length = sample_rate / MIN_FREQ;
  synth->burst = burst;
  synth->decay = decay;
  synth->pan = pan;
  synth->gain = gain;
  synth->detune = detune;
  synth->history_l = calloc(synth->history_length,
                             sizeof(float));
  synth->history_r = calloc(synth->history_length,
                             sizeof(float));
  return synth;
}

void synth_destroy(Synth *synth)
{
  free(synth->history_l);
  free(synth->history_r);
  free(synth);
}

void synth_set_note(Synth *synth, float freq)
{
  //  synth_clear(synth);
  synth->freq = freq;
  synth->burst_left = synth->burst;
}

static void synth_get_delays(float freq, unsigned int *loop_delay,
                             float *a, unsigned int sample_rate)
{
  float freq_rad, delay, phase_delay;

  freq_rad = 2 * M_PI * freq / sample_rate;
  delay = (float)sample_rate / freq;
  *loop_delay = floor(delay - .5);
  phase_delay = delay - (*loop_delay + .5);
  *a = (sin(1 - phase_delay) * freq_rad / 2) /
    (sin(1 + phase_delay) * freq_rad / 2);
}

static inline void synth_write_sample(float x, float a, float decay,
                                      unsigned int loop_delay, float gain,
                                      float *history,
                                      unsigned int *history_position,
                                      unsigned int history_length,
                                      float *previous_v,
                                      float *previous_w,
                                      float *buffer, unsigned long i)
{
  float w, v, p;

  p = pow(decay, loop_delay);
  w = x * (1 - p) + p * *(history + *history_position);
  v = a * w + *previous_w - a * *previous_v;
  *(buffer + i) = (v + *previous_v) / 2;

  // add the current output sample <delay> steps ahead in the history
  // buffer. this is the way we maintain the delay.
  *(history + ((loop_delay + *history_position) %
               history_length)) = *(buffer + i);

  *(buffer + i) *= gain;
  //*(buffer + i) *= 0;

  *previous_v = v;
  *previous_w = w;

  *history_position = (*history_position + 1) % history_length;
}

void synth_fill_buffers(Synth *synth, float *buffer_l, float *buffer_r,
                        unsigned long samples)
{
  float x, a_l, a_r;
  unsigned int loop_delay_l, loop_delay_r;
  unsigned long i;

  synth_get_delays(synth->freq - synth->detune / 2, &loop_delay_l, &a_l,
                   synth->sample_rate);
  synth_get_delays(synth->freq + synth->detune / 2, &loop_delay_r, &a_r,
                   synth->sample_rate);

  for(i = 0; i < samples; i ++) {
    if(synth->burst_left > 0) {
      x = (float)random() / RAND_MAX - .5;
      synth->burst_left --;
    }
    else
      x = 0;

    synth_write_sample(x, a_l, synth->decay, loop_delay_l,
                       synth->gain * 2 * synth->pan,
                       synth->history_l, &synth->history_position_l,
                       synth->history_length,
                       &synth->previous_v_l, &synth->previous_w_l,
                       buffer_l, i);

    synth_write_sample(x, a_r, synth->decay, loop_delay_r, 
                       synth->gain * 2 * (1 - synth->pan),
                       synth->history_r, &synth->history_position_r,
                       synth->history_length,
                       &synth->previous_v_r, &synth->previous_w_r,
                       buffer_r, i);
  }
}

void synth_clear(Synth *synth)
{
  memset(synth->history_l, 0, sizeof(float) * synth->history_length);
  memset(synth->history_r, 0, sizeof(float) * synth->history_length);
}
