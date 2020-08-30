﻿#include "player/player-status-flags.h"
#include "art-definition/art-sword-types.h"
#include "grid/grid.h"
#include "inventory/inventory-slot-types.h"
#include "monster-race/monster-race.h"
#include "monster-race/race-flags2.h"
#include "monster-race/race-flags7.h"
#include "mutation/mutation-flag-types.h"
#include "object-enchant/object-ego.h"
#include "object-enchant/tr-types.h"
#include "object-enchant/trc-types.h"
#include "object-hook/hook-checker.h"
#include "object-hook/hook-weapon.h"
#include "object/object-flags.h"
#include "player/player-class.h"
#include "player/player-race-types.h"
#include "player/player-race.h"
#include "player/player-skill.h"
#include "player/player-status.h"
#include "player/special-defense-types.h"
#include "realm/realm-hex-numbers.h"
#include "realm/realm-song-numbers.h"
#include "realm/realm-types.h"
#include "spell-realm/spells-hex.h"
#include "sv-definition/sv-weapon-types.h"
#include "system/floor-type-definition.h"
#include "system/monster-type-definition.h"
#include "system/object-type-definition.h"
#include "util/bit-flags-calculator.h"
#include "util/quarks.h"
#include "util/string-processor.h"

static BIT_FLAGS check_equipment_flags(player_type *creature_ptr, tr_type tr_flag);

static BIT_FLAGS check_equipment_flags(player_type *creature_ptr, tr_type tr_flag)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    BIT_FLAGS result = 0L;
    for (inventory_slot_type i = INVEN_RARM; i < INVEN_TOTAL; i++) {
        o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;

        object_flags(creature_ptr, o_ptr, flgs);

        if (has_flag(flgs, tr_flag))
            result |= 0x01 << (i - INVEN_RARM);
    }
    return result;
}

bool has_kill_wall(player_type *creature_ptr)
{
    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD || music_singing(creature_ptr, MUSIC_WALL)) {
        return TRUE;
    }

    if (creature_ptr->riding) {
        monster_type *riding_m_ptr = &creature_ptr->current_floor_ptr->m_list[creature_ptr->riding];
        monster_race *riding_r_ptr = &r_info[riding_m_ptr->r_idx];
        if (riding_r_ptr->flags2 & RF2_KILL_WALL)
            return TRUE;
    }

    return FALSE;
}

bool has_pass_wall(player_type *creature_ptr)
{
    bool pow = FALSE;

    if (creature_ptr->wraith_form || creature_ptr->tim_pass_wall || (!creature_ptr->mimic_form && creature_ptr->prace == RACE_SPECTRE)) {
        pow = TRUE;
    }

    if (creature_ptr->riding) {
        monster_type *riding_m_ptr = &creature_ptr->current_floor_ptr->m_list[creature_ptr->riding];
        monster_race *riding_r_ptr = &r_info[riding_m_ptr->r_idx];
        if (!(riding_r_ptr->flags2 & RF2_PASS_WALL))
            pow = FALSE;
    }

    return pow;
}

BIT_FLAGS has_xtra_might(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_XTRA_MIGHT);
    return result;
}

BIT_FLAGS has_esp_evil(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    if (creature_ptr->realm1 == REALM_HEX) {
        if (hex_spelling(creature_ptr, HEX_DETECT_EVIL))
            result |= 0x01 << FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }
    result |= check_equipment_flags(creature_ptr, TR_ESP_EVIL);
    return result;
}

BIT_FLAGS has_esp_animal(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_ANIMAL);
    return result;
}

BIT_FLAGS has_esp_undead(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_UNDEAD);
    return result;
}

BIT_FLAGS has_esp_demon(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_DEMON);
    return result;
}

BIT_FLAGS has_esp_orc(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_ORC);
    return result;
}

BIT_FLAGS has_esp_troll(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_TROLL);
    return result;
}

BIT_FLAGS has_esp_giant(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_GIANT);
    return result;
}

BIT_FLAGS has_esp_dragon(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_DRAGON);
    return result;
}

BIT_FLAGS has_esp_human(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_HUMAN);
    return result;
}

BIT_FLAGS has_esp_good(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_GOOD);
    return result;
}

BIT_FLAGS has_esp_nonliving(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_GOOD);
    return result;
}

BIT_FLAGS has_esp_unique(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_ESP_UNIQUE);
    return result;
}

BIT_FLAGS has_esp_telepathy(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (is_time_limit_esp(creature_ptr) || creature_ptr->ult_res || (creature_ptr->special_defense & KATA_MUSOU)) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->muta3 & MUT3_ESP) {
        result |= FLAG_CAUSE_MUTATION;
    }

    if (is_specific_player_race(creature_ptr, RACE_MIND_FLAYER) && creature_ptr->lev > 29)
        result |= FLAG_CAUSE_RACE;

    if (is_specific_player_race(creature_ptr, RACE_SPECTRE) && creature_ptr->lev > 34)
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->pclass == CLASS_MINDCRAFTER && creature_ptr->lev > 39)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    result |= check_equipment_flags(creature_ptr, TR_TELEPATHY);
    return result;
}

