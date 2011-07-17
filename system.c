#include "source.h"

Agent *agents;
int agent_count;

void system_init()
{
  if(!config_lookup_int(&cfg, "agent_count", &agent_count))
    config_die("agent_count");
  
  agents = calloc(agent_count, sizeof(Agent));
}

void system_destroy()
{
  free(agents);
}

void system_get_notes(unsigned char *notes)
{
  
}
