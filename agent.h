#ifndef AGENT_H
#define AGENT_H

typedef struct agent_type {
  float pitch;
  float bias;
  Scale *scale;
  float power;
  struct agent_type **collisions;
  int collision_count;

  // used to determine where to go if collision
  int index;
} Agent;

#endif

void agent_create(Agent *agent, float initial_pitch, Scale *initial_scale,
                  int agent_count, int index);
void agent_destroy(Agent *agent);
void agent_update(Agent *agent, Gem *gems, int gem_count,
                  int time, int min_pitch,
                  int max_pitch, int max_jump);
int agent_get_scaled_note(Agent *agent);