BIT_FLAGS has_bless_blade(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_BLESSED);
    return result;
}

BIT_FLAGS has_easy2_weapon(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_EASY2_WEAPON);
    return result;
}

BIT_FLAGS has_down_saving(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_DOWN_SAVING);
    return result;
}

BIT_FLAGS has_no_ac(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_NO_AC);
    return result;
}

void has_no_flowed(player_type *creature_ptr)
{
    object_type *o_ptr;
    bool has_sw = FALSE, has_kabe = FALSE;
    OBJECT_IDX this_o_idx, next_o_idx = 0;

    creature_ptr->no_flowed = FALSE;

    if (creature_ptr->pass_wall && !creature_ptr->kill_wall)
        creature_ptr->no_flowed = TRUE;

    for (int i = 0; i < INVEN_PACK; i++) {
        if ((creature_ptr->inventory_list[i].tval == TV_NATURE_BOOK) && (creature_ptr->inventory_list[i].sval == 2))
            has_sw = TRUE;
        if ((creature_ptr->inventory_list[i].tval == TV_CRAFT_BOOK) && (creature_ptr->inventory_list[i].sval == 2))
            has_kabe = TRUE;
    }

    for (this_o_idx = creature_ptr->current_floor_ptr->grid_array[creature_ptr->y][creature_ptr->x].o_idx; this_o_idx; this_o_idx = next_o_idx) {
        o_ptr = &creature_ptr->current_floor_ptr->o_list[this_o_idx];
        next_o_idx = o_ptr->next_o_idx;

        if ((o_ptr->tval == TV_NATURE_BOOK) && (o_ptr->sval == 2))
            has_sw = TRUE;
        if ((o_ptr->tval == TV_CRAFT_BOOK) && (o_ptr->sval == 2))
            has_kabe = TRUE;
    }

    if (has_sw && ((creature_ptr->realm1 == REALM_NATURE) || (creature_ptr->realm2 == REALM_NATURE) || (creature_ptr->pclass == CLASS_SORCERER))) {
        const magic_type *s_ptr = &mp_ptr->info[REALM_NATURE - 1][SPELL_SW];
        if (creature_ptr->lev >= s_ptr->slevel)
            creature_ptr->no_flowed = TRUE;
    }

    if (has_kabe && ((creature_ptr->realm1 == REALM_CRAFT) || (creature_ptr->realm2 == REALM_CRAFT) || (creature_ptr->pclass == CLASS_SORCERER))) {
        const magic_type *s_ptr = &mp_ptr->info[REALM_CRAFT - 1][SPELL_WALL];
        if (creature_ptr->lev >= s_ptr->slevel)
            creature_ptr->no_flowed = TRUE;
    }
}

BIT_FLAGS has_mighty_throw(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_MIGHTY_THROW);
    return result;
}

BIT_FLAGS has_dec_mana(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_DEC_MANA);
    return result;
}

BIT_FLAGS has_reflect(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_BERSERKER && creature_ptr->lev > 39)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->pclass == CLASS_MIRROR_MASTER && creature_ptr->lev > 39)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->special_defense & KAMAE_GENBU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res || creature_ptr->wraith_form || creature_ptr->magicdef || creature_ptr->tim_reflect) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_REFLECT);
    return result;
}

BIT_FLAGS has_see_nocto(player_type *creature_ptr) { return creature_ptr->pclass == CLASS_NINJA ? FLAG_CAUSE_CLASS : 0L; }

void has_warning(player_type *creature_ptr)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];

    creature_ptr->warning = FALSE;

    for (inventory_slot_type i = INVEN_RARM; i < INVEN_TOTAL; i++) {
        o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;

        object_flags(creature_ptr, o_ptr, flgs);

        if (has_flag(flgs, TR_WARNING)) {
            if (!o_ptr->inscription || !(angband_strchr(quark_str(o_ptr->inscription), '$')))
                creature_ptr->warning = TRUE;
        }
    }
}

BIT_FLAGS has_anti_magic(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_NO_MAGIC);
    return result;
}

BIT_FLAGS has_anti_tele(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_NO_TELE);
    return result;
}

BIT_FLAGS has_sh_fire(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->muta3 & MUT3_FIRE_BODY) {
        result |= FLAG_CAUSE_MUTATION;
    }

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (hex_spelling(creature_ptr, HEX_DEMON_AURA) || creature_ptr->ult_res || creature_ptr->tim_sh_fire) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_SH_FIRE);
    return result;
}

