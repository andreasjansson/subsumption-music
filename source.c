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
      yn = (unsigned short)(buffer_l[i] * (1 << 14) + (1 << 14));

      putchar((unsigned char)yn);
      putchar(yn >> 8);
    }
  }

	return 0;
}
