#include "abilities.h"
#include "state.h"
#include "types.h"

void CooldownStart(Cooldown *cooldown) { cooldown->available_at = state->time_elapsed + cooldown->down_time; }

bool IsCooldownReady(Cooldown cooldown) { return cooldown.available_at <= state->time_elapsed; }