BIT_FLAGS has_sh_elec(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->muta3 & MUT3_ELEC_TOUC)
        result |= FLAG_CAUSE_MUTATION;

    if (hex_spelling(creature_ptr, HEX_SHOCK_CLOAK) || creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || (creature_ptr->special_defense & KATA_MUSOU)) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SH_ELEC);
    return result;
}

BIT_FLAGS has_sh_cold(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res || hex_spelling(creature_ptr, HEX_ICE_ARMOR)) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_SH_COLD);
    return result;
}

BIT_FLAGS has_easy_spell(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_EASY_SPELL);
    return result;
}

BIT_FLAGS has_heavy_spell(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_HEAVY_SPELL);
    return result;
}

BIT_FLAGS has_hold_exp(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pseikaku == PERSONALITY_MUNCHKIN) {
        result |= FLAG_CAUSE_PERSONALITY;
    }

    if (creature_ptr->mimic_form == MIMIC_DEMON || creature_ptr->mimic_form == MIMIC_DEMON_LORD || creature_ptr->mimic_form == MIMIC_VAMPIRE) {
        result |= FLAG_CAUSE_RACE;
    }

    if (is_specific_player_race(creature_ptr, RACE_HOBBIT) || is_specific_player_race(creature_ptr, RACE_SKELETON)
        || is_specific_player_race(creature_ptr, RACE_ZOMBIE) || is_specific_player_race(creature_ptr, RACE_VAMPIRE)
        || is_specific_player_race(creature_ptr, RACE_SPECTRE) || is_specific_player_race(creature_ptr, RACE_BALROG)
        || is_specific_player_race(creature_ptr, RACE_ANDROID)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (is_specific_player_race(creature_ptr, RACE_GOLEM)) {
        if (creature_ptr->lev > 34)
            result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_HOLD_EXP);
    return result;
}

BIT_FLAGS has_see_inv(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_NINJA || creature_ptr->lev > 29)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->mimic_form == MIMIC_DEMON || creature_ptr->mimic_form == MIMIC_DEMON_LORD || creature_ptr->mimic_form == MIMIC_VAMPIRE) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form
        && (creature_ptr->prace == RACE_HIGH_ELF || creature_ptr->prace == RACE_GOLEM || creature_ptr->prace == RACE_SKELETON
            || creature_ptr->prace == RACE_ZOMBIE || creature_ptr->prace == RACE_SPECTRE || creature_ptr->prace == RACE_ARCHON)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_DARK_ELF) {
        if (creature_ptr->lev > 19)
            result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_MIND_FLAYER) {
        if (creature_ptr->lev > 14)
            result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_IMP || creature_ptr->prace == RACE_BALROG)) {
        if (creature_ptr->lev > 9)
            result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res || creature_ptr->tim_invis) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

	result |= check_equipment_flags(creature_ptr, TR_SEE_INVIS);
    return result;

}

BIT_FLAGS has_free_act(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->muta3 & MUT3_MOTION)
        result |= FLAG_CAUSE_MUTATION;

    if (is_specific_player_race(creature_ptr, RACE_GNOME) || is_specific_player_race(creature_ptr, RACE_GOLEM)
        || is_specific_player_race(creature_ptr, RACE_SPECTRE) || is_specific_player_race(creature_ptr, RACE_ANDROID)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (heavy_armor(creature_ptr) && (!creature_ptr->inventory_list[INVEN_RARM].k_idx || has_right_hand_weapon(creature_ptr))
        && (!creature_ptr->inventory_list[INVEN_LARM].k_idx || has_left_hand_weapon(creature_ptr))) {
        if (creature_ptr->lev > 24)
            result |= FLAG_CAUSE_CLASS;
    }

    if (creature_ptr->pclass == CLASS_MONK || creature_ptr->pclass == CLASS_FORCETRAINER) {
        if (!(heavy_armor(creature_ptr))) {
            if (creature_ptr->lev > 24)
                result |= FLAG_CAUSE_CLASS;
        }
    }

    if (creature_ptr->pclass == CLASS_BERSERKER) {
        result |= FLAG_CAUSE_CLASS;
    }

    if (creature_ptr->ult_res || creature_ptr->magicdef) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_FREE_ACT);
    return result;
}

BIT_FLAGS has_sustain_str(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_BERSERKER) {
        result |= FLAG_CAUSE_CLASS;
    }
    if (is_specific_player_race(creature_ptr, RACE_HALF_TROLL) || is_specific_player_race(creature_ptr, RACE_HALF_OGRE)
        || is_specific_player_race(creature_ptr, RACE_HALF_GIANT)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SUST_STR);
    return result;
}

BIT_FLAGS has_sustain_int(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (is_specific_player_race(creature_ptr, RACE_MIND_FLAYER)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SUST_INT);
    return result;
}

BIT_FLAGS has_sustain_wis(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    result = FALSE;
    if (creature_ptr->pclass == CLASS_MINDCRAFTER && creature_ptr->lev > 19)
        result |= FLAG_CAUSE_CLASS;

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_MIND_FLAYER)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SUST_WIS);
    return result;
}

