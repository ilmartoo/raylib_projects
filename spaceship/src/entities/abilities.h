#pragma once
#ifndef __SOURCE_ABILITIES_H_
#define __SOURCE_ABILITIES_H_

#include "types.h"

typedef struct Cooldown
{
    f32 down_time;    // Cooldown of the ability
    f64 available_at; // The time at which the ability will be available again
} Cooldown;

typedef struct AbilityProjectile
{
    u32 damage;
    u32 range;

    Cooldown cooldown
} AbilityProjectile;

#define ABILITY_SHOOT   ((AbilityProjectile){.damage = 10, .range = 400, .cooldown = {.down_time = 0.1f, .available_at = 0}})
#define ABILITY_MISSILE ((AbilityProjectile){.damage = 80, .range = 400, .cooldown = {.down_time = 10, .available_at = 0}})

void cooldown_start(Cooldown *cooldown);  // Starts a cooldown
bool is_cooldown_ready(Cooldown ability); // Check if the cooldown is up

#endif // __SOURCE_ABILITIES_H_