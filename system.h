#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct {
  int *notes;
  int length;
} Scale;

typedef struct {
  Scale scale;
  int time;
  int pitch;
  int width;
  int height;
} Gem;

void system_init(void);
void system_gems_destroy(void);
void system_destroy(void);
void system_get_notes(int *notes);
int system_get_agent_count(void);

#endif
