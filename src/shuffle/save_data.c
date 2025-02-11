#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"
#include "randomizer.h"
#include "units.h"
#include "wario.h"

LONGCALL void AutoSave_ExRead_Work(void);

void CreateStartingInventory() {
    AutoSave_ExRead_Work();

    for (int i = 0; i < PASSAGE_MAX; i++) {
        for (int j = 0; j < LEVEL_MAX; j++) {
            W4ItemStatus[i][j] |= StartingInventoryItemStatus[i][j];
        }
    }

    QueuedFullHealthItem = StartingInventoryJunkCounts[JUNK_FULL_HEALTH];
    QueuedFormTraps = StartingInventoryJunkCounts[JUNK_WARIO_FORM_TRAP];
    QueuedHearts = StartingInventoryJunkCounts[JUNK_HEART];
    QueuedLightningTraps = StartingInventoryJunkCounts[JUNK_LIGHTNING_TRAP];
    MiniGameCoinNum = StartingInventoryJunkCounts[JUNK_MINIGAME_COIN];
    iGmTotalScore = StartingInventoryJunkCounts[JUNK_DIAMOND] * CONVERT_SCORE(1000);

    WarioAbilities = StartingInventoryWarioAbilities;
}

u8* boxPosessionVariables[BOX_CD + 1] = { &Has1stGemPiece, &Has2ndGemPiece, &Has3rdGemPiece, &Has4thGemPiece, &HasCD };


// The boxes you've checked are in the second byte of the item status word.
// This was originally done with a patch, but now these functions are rewritten
// so this data is handled all in the same place.

void CheckLocations() {
    CollectedItemsCopy = CollectedItems;
    CollectedItems &= ~(W4ItemStatus[PassageID][InPassageLevelID] >> 8);
    for (int i = 0; i <= BOX_CD; i++) {
        if (HAS_BOX(i))
            HAS_BOX(i) = 3;
    }

    for (int i = BOX_GEM1; i < LOCATION_MAX; i++) {
        int flag = i + (i > BOX_CD);
        if (!(CollectedItems & (1 << flag)))
            continue;
        int item_id = ItemInCurrentLevel(i);
        const ExtData* multiworld_data = ExtDataInCurrentLevel(i);
        GiveItem_LevelEnd(item_id, multiworld_data);
    }

    if (HasKeyzer) {
        W4ItemStatus[PassageID][InPassageLevelID] |= ISB_KEYZER;
    }

    W4ItemStatus[PassageID][InPassageLevelID] |= CollectedItems << 8;
}

void CheckBossLocations() {
    unsigned int current_status = W4ItemStatus[PassageID][InPassageLevelID];
    unsigned int new_status = 0;
    if (Has1stGemPiece) {
        new_status |= ISB_GEM1;
    }
    if (Has2ndGemPiece) {
        new_status |= ISB_GEM2;
    }
    if (Has3rdGemPiece) {
        new_status |= ISB_GEM3;
    }
    if (HasKeyzer) {
        W4ItemStatus[PassageID][InPassageLevelID] |= ISB_KEYZER;
    }

    if (GoalType == GOAL_GOLDEN_DIVA) {
        W4ItemStatus[PassageID][InPassageLevelID] |= new_status;
        return;
    } else {
        CollectedItems = (current_status >> 8) ^ new_status;
        W4ItemStatus[PassageID][InPassageLevelID] |= new_status << 8;
    }

    if (CollectedItems) {
        MultiworldState = MW_TEXT_FOUND_BOSS_ITEMS;
        TextTimer = 15;
        VblkStatus = VBLK_DMAP_UPDATE;
        CollectedItems |= PassageID << 4;
    } else {
        return;
    }

    for (int i = 0; i < 3; i++) {
        int flag = (1 << i);
        if (!(CollectedItems & flag))
            continue;

        int item = ItemLocationTable[PassageID][InPassageLevelID][i];
        const ExtData* multi = ItemExtDataTable[PassageID][InPassageLevelID][i];
        GiveItem(item, multi);
    }
}

void SetItemCollection() {
    int item_status = W4ItemStatus[PassageID][InPassageLevelID];

    for (int i = 0; i <= BOX_CD; i++) {
        int has_item = item_status & (1 << (8 + i));
        if (has_item)
            has_item = 3;
        HAS_BOX(i) = has_item;
    }
    if (item_status & ISB_KEYZER) {
        HasKeyzer = 2;
    } else {
        HasKeyzer = 0;
    }

    AbilitiesInThisLevel = 0;
    CollectedItems = item_status >> 8;
}


extern u8 HasTreasure5;
extern u8 HasTreasure6;
extern u8 HasTreasure7;
extern u8 HasTreasure8;
extern u8 HasTreasure9;
extern u8 HasTreasure10;
extern u8 HasTreasure11;
extern u8 HasTreasure12;
extern u8 ucWorldNumBak[2];
extern u8 ucSaveFlg;
extern u8 ucSTEndType;

LONGCALL void BossSave();
LONGCALL void EndingSave();
LONGCALL void LoseSave();
LONGCALL void SramBackup_Auto_Write();

void BossDefeated_Save() {
    if (PassageID != 5) {
        BossSave();
        return;
    }
    if (GoalType == GOAL_GOLDEN_DIVA) {
        EndingSave();
        return;
    }

    int golden_treasure_count = 0;
    golden_treasure_count += !!Has1stGemPiece;
    golden_treasure_count += !!Has2ndGemPiece;
    golden_treasure_count += !!Has3rdGemPiece;
    golden_treasure_count += !!Has4thGemPiece;
    golden_treasure_count += !!HasTreasure5;
    golden_treasure_count += !!HasTreasure6;
    golden_treasure_count += !!HasTreasure7;
    golden_treasure_count += !!HasTreasure8;
    golden_treasure_count += !!HasTreasure9;
    golden_treasure_count += !!HasTreasure10;
    golden_treasure_count += !!HasTreasure11;
    golden_treasure_count += !!HasTreasure12;

    if (golden_treasure_count >= GoldenTreasuresNeeded) {
        EndingSave();
        return;
    }

    ucWorldNumBak[0] = PassageID;
    PassageID = PASSAGE_GOLDEN;
    InPassageLevelID = LEVEL_MAX;
    HasKeyzer = 0;
    SramBackup_Auto_Write();
    ucSaveFlg = 1;
}

void ResetTraps() {
    // Limit traps so you don't get looped
    if (QueuedFormTraps > 1)
        QueuedFormTraps = 1;
    if (QueuedLightningTraps > 1)
        QueuedLightningTraps = 1;

    TOptObjSet(Wario.usPosY, Wario.usPosX, 0x4D);
    if (InPassageLevelID == LEVEL_BOSS)
        LoseSave();
}