BIT_FLAGS has_sustain_dex(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    if (creature_ptr->pclass == CLASS_BERSERKER) {
        result |= FLAG_CAUSE_CLASS;
    }

    if (creature_ptr->pclass == CLASS_NINJA && creature_ptr->lev > 24)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SUST_DEX);
    return result;
}

BIT_FLAGS has_sustain_con(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    if (creature_ptr->pclass == CLASS_BERSERKER) {
        result |= FLAG_CAUSE_CLASS;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_AMBERITE || creature_ptr->prace == RACE_DUNADAN)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SUST_CON);
    return result;
}

BIT_FLAGS has_sustain_chr(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_SUST_CHR);
    return result;
}

BIT_FLAGS has_levitation(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->muta3 & MUT3_WINGS)
        result = FLAG_CAUSE_MUTATION;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result = FLAG_CAUSE_RACE;
    }

    if (is_specific_player_race(creature_ptr, RACE_DRACONIAN) || is_specific_player_race(creature_ptr, RACE_SPECTRE)
        || is_specific_player_race(creature_ptr, RACE_SPRITE) || is_specific_player_race(creature_ptr, RACE_ARCHON)
        || is_specific_player_race(creature_ptr, RACE_S_FAIRY)) {
        result = FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KAMAE_SUZAKU || (creature_ptr->special_defense & KATA_MUSOU)) {
        result = FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->riding) {
        monster_type *riding_m_ptr = &creature_ptr->current_floor_ptr->m_list[creature_ptr->riding];
        monster_race *riding_r_ptr = &r_info[riding_m_ptr->r_idx];
        result = (riding_r_ptr->flags7 & RF7_CAN_FLY) ? result : 0;
    }

    if (creature_ptr->tim_levitation) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_LEVITATION);
    return result;
}

void has_can_swim(player_type *creature_ptr)
{
    creature_ptr->can_swim = FALSE;
    if (creature_ptr->riding) {
        monster_type *riding_m_ptr = &creature_ptr->current_floor_ptr->m_list[creature_ptr->riding];
        monster_race *riding_r_ptr = &r_info[riding_m_ptr->r_idx];
        if (riding_r_ptr->flags7 & (RF7_CAN_SWIM | RF7_AQUATIC))
            creature_ptr->can_swim = TRUE;
    }
}

void has_slow_digest(player_type *creature_ptr)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    creature_ptr->slow_digest = FALSE;

    if (creature_ptr->pclass == CLASS_NINJA) {
        creature_ptr->slow_digest = TRUE;
    }

    if (creature_ptr->lev > 14 && !creature_ptr->mimic_form && creature_ptr->prace == RACE_HALF_TROLL) {
        if (creature_ptr->pclass == CLASS_WARRIOR || creature_ptr->pclass == CLASS_BERSERKER) {
            creature_ptr->slow_digest = TRUE;
            /* Let's not make Regeneration
             * a disadvantage for the poor warriors who can
             * never learn a spell that satisfies hunger (actually
             * neither can rogues, but half-trolls are not
             * supposed to play rogues) */
        }
    }

    if (creature_ptr->ult_res || (creature_ptr->special_defense & KATA_MUSOU)) {
        creature_ptr->slow_digest = TRUE;
    }

    if (!creature_ptr->mimic_form
        && (creature_ptr->prace == RACE_GOLEM || creature_ptr->prace == RACE_ZOMBIE || creature_ptr->prace == RACE_SPECTRE
            || creature_ptr->prace == RACE_ANDROID)) {
        creature_ptr->slow_digest = TRUE;
    }

    for (inventory_slot_type i = INVEN_RARM; i < INVEN_TOTAL; i++) {
        o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;
        object_flags(creature_ptr, o_ptr, flgs);
        if (has_flag(flgs, TR_SLOW_DIGEST))
            creature_ptr->slow_digest = TRUE;
    }
}

BIT_FLAGS has_regenerate(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (is_specific_player_race(creature_ptr, RACE_HALF_TROLL) && creature_ptr->lev > 14) {
        result |= FLAG_CAUSE_RACE;
    }

    if (is_specific_player_race(creature_ptr, RACE_AMBERITE)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->pclass == CLASS_WARRIOR && creature_ptr->lev > 44) {
        result |= FLAG_CAUSE_RACE;    
	}

	if (creature_ptr->pclass == CLASS_BERSERKER) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->muta3 & MUT3_REGEN)
        result |= FLAG_CAUSE_MUTATION;

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_MUTATION;
    }

    if (hex_spelling(creature_ptr, HEX_DEMON_AURA) || creature_ptr->ult_res || creature_ptr->tim_regen) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_REGEN);

    if (creature_ptr->muta3 & MUT3_FLESH_ROT)
        result = 0L;

    return result;
}

