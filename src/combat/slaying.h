﻿#pragma once

#include "combat/combat-options-type.h"

MULTIPLY mult_slaying(player_type *player_ptr, MULTIPLY mult, const BIT_FLAGS *flgs, monster_type *m_ptr);
MULTIPLY mult_brand(player_type *player_ptr, MULTIPLY mult, const BIT_FLAGS *flgs, monster_type *m_ptr);
HIT_POINT calc_attack_damage_with_slay(player_type *attacker_ptr, object_type *o_ptr, HIT_POINT tdam, monster_type *m_ptr, combat_options mode, bool thrown);