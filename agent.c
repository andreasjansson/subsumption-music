#include "source.h"

static Scale *agent_get_new_scale(Agent *agent, Gem *gems, int time);
static float agent_get_new_bias(Agent *agent, int min_pitch, int max_pitch);
static float agent_get_new_pitch(Agent *agent, int max_jump);

void agent_create(Agent *agent, float initial_pitch,
                  Scale *initial_scale, int agent_count, int index)
{
  agent->pitch = initial_pitch;
  agent->bias = 0;
  agent->scale = initial_scale;
  agent->power = 1;
  agent->collisions = calloc(agent_count - 1, sizeof(Agent *));
  agent->collision_count = 0;
  agent->index = index;
}

void agent_destroy(Agent *agent)
{
  free(agent);
}

void agent_update(Agent *agent, Gem *gems, int time, int min_pitch,
                  int max_pitch, int max_jump)
{
  Scale *scale = agent_get_new_scale(agent, gems, time);
  if(scale == NULL)
    agent->power /= 2;
  else {
    agent->scale = scale;
    agent->power = 1;
  }

  agent->bias = agent_get_new_bias(agent, min_pitch, max_pitch);
  agent->pitch = agent_get_new_pitch(agent, max_jump);
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

static Scale *agent_get_new_scale(Agent *agent, Gem *gems, int time)
{
  // check if we're on a gem
  Gem *gem;
  int i;
  for(i = 0; i < sizeof(gems) / sizeof(Gem *); i ++) {
    gem = gems + i;
    if(abs(gem->time - time) < gem->width &&
       abs(gem->pitch - agent->pitch) < gem->height)
      return &gem->scale;
  }

  // check if we're colliding with another agent.
  // if we are, check if that agent has more power.
  // if he has, get its scale.
  if(agent->collision_count == 0)
    return NULL;

  Scale *new_scale = agent->scale;
  float new_power = agent->power;
  Agent *other_agent;
  for(i = 0; i < agent->collision_count; i ++) {
    other_agent = agent->collisions[i];
    if(new_scale != other_agent->scale &&
       other_agent->power > new_power) {
      new_scale = other_agent->scale;
      new_power = other_agent->power;
    }
  }

  if(new_scale == agent->scale)
    return NULL;

  return new_scale;
}

static float agent_get_new_bias(Agent *agent, int min_pitch, int max_pitch)
{
  if(agent->pitch < min_pitch)
    return 1;
  else if(agent->pitch > max_pitch)
    return -1;

  if(agent->collisions == NULL)
    return agent->bias / 2;

  float bias = 0;
  int i;
  for(i = 0; i < agent->collision_count; i ++) {
    bias += sign(i - agent->collisions[i]->index);
  }

  return sign(bias);
}

static float agent_get_new_pitch(Agent *agent, int max_jump)
{
  float low = 0 - max_jump / 2 + (max_jump / 2) * agent->bias;
  float high = max_jump / 2 + (max_jump / 2) * agent->bias;
  float rnd = (float)rand() / RAND_MAX;
  float pitch = agent->pitch + low + rnd * (high - low);

  return pitch;
}