void has_curses(player_type *creature_ptr)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    creature_ptr->cursed = 0L;

    if (creature_ptr->pseikaku == PERSONALITY_SEXY)
        creature_ptr->cursed |= (TRC_AGGRAVATE);

    for (inventory_slot_type i = INVEN_RARM; i < INVEN_TOTAL; i++) {
        o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;
        object_flags(creature_ptr, o_ptr, flgs);
        if (has_flag(flgs, TR_AGGRAVATE))
            creature_ptr->cursed |= TRC_AGGRAVATE;
        if (has_flag(flgs, TR_DRAIN_EXP))
            creature_ptr->cursed |= TRC_DRAIN_EXP;
        if (has_flag(flgs, TR_TY_CURSE))
            creature_ptr->cursed |= TRC_TY_CURSE;
        if (has_flag(flgs, TR_ADD_L_CURSE))
            creature_ptr->cursed |= TRC_ADD_L_CURSE;
        if (has_flag(flgs, TR_ADD_H_CURSE))
            creature_ptr->cursed |= TRC_ADD_H_CURSE;
        if (has_flag(flgs, TR_DRAIN_HP))
            creature_ptr->cursed |= TRC_DRAIN_HP;
        if (has_flag(flgs, TR_DRAIN_MANA))
            creature_ptr->cursed |= TRC_DRAIN_MANA;
        if (has_flag(flgs, TR_CALL_ANIMAL))
            creature_ptr->cursed |= TRC_CALL_ANIMAL;
        if (has_flag(flgs, TR_CALL_DEMON))
            creature_ptr->cursed |= TRC_CALL_DEMON;
        if (has_flag(flgs, TR_CALL_DRAGON))
            creature_ptr->cursed |= TRC_CALL_DRAGON;
        if (has_flag(flgs, TR_CALL_UNDEAD))
            creature_ptr->cursed |= TRC_CALL_UNDEAD;
        if (has_flag(flgs, TR_COWARDICE))
            creature_ptr->cursed |= TRC_COWARDICE;
        if (has_flag(flgs, TR_LOW_MELEE))
            creature_ptr->cursed |= TRC_LOW_MELEE;
        if (has_flag(flgs, TR_LOW_AC))
            creature_ptr->cursed |= TRC_LOW_AC;
        if (has_flag(flgs, TR_LOW_MAGIC))
            creature_ptr->cursed |= TRC_LOW_MAGIC;
        if (has_flag(flgs, TR_FAST_DIGEST))
            creature_ptr->cursed |= TRC_FAST_DIGEST;
        if (has_flag(flgs, TR_SLOW_REGEN))
            creature_ptr->cursed |= TRC_SLOW_REGEN;

        creature_ptr->cursed |= (o_ptr->curse_flags & (0xFFFFFFF0L));
        if (o_ptr->name1 == ART_CHAINSWORD)
            creature_ptr->cursed |= TRC_CHAINSWORD;

        if (has_flag(flgs, TR_TELEPORT)) {
            if (object_is_cursed(o_ptr))
                creature_ptr->cursed |= TRC_TELEPORT;
            else {
                concptr insc = quark_str(o_ptr->inscription);

                /* {.} will stop random teleportation. */
                if (o_ptr->inscription && angband_strchr(insc, '.')) {
                } else {
                    creature_ptr->cursed |= TRC_TELEPORT_SELF;
                }
            }
        }
    }

    if (creature_ptr->cursed & TRC_TELEPORT)
        creature_ptr->cursed &= ~(TRC_TELEPORT_SELF);

    if ((is_specific_player_race(creature_ptr, RACE_S_FAIRY)) && (creature_ptr->pseikaku != PERSONALITY_SEXY) && (creature_ptr->cursed & TRC_AGGRAVATE)) {
        creature_ptr->cursed &= ~(TRC_AGGRAVATE);
    }
}

BIT_FLAGS has_impact(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    result |= check_equipment_flags(creature_ptr, TR_IMPACT);
    return result;
}

void has_extra_blow(player_type *creature_ptr)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    creature_ptr->extra_blows[0] = creature_ptr->extra_blows[1] = 0;

    for (inventory_slot_type i = INVEN_RARM; i < INVEN_TOTAL; i++) {
        o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;

        object_flags(creature_ptr, o_ptr, flgs);

        if (has_flag(flgs, TR_INFRA))
            creature_ptr->see_infra += o_ptr->pval;
        if (has_flag(flgs, TR_BLOWS)) {
            if ((i == INVEN_RARM || i == INVEN_RIGHT) && !has_two_handed_weapons(creature_ptr))
                creature_ptr->extra_blows[0] += o_ptr->pval;
            else if ((i == INVEN_LARM || i == INVEN_LEFT) && !has_two_handed_weapons(creature_ptr))
                creature_ptr->extra_blows[1] += o_ptr->pval;
            else {
                creature_ptr->extra_blows[0] += o_ptr->pval;
                creature_ptr->extra_blows[1] += o_ptr->pval;
            }
        }
    }
}

