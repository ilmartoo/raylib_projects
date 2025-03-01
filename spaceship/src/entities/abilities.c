#include "abilities.h"
#include "state.h"
#include "types.h"

void cooldown_start(Cooldown *cooldown) { cooldown->available_at = state.time_elapsed + cooldown->down_time; }

bool is_cooldown_ready(Cooldown cooldown) { return cooldown.available_at <= state.time_elapsed; }
