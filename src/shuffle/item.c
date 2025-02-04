#include <gba.h>

#include "unsorted/macros.h"
#include "unsorted/variables.h"
#include "entity.h"
#include "item_table.h"
#include "multiworld.h"
#include "wario.h"


u8 HasItemInLevel(u8 index) {
    return !!(CollectedItems & (1 << (index + (index > BOX_CD))));
}

extern u8 CurrentJewelIconPosition;

static Tile4bpp* ItemOAM[] = { (Tile4bpp*) 0x60125C0, (Tile4bpp*) 0x6017BC0 };
static const TAnmDef* ItemAnimations[] = { Item1Anm, Item2Anm };

static const TAnmDef* RandomItemTilesCreate(const Tile4bpp* top, const Tile4bpp* bottom) {
    int position = CurrentJewelIconPosition % 2;
    CurrentJewelIconPosition += 1;

    Tile4bpp* destination = ItemOAM[position];
    dmaCopy(top, destination, 2 * sizeof(Tile4bpp));
    destination += TILE_OFFSET(0, 1);
    dmaCopy(bottom, destination, 2 * sizeof(Tile4bpp));

    return ItemAnimations[position];
}

const TAnmDef* ItemLoadInGameGraphics(u8 index) {
    int item_id = ItemInCurrentLevel(index);
    ItemType item_type = Item_GetType(item_id);
    switch (item_type) {
        case ITEMTYPE_AP:
            SetTreasurePalette(PAL_AP);
            return RandomItemTilesCreate(APLogoTilesTop, APLogoTilesBottom);
        case ITEMTYPE_JUNK:
            switch (item_id) {
                case ITEM_FULL_HEALTH_ITEM:
                    return takara_Anm_01;
                case ITEM_HEART:
                    return HeartAnm;
                case ITEM_MINIGAME_COIN:
                    SetTreasurePalette(PAL_MINGAME_COIN);
                    return RandomItemTilesCreate(MinigameCoinTiles, MinigameCoinTiles + 2);
                case ITEM_DIAMOND:
                    return DiamondAnm;
                default:
                    return NULL;
            }
            break;
        case ITEMTYPE_ABILITY: {
            int ability = item_id & 7;
            if (ability == ABILITY_GROUND_POUND && HAS_ABILITY_TEMPORARY(ABILITY_GROUND_POUND)) {
                ability = ABILITY_SUPER_GROUND_POUND;
            } else if (ability == ABILITY_GRAB && HAS_ABILITY_TEMPORARY(ABILITY_GRAB)) {
                ability = ABILITY_HEAVY_GRAB;
            }
            SetTreasurePalette(AbilityPaletteTable[ability]);
            const Tile4bpp* top_row_source = AbilityIconTilesTop + 2 * ability;
            const Tile4bpp* bottom_row_source = AbilityIconTilesBottom + 2 * ability;
            return RandomItemTilesCreate(top_row_source, bottom_row_source);
        }
        case ITEMTYPE_GEM: {
            SetTreasurePalette((item_id >> 2) & 7);
            // HasGemPiece(n) does not agree with the tile order here
            // I could've done this with a table or if-else, but noooo, I haaad
            // to do it with math instead
            // 0 -> 2; 1 -> 3; 2 -> 1; 3 -> 0
            int piece = item_id & 3;
            int position = piece ^ ((piece >> 1) | 2);
            const Tile4bpp* top_row_source = BasicElementTiles + TILE_OFFSET(2 * position + 2, 3);
            const Tile4bpp* bottom_row_source = top_row_source + TILE_OFFSET(0, 1);
            return RandomItemTilesCreate(top_row_source, bottom_row_source);
        }
        case ITEMTYPE_CD:
            SetTreasurePalette((item_id >> 2) & 7);
            return takara_Anm_00;
        case ITEMTYPE_TREASURE: {
            u16* palette;
            if ((GlobalGameMode == 2 || GlobalGameMode == 8) && (sGameSeq == 2 || sGameSeq == 8))
                palette = &SPRITE_PALETTE[1 * 16];
            else
                palette = &SPRITE_PALETTE_EWRAM[1 * 16];
            dmaCopy(GoldenTreasurePalette,
                    palette,
                    16 * sizeof(u16));
            dmaCopy(GoldenTreasureTiles[item_id & 0xF],
                    (Tile4bpp*) 0x6011C40,
                    4 * sizeof(Tile4bpp));
            dmaCopy(GoldenTreasureTiles[item_id & 0xF] + 4,
                    (Tile4bpp*) 0x6012040,
                    4 * sizeof(Tile4bpp));
            dmaCopy(GoldenTreasureTiles[item_id & 0xF] + 8,
                    (Tile4bpp*) 0x6011CC0,
                    4 * sizeof(Tile4bpp));
            dmaCopy(GoldenTreasureTiles[item_id & 0xF] + 12,
                    (Tile4bpp*) 0x60120C0,
                    4 * sizeof(Tile4bpp));
            return GoldenTreasureAnm;
        }
        default:
            return NULL;
    }
}