BIT_FLAGS has_resist_acid(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_YEEK || creature_ptr->prace == RACE_KLACKON)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_DRACONIAN && creature_ptr->lev > 14) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= has_immune_acid(creature_ptr);

    result |= check_equipment_flags(creature_ptr, TR_RES_ACID);
    return result;

}

BIT_FLAGS has_resist_elec(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_DRACONIAN && creature_ptr->lev > 19) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_ELEC);
    result |= has_immune_elec(creature_ptr);
    return result;
}

BIT_FLAGS has_resist_fire(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON || creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_DRACONIAN && creature_ptr->lev > 4) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_IMP || creature_ptr->prace == RACE_BALROG)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
	}

    result |= check_equipment_flags(creature_ptr, TR_RES_FIRE);
    result |= has_immune_fire(creature_ptr);
    return result;
}

BIT_FLAGS has_resist_cold(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD || creature_ptr->mimic_form == MIMIC_VAMPIRE) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_ZOMBIE) && creature_ptr->lev > 4) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_DRACONIAN || creature_ptr->prace == RACE_SKELETON) && creature_ptr->lev > 9) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_VAMPIRE || creature_ptr->prace == RACE_SPECTRE)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_COLD);
    result |= has_immune_cold(creature_ptr);
    return result;
}

BIT_FLAGS has_resist_pois(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_NINJA && creature_ptr->lev > 19)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->mimic_form == MIMIC_VAMPIRE || creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_DRACONIAN && creature_ptr->lev > 34) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form
        && (creature_ptr->prace == RACE_KOBOLD || creature_ptr->prace == RACE_GOLEM || creature_ptr->prace == RACE_SKELETON
            || creature_ptr->prace == RACE_VAMPIRE || creature_ptr->prace == RACE_SPECTRE || creature_ptr->prace == RACE_ANDROID)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KAMAE_SEIRYU || creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_POIS);
    return result;
}

BIT_FLAGS has_resist_conf(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_MINDCRAFTER && creature_ptr->lev > 29)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->pseikaku == PERSONALITY_CHARGEMAN || creature_ptr->pseikaku == PERSONALITY_MUNCHKIN) {
        result |= FLAG_CAUSE_PERSONALITY;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_KLACKON || creature_ptr->prace == RACE_BEASTMAN || creature_ptr->prace == RACE_KUTAR)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->ult_res || creature_ptr->magicdef) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_CONF);
    return result;
}

BIT_FLAGS has_resist_sound(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_BARD) {
        result |= FLAG_CAUSE_CLASS;
    }

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_CYCLOPS || creature_ptr->prace == RACE_BEASTMAN)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_SOUND);
    return result;
}

BIT_FLAGS has_resist_lite(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_ELF || creature_ptr->prace == RACE_HIGH_ELF || creature_ptr->prace == RACE_SPRITE)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_LITE);
    return result;
}

BIT_FLAGS has_resist_dark(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_VAMPIRE) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form
        && (creature_ptr->prace == RACE_HALF_ORC || creature_ptr->prace == RACE_HALF_OGRE || creature_ptr->prace == RACE_NIBELUNG
            || creature_ptr->prace == RACE_DARK_ELF || creature_ptr->prace == RACE_VAMPIRE)) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_DARK);
    return result;
}

BIT_FLAGS has_resist_chaos(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pclass == CLASS_CHAOS_WARRIOR && creature_ptr->lev > 29)
        result |= FLAG_CAUSE_CLASS;

    if (creature_ptr->mimic_form == MIMIC_DEMON || creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_HALF_TITAN)
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_CHAOS);
    return result;
}

BIT_FLAGS has_resist_disen(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_NIBELUNG)
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_DISEN);
    return result;
}

BIT_FLAGS has_resist_shard(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (!creature_ptr->mimic_form && (creature_ptr->prace == RACE_HALF_TITAN || creature_ptr->prace == RACE_SKELETON))
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_SHARDS);
    return result;
}

BIT_FLAGS has_resist_nexus(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        result |= FLAG_CAUSE_RACE;
    }

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_NEXUS);
    return result;
}

BIT_FLAGS has_resist_blind(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->pseikaku == PERSONALITY_MUNCHKIN) {
        result |= FLAG_CAUSE_PERSONALITY;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_DWARF)
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res || creature_ptr->magicdef) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_BLIND);
    return result;
}

