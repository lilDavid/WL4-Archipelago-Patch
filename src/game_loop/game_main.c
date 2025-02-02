#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "graphics.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"
#include "wario.h"


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
    GiveItem_InGame(item_id, NULL);
    VblkStatus |= VBLK_MAIN_UPDATE_TEXT;
    ItemReceivedFeedbackSound(item_id);

    ItemType item_type = Item_GetType(item_id);
    switch (item_type) {
        case ITEMTYPE_ABILITY: {
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
            VblkStatus |= VBLK_MAIN_SET_PALETTE | AbilityPaletteTable[ability];
            SpawnCollectionIndicator(!progressive, 1);
            break;
        }
        case ITEMTYPE_GEM:
        case ITEMTYPE_CD:
            VblkStatus |= VBLK_MAIN_SET_PALETTE | ((item_id >> 2) & 7);
            SpawnCollectionIndicator(item_type == ITEMTYPE_CD, 1);
            break;
        default: break;
    }
}

// Give and apply queued junk items given by, for example, another player in the
// multiworld.
static void GameMain_CollectJunk(void) {
    if (MultiworldState == MW_TEXT_FOUND_BOSS_ITEMS)
        return;

    if (QueuedFullHealthItem) {
        GiveWarioHearts(8);
        QueuedFullHealthItem = 0;
    } else if (QueuedHearts > 0 && GlobalTimer % 32 == 0) {
        GiveWarioHearts(1);
        QueuedHearts -= 1;
    }

    if (Wario.ucMiss == 0 && Wario.ucReact <= REACT_WATER) {
        int is_swimming = Wario.ucReact == REACT_WATER && HAS_ABILITY_TEMPORARY(ABILITY_SWIM);
        int favor_transforms = (QueuedFormTraps ^ QueuedLightningTraps) & 1;
        if (QueuedFormTraps > 0 && favor_transforms
            && (Wario.ucReact == REACT_NORMAL || is_swimming)
        ) {
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

    if (MultiworldState == MW_TEXT_RECEIVED_ITEM || MultiworldState == MW_TEXT_SENDING_ITEM) {
        int text_type = MW_TEXT_SENDING_ITEM - MultiworldState;
        GameMain_CreateTextOAM(text_type);
        if (TextTimer != 0) {
            TextTimer -= 1;
        } else {
            MultiworldState = MW_IDLE;
            VblkStatus |= VBLK_MAIN_UPDATE_TEXT;
        }
    }

    ClearOamBuf();
    GmapSceneCreate();
}


LONGCALL void GmVblkIntr1_InGameUpdateWarioOAMAndSpritesTiles(void);

void GameMain_ReceivedTextVBlk() {
    if (VblkStatus & VBLK_MAIN_UPDATE_TEXT) {
        switch (MultiworldState) {
            case MW_IDLE:
                SetTextColor(0x3D30);  // Return this color to normal
                break;

            case MW_TEXT_RECEIVED_ITEM:
                LoadReceivedText();
                break;

            case MW_TEXT_SENDING_ITEM: {
                int box_type = LastCollectedBox;
                int item_id = BoxContents[box_type];
                const ExtData* multi = BoxExtData[box_type];

                Tile4bpp* tiles1 = (Tile4bpp*) 0x6012180;  // Row of 12
                Tile4bpp* tiles2 = (Tile4bpp*) 0x6012600;  // Row of 8
                Tile4bpp* tiles3 = (Tile4bpp*) 0x6012A00;  // Row of 8
                if (SendMultiworldItemsImmediately) {
                    const ExtData* multi = BoxExtData[box_type];
                    if (box_type > BOX_CD)
                        box_type += 1;
                    W4ItemStatus[PassageID][InPassageLevelID] |= (1 << (box_type + 8));

                    // Sent to <Player name>
                    const u8* namebytes = multi->receiver;
                    int sent_len = sizeof(StrItemSent) - 1;  // trim space
                    int to_len = sizeof(StrItemTo);
                    SetTextColor(0x7FFF);
                    LoadSpriteString(StrItemSent, tiles1, sent_len);
                    LoadSpriteString(StrItemTo, tiles1 + sent_len, to_len);
                    namebytes = LoadSpriteString(namebytes, tiles1 + sent_len + to_len, 12 - sent_len - to_len);
                    namebytes = LoadSpriteString(namebytes, tiles2, 8);
                    LoadSpriteString(namebytes, tiles3, 8);
                } else {
                    // Item name
                    switch (item_id & 7) {
                        case AP_IC_FILLER:      SetTextColor(0x7B6B /* cyan */); break;
                        case AP_IC_PROGRESSION: SetTextColor(0x51D5 /* plum */); break;
                        case AP_IC_USEFUL:      SetTextColor(0x7DC6 /* slate blue */); break;
                        case AP_IC_TRAP:        SetTextColor(0x39DF /* salmon */); break;
                        default:                SetTextColor(0x7FFF /* white */); break;
                    }
                    const u8* namebytes = multi->item_name;
                    namebytes = LoadSpriteString(namebytes, tiles1, 12);
                    namebytes = LoadSpriteString(namebytes, tiles2, 8);
                    LoadSpriteString(namebytes, tiles3, 8);
                }
                break;
            }
        }
        VblkStatus &= ~VBLK_MAIN_UPDATE_TEXT;
    }

    if (VblkStatus & VBLK_MAIN_SET_PALETTE) {
        SetTreasurePalette(VblkStatus & ~VBLK_MAIN_SET_PALETTE);
        VblkStatus = 0;
    }

    GmVblkIntr1_InGameUpdateWarioOAMAndSpritesTiles();
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
