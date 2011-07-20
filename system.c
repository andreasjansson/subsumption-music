#include "source.h"

static void system_init_config(void);
static void system_init_agents(void);
static void system_find_agent_collisions(void);

Agent *agents;
int agent_count;
int loop_time;
Scale *initial_scale;
int min_pitch;
int max_pitch;
int max_jump;
double radius_time_factor;
double radius_pitch_factor;
Gem *gems;
int time;

void system_init(void)
{
  system_init_config();
  system_init_agents();
  time = 0;
}

static void system_init_config(void)
{
  if(!config_lookup_int(&cfg, "agent_count", &agent_count))
    config_die("agent_count");

  if(!config_lookup_int(&cfg, "loop_time", &loop_time))
    config_die("loop_time");
  
  if(!config_lookup_int(&cfg, "min_pitch", &min_pitch))
    config_die("min_pitch");
  
  if(!config_lookup_int(&cfg, "max_pitch", &max_pitch))
    config_die("max_pitch");
  
  if(!config_lookup_int(&cfg, "max_jump", &max_jump))
    config_die("max_jump");
  
  if(!config_lookup_float(&cfg, "radius_pitch_factor", &radius_pitch_factor))
    config_die("radius_pitch_factor");
  
  if(!config_lookup_float(&cfg, "radius_time_factor", &radius_pitch_factor))
    config_die("radius_time_factor");
  
  config_setting_t *gem_settings;
  if(!(gem_settings = config_lookup(&cfg, "gems")))
    config_die("gems");

  int gem_count = config_setting_length(gem_settings);
  gems = calloc(gem_count, sizeof(Gem));
  config_setting_t *scale_settings, *gem_setting;
  int scale_count;
  int i, j;

  for(i = 0; i < gem_count; i ++) {
    gem_setting = config_setting_get_elem(gem_settings, i);
    if(!(scale_settings = config_setting_get_member(gem_setting, "scale")))
      config_die("gem.scale");

    scale_count = config_setting_length(scale_settings);
    gems[i].scale.notes = calloc(scale_count, sizeof(int));
    gems[i].scale.length = scale_count;
    for(j = 0; j < scale_count; j ++) {
      gems[i].scale.notes[j] = config_setting_get_int_elem(scale_settings, j);
    }

    if(!config_setting_lookup_int(gem_setting, "time", &gems[i].time))
      config_die("gem.time");

    if(!config_setting_lookup_int(gem_setting, "pitch", &gems[i].pitch))
      config_die("gem.pitch");

    int radius;
    if(!config_setting_lookup_int(gem_setting, "radius", &radius))
      config_die("gem.radius");
    gems[i].width = radius * radius_time_factor;
    gems[i].height = radius * radius_pitch_factor;
  }

  initial_scale = &gems[0].scale;
}

static void system_init_agents(void)
{
  agents = calloc(agent_count, sizeof(Agent));
  int i;
  for(i = 0; i < agent_count; i ++) {
    float initial_pitch = ((float)(max_pitch - min_pitch) /
                           (agent_count + 2)) * (i + 1) + min_pitch;
    agent_create(agents + i, initial_pitch, initial_scale, agent_count, i);
  }
}

void system_gems_destroy(void)
{

}

void system_destroy(void)
{
  system_gems_destroy();
  free(agents);
}

void system_get_notes(int *notes)
{
  system_find_agent_collisions();

  int i;
  for(i = 0; i < agent_count; i ++) {
    notes[i] = agent_get_scaled_note(agents + i);
    agent_update(agents + i, gems, time, min_pitch, max_pitch, max_jump);
  }

  time = (time + 1) % loop_time;
}

static void system_find_agent_collisions(void)
{
  int i, j;

  for(i = 0; i < agent_count; i ++)
    agents[i].collision_count = 0;

  for(i = 0; i < agent_count - 1; i ++) {
    for(j = i + 1; j < agent_count; j ++) {
      if(agents[i].pitch == agents[j].pitch) {
        agents[i].collisions[agents[i].collision_count ++] =
          &agents[j];
        agents[j].collisions[agents[j].collision_count ++] =
          &agents[i];
      }
    }
  }
}

int system_get_agent_count(void)
{
  return agent_count;
}