BIT_FLAGS has_resist_neth(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD || creature_ptr->mimic_form == MIMIC_DEMON || creature_ptr->mimic_form == MIMIC_VAMPIRE) {
        result |= FLAG_CAUSE_RACE;
    }

    if (!creature_ptr->mimic_form
        && (creature_ptr->prace == RACE_ZOMBIE || creature_ptr->prace == RACE_VAMPIRE || creature_ptr->prace == RACE_SPECTRE
            || creature_ptr->prace == RACE_BALROG))
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->special_defense & KATA_MUSOU) {
        result |= FLAG_CAUSE_BATTLE_FORM;
    }

    if (creature_ptr->ult_res || creature_ptr->tim_res_nether) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_NETHER);
    return result;
}

BIT_FLAGS has_resist_time(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (creature_ptr->tim_res_time) {
        result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_RES_TIME);
    return result;
}

BIT_FLAGS has_resist_water(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_MERFOLK)
        result |= FLAG_CAUSE_RACE;

    return result;
}

void has_resist_fear(player_type *creature_ptr)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    creature_ptr->resist_fear = FALSE;

    if (creature_ptr->muta3 & MUT3_FEARLESS)
        creature_ptr->resist_fear = TRUE;

    switch (creature_ptr->pclass) {
    case CLASS_WARRIOR:
        if (creature_ptr->lev > 29)
            creature_ptr->resist_fear = TRUE;
        break;
    case CLASS_PALADIN:
        if (creature_ptr->lev > 39)
            creature_ptr->resist_fear = TRUE;
        break;
    case CLASS_CHAOS_WARRIOR:
        if (creature_ptr->lev > 39)
            creature_ptr->resist_fear = TRUE;
        break;
    case CLASS_MINDCRAFTER:
        if (creature_ptr->lev > 9)
            creature_ptr->resist_fear = TRUE;
        break;
    case CLASS_SAMURAI:
        if (creature_ptr->lev > 29)
            creature_ptr->resist_fear = TRUE;
        break;
    case CLASS_NINJA:
        creature_ptr->resist_fear = TRUE;
        break;
    }

    if (creature_ptr->mimic_form == MIMIC_DEMON_LORD) {
        creature_ptr->resist_fear = TRUE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_BARBARIAN)
        creature_ptr->resist_fear = TRUE;

    if (creature_ptr->ult_res || (creature_ptr->special_defense & KATA_MUSOU)) {
        creature_ptr->resist_fear = TRUE;
    }

    if (is_hero(creature_ptr) || creature_ptr->shero) {
        creature_ptr->resist_fear = TRUE;
    }

    for (inventory_slot_type i = INVEN_RARM; i < INVEN_TOTAL; i++) {
        o_ptr = &creature_ptr->inventory_list[i];
        if (!o_ptr->k_idx)
            continue;

        object_flags(creature_ptr, o_ptr, flgs);
        if (has_flag(flgs, TR_RES_FEAR))
            creature_ptr->resist_fear = TRUE;
    }
}

BIT_FLAGS has_immune_acid(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;
    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_YEEK && creature_ptr->lev > 19)
        result |= FLAG_CAUSE_RACE;

    if (creature_ptr->ele_immune) {
        if (creature_ptr->special_defense & DEFENSE_ACID)
            result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_IM_ACID);
    return result;
}

BIT_FLAGS has_immune_elec(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

	if (creature_ptr->ele_immune) {
        if (creature_ptr->special_defense & DEFENSE_ELEC)
            result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_IM_ELEC);
    return result;
}

BIT_FLAGS has_immune_fire(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

	if (creature_ptr->ele_immune) {
        if (creature_ptr->special_defense & DEFENSE_FIRE)
            result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_IM_FIRE);
    return result;
}

BIT_FLAGS has_immune_cold(player_type *creature_ptr)
{
    BIT_FLAGS result = 0L;

	if (creature_ptr->ele_immune) {
        if (creature_ptr->special_defense & DEFENSE_COLD)
           result |= FLAG_CAUSE_MAGIC_TIME_EFFECT;
    }

    result |= check_equipment_flags(creature_ptr, TR_IM_COLD);
    return result;
}

bool has_right_hand_weapon(player_type *creature_ptr)
{
    if (has_melee_weapon(creature_ptr, INVEN_RARM))
        return TRUE;

    if (can_two_hands_wielding(creature_ptr)) {
        switch (creature_ptr->pclass) {
        case CLASS_MONK:
        case CLASS_FORCETRAINER:
        case CLASS_BERSERKER:
            if (empty_hands(creature_ptr, FALSE) == (EMPTY_HAND_RARM | EMPTY_HAND_LARM)) {
                return TRUE;
            }
            break;
        }
    }

    return FALSE;
}

bool has_left_hand_weapon(player_type *creature_ptr) { return has_melee_weapon(creature_ptr, INVEN_LARM); }

