#pragma once

#include <gba.h>

typedef enum {
    GOAL_GOLDEN_DIVA,
    GOAL_TREASURE_HUNT,
    GOAL_DIVA_HUNT,
    GOAL_MAX
} RandoGoal;

typedef enum {
    TB_ACCUMULATE,
    TB_APPLY_ONCE,
    TB_MAX
} TrapQueueBehavior;

extern const u8 GoalType;
extern const u8 GoldenTreasuresNeeded;
extern const u8 TrapBehavior;
extern const u8 DiamondShuffle;
