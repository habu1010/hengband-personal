﻿/*!
 * @brief ダンジョンの壁等に関する判定関数の集合
 * @date 2020/07/18
 * @author Hourier
 * @details
 * Dance with a colorless dress. Shout with a withered voice.
 */

#include "floor/cave.h"
#include "grid/feature.h"
#include "grid/grid.h"
#include "system/floor-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "world/world.h"

/*
 * Determines if a map location is fully inside the outer walls
 */
bool in_bounds(floor_type *floor_ptr, POSITION y, POSITION x) { return (y > 0) && (x > 0) && (y < floor_ptr->height - 1) && (x < floor_ptr->width - 1); }

/*
 * Determines if a map location is on or inside the outer walls
 */
bool in_bounds2(floor_type *floor_ptr, POSITION y, POSITION x) { return (y >= 0) && (x >= 0) && (y < floor_ptr->height) && (x < floor_ptr->width); }

/*
 * Determines if a map location is on or inside the outer walls
 * (unsigned version)
 */
bool in_bounds2u(floor_type *floor_ptr, POSITION y, POSITION x) { return (y < floor_ptr->height) && (x < floor_ptr->width); }

/*
 * Determine if a "legal" grid is an "empty" floor grid
 * Determine if monsters are allowed to move into a grid
 *
 * Line 1 -- forbid non-placement grids
 * Line 2 -- forbid normal monsters
 * Line 3 -- forbid the player
 */
bool is_cave_empty_bold(player_type *player_ptr, POSITION y, POSITION x)
{
    floor_type *floor_ptr = player_ptr->current_floor_ptr;
    bool is_empty_grid = cave_have_flag_bold(floor_ptr, y, x, FF_PLACE);
    is_empty_grid &= !(floor_ptr->grid_array[y][x].m_idx);
    is_empty_grid &= !player_bold(player_ptr, y, x);
    return is_empty_grid;
}

/*
 * Determine if a "legal" grid is an "empty" floor grid
 * Determine if monster generation is allowed in a grid
 *
 * Line 1 -- forbid non-empty grids
 * Line 2 -- forbid trees while dungeon generation
 */
bool is_cave_empty_bold2(player_type *player_ptr, POSITION y, POSITION x)
{
    bool is_empty_grid = is_cave_empty_bold(player_ptr, y, x);
    is_empty_grid &= current_world_ptr->character_dungeon || !cave_have_flag_bold(player_ptr->current_floor_ptr, y, x, FF_TREE);
    return is_empty_grid;
}

bool cave_have_flag_bold(floor_type *floor_ptr, POSITION y, POSITION x, feature_flag_type f_idx)
{
    return have_flag(f_info[floor_ptr->grid_array[y][x].feat].flags, f_idx);
}

/*
 * Determine if a "legal" grid is within "los" of the player
 */
bool player_has_los_bold(player_type *player_ptr, POSITION y, POSITION x)
{
    return ((player_ptr->current_floor_ptr->grid_array[y][x].info & CAVE_VIEW) != 0) || player_ptr->phase_out;
}

/*
 * Determine if player is on this grid
 */
bool player_bold(player_type *player_ptr, POSITION y, POSITION x) { return (y == player_ptr->y) && (x == player_ptr->x); }