// Collect the item in this level at a given index. If it's your own junk item,
// it gets immediately given. Otherwise, this marks the item as collected so
// that it can be properly given to you or another player when you escape the
// level.
void CollectItemInLevel(u8 index) {
    int flag = index + (index > BOX_CD);

    CollectedItems |= 1 << flag;
    LastCollectedItemIndex = index;

    int item_id = ItemInCurrentLevel(index);
    const ExtData* multi = ExtDataInCurrentLevel(index);

    if (multi != NULL) {
        m4aSongNumStart(SE_GEM_GET);
        MultiworldState = MW_TEXT_SENDING_ITEM;
        VblkStatus |= VBLK_MAIN_UPDATE_TEXT;
        TextTimer = 120;
        return;
    }

    switch(Item_GetType(item_id)) {
        case ITEMTYPE_JUNK:
            switch (item_id) {
                case ITEM_FULL_HEALTH_ITEM: GiveWarioHearts(8); break;
                case ITEM_WARIO_FORM_TRAP: GiveTransformTrap(); break;
                case ITEM_HEART: GiveWarioHearts(1); break;
                case ITEM_LIGHTNING_TRAP: GiveLightningTrap(); break;
                case ITEM_MINIGAME_COIN: m4aSongNumStart(SE_MINIGAME_COIN_GET); break;
                case ITEM_DIAMOND:
                    GmStScoreCalc(100);
                    TOptObjSet(CurrentEnemyData.YPos - 100, CurrentEnemyData.XPos - 64, 4);
                    if (Wario.ucReact == REACT_WATER)
                        m4aSongNumStart(SE_DIAMOND_GET_UNDERWATER);
                    else
                        m4aSongNumStart(SE_DIAMOND_GET);
                    break;
            }
            return;
        case ITEMTYPE_ABILITY:
            int new_ability = item_id & 7;
            if (item_id == ITEM_GROUND_POUND || item_id == ITEM_GRAB) {
                if (HAS_ABILITY_TEMPORARY(new_ability)) {
                    if (item_id == ITEM_GROUND_POUND)
                        new_ability = ABILITY_SUPER_GROUND_POUND;
                    else
                        new_ability = ABILITY_HEAVY_GRAB;
                }
                SpawnCollectionIndicator(0, 0);
            } else {
                SpawnCollectionIndicator(1, 0);
            }
            AbilitiesInThisLevel |= 1 << new_ability;
            m4aSongNumStart(SE_ALL_BOXES_GET);
            break;
        case ITEMTYPE_CD:
            SpawnCollectionIndicator(1, 0);
            m4aSongNumStart(SE_CD_GET);
            break;
        case ITEMTYPE_GEM:
            SpawnCollectionIndicator(0, 0);
            m4aSongNumStart(SE_GEM_GET);
            break;
        case ITEMTYPE_TREASURE:
            m4aSongNumStart(SE_HIGH_SCORE);
            break;
        default: return;
    }
    LastCollectedItemID = item_id;
}

static void ItemInitBoxContents() {
    CurrentEnemyData.SizeY0_Bottom = 32;
    CurrentEnemyData.SizeY1_Top = 16;
    CurrentEnemyData.SizeX_LeftRight = 16;
    CurrentEnemyData.HitboxY0 = 64;
    CurrentEnemyData.HitboxY1 = 16;
    CurrentEnemyData.HitboxX0 = 40;
    CurrentEnemyData.HitboxX1 = 36;
}

static void ItemInitDiamond() {
    CurrentEnemyData.SizeY0_Bottom = 24;
    CurrentEnemyData.SizeY1_Top = 8;
    CurrentEnemyData.SizeX_LeftRight = 16;
    CurrentEnemyData.HitboxY0 = 80;
    CurrentEnemyData.HitboxY1 = 136;
    CurrentEnemyData.HitboxX0 = 40;
    CurrentEnemyData.HitboxX1 = 36;
}

void ItemSetHitboxAndDrawDistance(u8 item_id) {
    switch (item_id) {
        case ITEM_DIAMOND: ItemInitDiamond(); break;
        default: ItemInitBoxContents(); break;
    }
}
