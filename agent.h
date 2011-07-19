#ifndef AGENT_H
#define AGENT_H

typedef struct agent_type {
  float pitch;
  float bias;
  Scale *scale;
  float power;
  struct agent_type **collisions;
  int collision_count;
} Agent;

#endif

void agent_create(Agent *agent, float initial_pitch, Scale *initial_scale,
                  int agent_count);
void agent_destroy(Agent *agent);
void agent_update(Agent *agent);
int agent_get_scaled_note(Agent *agent);
