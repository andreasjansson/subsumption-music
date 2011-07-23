/* 
 * subsumption-music
 * Copyright (C) 2011  Andreas Jansson <andreas@jansson.me.uk>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

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
void system_update(void);
int system_get_agent_count(void);

#endif
