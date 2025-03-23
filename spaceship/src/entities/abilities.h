#pragma once
#ifndef __SOURCE_ABILITIES_H_
#define __SOURCE_ABILITIES_H_

#include "types.h"

// ----------------------------------------------------------------------------
// ---- Cooldowns -------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Cooldown
{
    f32 down_time;    // Cooldown of the ability
    f64 available_at; // The time at which the ability will be available again
} Cooldown;

void CooldownStart(Cooldown *cooldown);  // Starts a cooldown
bool IsCooldownReady(Cooldown cooldown); // Check if the cooldown is up

#define COOLDOWN_DEFINITION(time) ((Cooldown){.down_time = time, .available_at = 0})

// ----------------------------------------------------------------------------
// ---- Regeneration Abilities ------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct AbilityRegen
{
    u32 flat;
    u8 percentage;
    Cooldown damage_cooldown;
    Cooldown regen_cooldown;
} AbilityRegen;

#define ABILITY_REGEN_DEFINITION(flat_regen, percentage_regen, damage_cooldown_time, regen_cooldown_time)                                  \
    ((AbilityRegen){.flat = flat_regen,                                                                                                    \
                    .percentage = percentage_regen,                                                                                        \
                    .damage_cooldown = COOLDOWN_DEFINITION(damage_cooldown_time),                                                          \
                    .regen_cooldown = COOLDOWN_DEFINITION(regen_cooldown_time)})

// ----------------------------------------------------------------------------
// ---- Projectile Abilities --------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct AbilityProjectile
{
    u32 damage;
    u32 range;
    Cooldown cooldown;
} AbilityProjectile;

#define ABILITY_PROJECTILE_DEFINITION(projectile_damage, projectile_range, cooldown_time)                                                  \
    ((AbilityProjectile){.damage = projectile_damage, .range = projectile_range, .cooldown = COOLDOWN_DEFINITION(cooldown_time)})

#endif // __SOURCE_ABILITIES_H_