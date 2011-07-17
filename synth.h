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

#ifndef SYNTH_H
#define SYNTH_H

#define MIN_FREQ 20
#define MAX_FREQ 20000

typedef struct {
  unsigned int sample_rate;

  // the current note's frequency
  float freq;

  // number of samples left on the burst
  float burst_left;

  // length of noise burst in samples
  unsigned int burst;

  // length of decay. 0 == none, 1 == infinite
  float decay;

  // pan: 0 == left, .5 == centre, 1 == right
  float pan;

  // extra gain
  float gain;

  // frequency separation between left and right channels
  float detune;

  // the history array is a circular buffer used to
  // keep track of old samples
  float *history_l;
  float *history_r;
  unsigned int history_position_l;
  unsigned int history_position_r;
  unsigned int history_length;

  // previous samples for allpass and lowpass filteres
  float previous_v_l;
  float previous_w_l;
  float previous_v_r;
  float previous_w_r;
} Synth;

Synth *synth_create(unsigned int sample_rate, unsigned int burst, float decay, float pan,
                    float gain, float detune);

void synth_destroy(Synth *synth);

void synth_set_note(Synth *synth, float freq);

void synth_fill_buffers(Synth *synth, float *buffer_l, float *buffer_r,
                        unsigned long samples);

void synth_clear(Synth *synth);

#endif
