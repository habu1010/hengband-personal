﻿#pragma once

#include "system/angband.h"

typedef enum feature_flag_type feature_flag_type;
bool in_bounds(floor_type *floor_ptr, POSITION y, POSITION x);
bool in_bounds2(floor_type *floor_ptr, POSITION y, POSITION x);
bool in_bounds2u(floor_type *floor_ptr, POSITION y, POSITION x);
bool is_cave_empty_bold(player_type *player_ptr, POSITION x, POSITION y);
bool is_cave_empty_bold2(player_type *player_ptr, POSITION x, POSITION y);
bool cave_have_flag_bold(floor_type *floor_ptr, POSITION y, POSITION x, feature_flag_type f_idx);
bool player_has_los_bold(player_type *player_ptr, POSITION y, POSITION x);
bool player_bold(player_type *player_ptr, POSITION y, POSITION x);