bool has_two_handed_weapons(player_type *creature_ptr)
{
    if (can_two_hands_wielding(creature_ptr)) {
        if (has_right_hand_weapon(creature_ptr) && (empty_hands(creature_ptr, FALSE) == EMPTY_HAND_LARM)
            && object_allow_two_hands_wielding(&creature_ptr->inventory_list[INVEN_RARM])) {
            return TRUE;
        } else if (has_left_hand_weapon(creature_ptr) && (empty_hands(creature_ptr, FALSE) == EMPTY_HAND_RARM)
            && object_allow_two_hands_wielding(&creature_ptr->inventory_list[INVEN_LARM])) {
            return TRUE;
        }
    }
    return FALSE;
}

void has_lite(player_type *creature_ptr)
{
    creature_ptr->lite = FALSE;
    if (creature_ptr->pclass == CLASS_NINJA)
        return;

    if (creature_ptr->pseikaku == PERSONALITY_MUNCHKIN) {
        creature_ptr->lite = TRUE;
    }

    if (creature_ptr->mimic_form == MIMIC_VAMPIRE) {
        creature_ptr->lite = TRUE;
    }

    if (creature_ptr->muta3 & MUT3_FIRE_BODY) {
        creature_ptr->lite = TRUE;
    }

    if (!creature_ptr->mimic_form && creature_ptr->prace == RACE_VAMPIRE)
        creature_ptr->lite = TRUE;

    if (creature_ptr->sh_fire)
        creature_ptr->lite = TRUE;

    if (creature_ptr->ult_res || (creature_ptr->special_defense & KATA_MUSOU)) {
        creature_ptr->lite = TRUE;
    }
}

bool is_disable_two_handed_bonus(player_type *creature_ptr, int i)
{
    object_type *o_ptr;
    o_ptr = &creature_ptr->inventory_list[INVEN_RARM + i];
    if (has_melee_weapon(creature_ptr, INVEN_RARM + i)) {
        if (calc_weapon_weight_limit(creature_ptr) * 2 >= o_ptr->weight / 10 && has_two_handed_weapons(creature_ptr)
            && (calc_weapon_weight_limit(creature_ptr) * 2 < o_ptr->weight / 5))
            return TRUE;
    }
    return FALSE;
}

bool is_icky_wield_weapon(player_type *creature_ptr, int i)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    o_ptr = &creature_ptr->inventory_list[INVEN_RARM + i];
    object_flags(creature_ptr, o_ptr, flgs);

    if ((creature_ptr->pclass == CLASS_PRIEST) && (!(has_flag(flgs, TR_BLESSED))) && ((o_ptr->tval == TV_SWORD) || (o_ptr->tval == TV_POLEARM))) {
        return TRUE;
    } else if (creature_ptr->pclass == CLASS_SORCERER) {
        if (!((o_ptr->tval == TV_HAFTED) && ((o_ptr->sval == SV_WIZSTAFF) || (o_ptr->sval == SV_NAMAKE_HAMMER)))) {
            return TRUE;
        }
    }
    if (is_not_monk_weapon(creature_ptr, i) || is_not_ninja_weapon(creature_ptr, i)) {
        return TRUE;
    }
    return FALSE;
}

bool is_riding_wield_weapon(player_type *creature_ptr, int i)
{
    object_type *o_ptr;
    BIT_FLAGS flgs[TR_FLAG_SIZE];
    o_ptr = &creature_ptr->inventory_list[INVEN_RARM + i];
    object_flags(creature_ptr, o_ptr, flgs);
    if (creature_ptr->riding != 0 && !(o_ptr->tval == TV_POLEARM) && ((o_ptr->sval == SV_LANCE) || (o_ptr->sval == SV_HEAVY_LANCE))
        && !has_flag(flgs, TR_RIDING)) {
        return TRUE;
    }
    return FALSE;
}

bool is_not_ninja_weapon(player_type *creature_ptr, int i)
{
    tval_type tval = creature_ptr->inventory_list[INVEN_RARM + i].tval - TV_WEAPON_BEGIN;
    OBJECT_SUBTYPE_VALUE sval = creature_ptr->inventory_list[INVEN_RARM + i].sval;
    return creature_ptr->pclass == CLASS_NINJA
        && !((s_info[CLASS_NINJA].w_max[tval][sval] > WEAPON_EXP_BEGINNER) && (creature_ptr->inventory_list[INVEN_LARM - i].tval != TV_SHIELD));
}

bool is_not_monk_weapon(player_type *creature_ptr, int i)
{
    tval_type tval = creature_ptr->inventory_list[INVEN_RARM + i].tval - TV_WEAPON_BEGIN;
    OBJECT_SUBTYPE_VALUE sval = creature_ptr->inventory_list[INVEN_RARM + i].sval;
    return (creature_ptr->pclass == CLASS_MONK) || (creature_ptr->pclass == CLASS_FORCETRAINER) && (!s_info[creature_ptr->pclass].w_max[tval][sval]);
}

bool has_good_luck(player_type *creature_ptr) { return (creature_ptr->pseikaku == PERSONALITY_LUCKY) || (creature_ptr->muta3 |= MUT3_GOOD_LUCK); };
