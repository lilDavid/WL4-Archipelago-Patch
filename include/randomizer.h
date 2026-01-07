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

// The maximum value of in-game time, 99 hours 59 minutes 59 sec 59 frames
#define MAX_IN_GAME_TIME (100 * 60 * 60 * 60 - 1)

extern const u16 sLocationCount;
extern u32 gInGameTime;
extern u32 gSavedInGameTime;

extern const u8 GoalType;
extern const u8 GoldenTreasuresNeeded;
extern const u8 TrapBehavior;
extern const u8 DiamondShuffle;
