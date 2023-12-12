#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "graphics.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"


static void GameMain_CollectMultiworld();
static void GameMain_CollectJunk();

void GameMain_Rando() {
#ifdef DEBUG
    // While free movement is enabled:
    //  - D-pad to move, hold R to move faster
    //  - Press Start to center camera on Wario. Press Start again or exit free
    //    movement to return the camera to normal.
    //  - Press L + D-pad up to clear the level. RANDO: This will only give
    //    Keyzer.
    if (usTrg_KeyPress1Frame & KEY_SELECT)
        sGameSeq = 8;
#endif

    if (usWarStopFlg != 0)
        return;

    GameMain_CollectMultiworld();
    GameMain_CollectJunk();
}

static void GameMain_CollectMultiworld() {
    int item_id = ReceiveNextItem();
    if (item_id == ITEM_NONE)
        return;

    TextTimer = 120;
    GiveItem(item_id, NULL);
    LoadReceivedText();
    ItemReceivedFeedbackSound(item_id);

    if (item_id & ITEMBIT_JUNK)
        return;

    if (item_id & ITEMBIT_ABILITY) {
        int ability = item_id & 7;
        int progressive = 0;
        if (ability == ABILITY_GROUND_POUND || item_id == ITEM_GRAB) {
            progressive = 1;
            if (ability == ABILITY_GROUND_POUND &&
                HAS_ABILITY_TEMPORARY(ABILITY_SUPER_GROUND_POUND))
            {
                ability = ABILITY_SUPER_GROUND_POUND;
            } else if (ability == ABILITY_GRAB &&
                       HAS_ABILITY_TEMPORARY(ABILITY_HEAVY_GRAB))
            {
                ability = ABILITY_HEAVY_GRAB;
            }
        }
        SetTreasurePalette(AbilityPaletteTable[ability]);
        SpawnCollectionIndicator(!progressive, 1);
    } else {
        SetTreasurePalette((item_id >> 2) & 7);
        SpawnCollectionIndicator((item_id & ITEMBIT_CD) != 0, 1);
    }
}

// Give and apply queued junk items given by, for example, another player in the
// multiworld.
static void GameMain_CollectJunk(void) {
    if (QueuedFullHealthItem) {
        GiveWarioHearts(8);
        QueuedFullHealthItem = 0;
    } else if (QueuedHearts > 0 && GlobalTimer % 32 == 0) {
        GiveWarioHearts(1);
        QueuedHearts -= 1;
    }

    if (Wario_ucMiss == 0 && Wario_ucReact <= 1) {
        if (QueuedFormTraps > 0) {
            GiveTransformTrap();
            QueuedFormTraps -= 1;
        } else if (QueuedLightningTraps > 0) {
            GiveLightningTrap();
            QueuedLightningTraps -= 1;
        }
    }
}


static void GameMain_CreateTextOAM(u32);

void GameMain_RandoGraphics() {
    if (ucWarOffFlg == 0)
        GmWarioCreate();
    EnemyDisplayMain();

    if (MultiworldState >= MW_TEXT_RECEIVED_ITEM) {
        int text_type = MW_TEXT_SENDING_ITEM - MultiworldState;
        GameMain_CreateTextOAM(text_type);
        if (TextTimer != 0) {
            TextTimer -= 1;
        } else {
            MultiworldState = MW_IDLE;
        }
    }

    ClearOamBuf();
    GmapSceneCreate();
}


static void GameMain_CreateTextOAM(u32 spaces_around_3rd) {
    int attr0 = ATTR0_WIDE | ATTR0_COLOR_16 | OBJ_Y(146);
    int attr1 = ATTR1_SIZE_16 | OBJ_X(8);
    int attr2 = ATTR2_PALETTE(3) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x10C);

    // TODO: Reduce copypasta

    // 1st object
    OamBuf_AddObj(attr0, attr1, attr2);

    // 2nd object
    attr1 += OBJ_X(32);
    attr2 += OBJ_CHAR(4);
    OamBuf_AddObj(attr0, attr1, attr2);

    // 3rd object
    if (spaces_around_3rd)
        attr1 += OBJ_X(8);
    attr1 += OBJ_X(32);
    attr2 += OBJ_CHAR(4);
    OamBuf_AddObj(attr0, attr1, attr2);
    if (spaces_around_3rd)
        attr1 += OBJ_X(8);

    // 4th
    attr1 += OBJ_X(32);
    attr2 += OBJ_CHAR(0x130 - 0x114);
    OamBuf_AddObj(attr0, attr1, attr2);

    // 5th
    attr1 += OBJ_X(32);
    attr2 += OBJ_CHAR(4);
    OamBuf_AddObj(attr0, attr1, attr2);

    // 6th
    attr1 += OBJ_X(32);
    attr2 += OBJ_CHAR(0x20 - 4);
    OamBuf_AddObj(attr0, attr1, attr2);

    // 7th
    attr1 += OBJ_X(32);
    attr2 += OBJ_CHAR(4);
    OamBuf_AddObj(attr0, attr1, attr2);
}
