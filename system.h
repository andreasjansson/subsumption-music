#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct {
  int *scale;
  int time;
  int pitch;
  int radius;
} Gem;

void system_init();
void system_destroy();
void system_get_notes(int *notes);

#endif
