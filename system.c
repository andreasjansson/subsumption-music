#include "source.h"

Agent *agents;
int agent_count;
int loop_time;
int *initial_scale;
int pitch_range_low;
int pitch_range_high;
int max_jump;
double radius_factor;
Gem *gems;

void system_init()
{
  if(!config_lookup_int(&cfg, "agent_count", &agent_count))
    config_die("agent_count");

  if(!config_lookup_int(&cfg, "loop_time", &loop_time))
    config_die("loop_time");
  
  if(!config_lookup_int(&cfg, "pitch_range_low", &pitch_range_low))
    config_die("pitch_range_low");
  
  if(!config_lookup_int(&cfg, "pitch_range_high", &pitch_range_high))
    config_die("pitch_range_high");
  
  if(!config_lookup_int(&cfg, "max_jump", &max_jump))
    config_die("max_jump");
  
  if(!config_lookup_float(&cfg, "radius_factor", &radius_factor))
    config_die("radius_factor");
  
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
    gems[i].scale = calloc(scale_count, sizeof(int));
    for(j = 0; j < scale_count; j ++) {
      gems[i].scale[j] = config_setting_get_int_elem(scale_settings, j);
    }

    if(!config_setting_lookup_int(gem_setting, "time", &gems[i].time))
      config_die("gem.time");

    if(!config_setting_lookup_int(gem_setting, "pitch", &gems[i].pitch))
      config_die("gem.pitch");
    
    if(!config_setting_lookup_int(gem_setting, "radius", &gems[i].radius))
      config_die("gem.radius");
  }
  
  agents = calloc(agent_count, sizeof(Agent));
}

void gems_destroy()
{

}

void system_destroy()
{
  gems_destroy();
  free(agents);
}

void system_get_notes(int *notes)
{
  
}
