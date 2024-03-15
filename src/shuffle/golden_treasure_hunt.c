#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/types.h"
#include "unsorted/variables.h"
#include "item.h"

__attribute__((long_call)) void ItemGetFlgSet_LoadSavestateInfo2RAM(void);
extern u8 ucTakaraCount;
extern const u8 GoalType;
extern const u8 GoldenTreasuresNeeded;

u32 EnterSoundRoom() {
    if (GoalType != GOAL_TREASURE_HUNT)
        return 0;

    PassageID = PASSAGE_GOLDEN;
    InPassageLevelID = LEVEL_BOSS;
    ItemGetFlgSet_LoadSavestateInfo2RAM();
    PassageID = PASSAGE_MAX;  // Sound room
    InPassageLevelID = 0;

    if (ucTakaraCount < GoldenTreasuresNeeded)
        return 0;

    GlobalGameMode = 0;
    sGameSeq = -3;
    m4aSongNumStart(BGM_AFTER_DIVA_ESCAPE);
    W4ItemStatus[PASSAGE_GOLDEN][LEVEL_BOSS] |= 0x10;
    return 1;
}