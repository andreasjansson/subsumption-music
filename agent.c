#include "source.h"

void agent_create(Agent *agent, float initial_pitch,
                  Scale *initial_scale, int agent_count)
{
  agent->pitch = initial_pitch;
  agent->bias = 0;
  agent->scale = initial_scale;
  agent->power = 1;
  agent->collisions = calloc(agent_count - 1, sizeof(Agent *));
  agent->collision_count = 0;
}

void agent_destroy(Agent *agent)
{
  free(agent);
}

void agent_update(Agent *agent)
{
  
}

int agent_get_scaled_note(Agent *agent)
{
  int octave = agent->pitch / 12;
  int note = (int)agent->pitch % 12;
  int scaled_note;
  int i = 0;

  while((scaled_note = agent->scale->notes[i ++]) <= note &&
        i < agent->scale->length)
    ;

  return scaled_note + octave * 12;
}
