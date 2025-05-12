#include "abilities.h"
#include "game_state.h"
#include "types.h"

void CooldownStart(Cooldown *cooldown) { cooldown->available_at = state->time_elapsed + cooldown->down_time; }

bool IsCooldownReady(Cooldown cooldown) { return cooldown.available_at <= state->time_elapsed; }
