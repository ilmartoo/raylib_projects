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

void cooldown_start(Cooldown *cooldown);   // Starts a cooldown
bool is_cooldown_ready(Cooldown cooldown); // Check if the cooldown is up

#define COOLDOWN_DEFINITION(time) ((Cooldown){.down_time = time, .available_at = 0})

// ----------------------------------------------------------------------------
// ---- Projectile Abilities --------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct AbilityProjectile
{
    u32 damage;
    u32 range;

    Cooldown cooldown;
} AbilityProjectile;

#define ABILITY_SHOOT_DAMAGE        10
#define ABILITY_SHOOT_RANGE         400
#define ABILITY_SHOOT_COOLDOWN_TIME 0.1f
#define ABILITY_SHOOT                                                                                                                      \
    ((AbilityProjectile){                                                                                                                  \
        .damage = ABILITY_SHOOT_DAMAGE, .range = ABILITY_SHOOT_RANGE, .cooldown = COOLDOWN_DEFINITION(ABILITY_SHOOT_COOLDOWN_TIME)})

#define ABILITY_SHOOT_MISSILE_DAMAGE        80
#define ABILITY_SHOOT_MISSILE_RANGE         400
#define ABILITY_SHOOT_MISSILE_COOLDOWN_TIME 6
#define ABILITY_SHOOT_MISSILE                                                                                                              \
    ((AbilityProjectile){.damage = ABILITY_SHOOT_MISSILE_DAMAGE,                                                                           \
                         .range = ABILITY_SHOOT_MISSILE_RANGE,                                                                             \
                         .cooldown = COOLDOWN_DEFINITION(ABILITY_SHOOT_MISSILE_COOLDOWN_TIME)})

#endif // __SOURCE_ABILITIES_H_