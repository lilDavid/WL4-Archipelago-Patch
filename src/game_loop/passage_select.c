#include <gba.h>

#include "unsorted/variables.h"
#include "unsorted/functions.h"
#include "unsorted/macros.h"
#include "unsorted/types.h"
#include "graphics.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"
#include "wario.h"


#ifdef DEBUG
void PassageSelect_DebugSetFlagsWithL();
#endif

u32 PassageSelect_Rando() {
#ifdef DEBUG
    PassageSelect_DebugSetFlagsWithL();
#endif
    if (MultiworldState == MW_TEXT_RECEIVED_ITEM)
        return 1;

    int item_id = ReceiveNextItem();
    if (item_id == ITEM_NONE)
        return 0;

    GiveItem(item_id, NULL);
    switch (item_id) {
        case ITEM_FULL_HEALTH_ITEM:
        case ITEM_HEART:
            m4aSongNumStart(SE_GEM_GET);
            WarioVoiceSet(WV_TREASURE);
            break;
        case ITEM_WARIO_FORM_TRAP:
        case ITEM_LIGHTNING_TRAP:
            m4aSongNumStart(SE_GEM_GET);
            WarioVoiceSet(WV_HURT);
            break;
        default:
            ItemReceivedFeedbackSound(item_id);
            break;
    }
    TextTimer = 15;
    VblkStatus = VBLK_DMAP_UPDATE;

    return 0;
}


static const TObjDef msgSmallItem = {
    1,
    { ANM_OBJ(-8, -8, ATTR0_SQUARE, ATTR1_SIZE_16, 0x200, 15, 0) }
};
static const TObjDef msgLargeItem = {
    1,
    { ANM_OBJ(-16, -16, ATTR0_SQUARE, ATTR1_SIZE_32, 0x200, 15, 0) }
};
static const TObjDef msgFullHealthItem = {
    2,
    { ANM_OBJ(-8,  -4, ATTR0_SQUARE, ATTR1_SIZE_16, 0x200, 15, 0),
      ANM_OBJ(-8, -12, ATTR0_WIDE,   ATTR1_SIZE_8,  0x202, 15, 0) }
};
static const TObjDef msgWarioFormTrap = {
    3,
    { ANM_OBJ(-12, -12, ATTR0_SQUARE, ATTR1_SIZE_16, 0x200, 15, 0),
      ANM_OBJ(  4, -12, ATTR0_TALL,   ATTR1_SIZE_8,  0x202, 15, 0),
      ANM_OBJ(-20,   4, ATTR0_WIDE,   ATTR1_SIZE_16, 0x203, 15, 0) }
};
static const TObjDef msgLightningTrap = {
    3,
    { ANM_OBJ(-12, -12, ATTR0_SQUARE, ATTR1_SIZE_16, 0x200, 15, 0),
      ANM_OBJ(  4, -12, ATTR0_TALL,   ATTR1_SIZE_8,  0x202, 15, 0),
      ANM_OBJ(-12,   4, ATTR0_WIDE,   ATTR1_SIZE_16, 0x203, 15, 0) }
};

void PassageSelect_CreateReceivedOAM() {
    if (MultiworldState != MW_TEXT_RECEIVED_ITEM)
        return;

    const TObjDef* item_sprite;
    switch (Item_GetType(IncomingItemID)) {
        case ITEMTYPE_JUNK:
            switch (IncomingItemID) {
                case ITEM_FULL_HEALTH_ITEM:
                    item_sprite = &msgFullHealthItem;
                    break;
                case ITEM_HEART:
                case ITEM_MINIGAME_COIN:
                    item_sprite = &msgSmallItem;
                    break;
                case ITEM_WARIO_FORM_TRAP:
                    item_sprite = &msgWarioFormTrap;
                    break;
                case ITEM_LIGHTNING_TRAP:
                    item_sprite = &msgLightningTrap;
                    break;
                default:
                    return;
            }
            break;
        case ITEMTYPE_CD:
        case ITEMTYPE_TREASURE:
            item_sprite = &msgLargeItem;
            break;
        case ITEMTYPE_GEM:
        case ITEMTYPE_ABILITY:
            item_sprite = &msgSmallItem;
            break;
        default:
            return;
    }

    const int sprite_x = SCREEN_WIDTH / 2;
    const int sprite_y = 112;
    for (int i = 0; i < item_sprite->length; i++) {
        const TObjDef_Attr* attr = &item_sprite->objects[i];
        int y = OBJ_Y(attr->attr0 + sprite_y);
        int x = OBJ_X(attr->attr1 + sprite_x);
        int attr0 = (attr->attr0 & ~OBJ_Y(-1)) | y;
        int attr1 = (attr->attr1 & ~OBJ_X(-1)) | x;
        OamBuf_AddObj(attr0, attr1, attr->attr2);
    }
}


static void LoadPyramidBG3(void);
static void LoadMessageBG(void);
static void PassageSelect_ShowReceivedItem(void);
LONGCALL void SelectVblkIntr01();

static void PassageSelect_RandoVblk(void) {
    if (MultiworldState != MW_TEXT_RECEIVED_ITEM)
        return;

    if (VblkStatus == VBLK_DMAP_UPDATE) {
        LoadMessageBG();
        PassageSelect_ShowReceivedItem();
        VblkStatus = 0;
    }

    if (TextTimer == 0) {
        if (usTrg_KeyPress1Frame & KEY_A) {
            m4aSongNumStart(SE_CONFIRM);
            LoadPyramidBG3();
            MultiworldState = MW_IDLE;
        }
    } else {
        TextTimer -= 1;
    }
}

void PassageSelect_Vblk() {
    PassageSelect_RandoVblk();
    SelectVblkIntr01();
}


extern const u16 PortalPaletteDTable[PASSAGE_MAX][16];
extern const u16 PortalPaletteETable[4][16];
extern const Tile4bpp PortalOBJTileset[];
extern const u16 CommonRoomEntityPalettes4[4][16];
extern const u16 BigBoardEntityPalettes[][16];
extern const Tile4bpp BigBoardEntityTiles[];
static const u8 gem_tile_offsets[4] = {0x31, 0x35, 0x39, 0x3D};

void PassageSelect_ShowReceivedItem() {
    // Text
    int char_size = 2 * sizeof(Tile4bpp);  // Each character is 2 tiles
    int next_tile = 0x9000;

    MojiCreate(StrItemReceived, next_tile, sizeof(StrItemReceived));
    next_tile += sizeof(StrItemReceived) * char_size;
    MojiCreate(StrItemFrom, next_tile, sizeof(StrItemFrom));
    next_tile += sizeof(StrItemFrom) * char_size;
    int length = W4strlen(IncomingItemSender);
    MojiCreate(IncomingItemSender, next_tile, length);
    next_tile += length * char_size;
    MojiCreate(StrScreenFiller, next_tile, (0xA180 - next_tile) / char_size);

    // Sprite
    u16* palette_dest = SPRITE_PALETTE + 16 * 0xF;
    Tile4bpp* item_dest = (Tile4bpp*) 0x6014000;
    switch (Item_GetType(IncomingItemID)) {
        case ITEMTYPE_JUNK:
            switch (IncomingItemID) {
                case ITEM_FULL_HEALTH_ITEM:
                    dmaCopy(CommonRoomEntityPalettes4[2],  // normally OBP6
                            palette_dest,
                            16 * sizeof(u16));
                    dmaCopy(BasicElementTiles + TILE_OFFSET(24, 7),
                            item_dest,
                            34 * sizeof(Tile4bpp));
                    dmaCopy(BasicElementTiles + TILE_OFFSET(4, 1),
                            item_dest + 2,
                            2 * sizeof(Tile4bpp));
                    break;

                case ITEM_HEART:
                    dmaCopy(CommonRoomEntityPalettes4[3],  // normally OBP7
                            palette_dest,
                            16 * sizeof(u16));
                    dmaCopy(BasicElementTiles + TILE_OFFSET(10, 3),
                            item_dest,
                            34 * sizeof(Tile4bpp));
                    break;

                case ITEM_MINIGAME_COIN:
                    dmaCopy(PassagePaletteTable[PAL_MINGAME_COIN],
                            palette_dest + (16 - 5),
                            5 * sizeof(u16));
                    dmaCopy(MinigameCoinTiles,
                            item_dest,
                            2 * sizeof(Tile4bpp));
                    dmaCopy(MinigameCoinTiles + 2,
                            item_dest + TILE_OFFSET(0, 1),
                            2 * sizeof(Tile4bpp));
                    break;

                case ITEM_WARIO_FORM_TRAP:
                case ITEM_LIGHTNING_TRAP:
                    dmaCopy(BigBoardEntityPalettes[1],
                            palette_dest,
                            16 * sizeof(u16));
                    const Tile4bpp* top;
                    const Tile4bpp* bottom;
                    if (IncomingItemID == ITEM_WARIO_FORM_TRAP) {
                        top = BigBoardEntityTiles + TILE_OFFSET(22, 4);
                        bottom = BigBoardEntityTiles + TILE_OFFSET(28, 5);
                    } else /* Lightning */ {
                        top = BigBoardEntityTiles + TILE_OFFSET(12, 4);
                        bottom = BigBoardEntityTiles + TILE_OFFSET(25, 4);
                    }
                    dmaCopy(top, item_dest, 35 * sizeof(Tile4bpp));
                    dmaCopy(bottom, item_dest + 3, 4 * sizeof(Tile4bpp));
                    break;
            }
            break;
        case ITEMTYPE_ABILITY:
            int ability = IncomingItemID & 7;
            if (ability == ABILITY_GROUND_POUND || ability == ABILITY_GRAB) {
                if (ability == ABILITY_GROUND_POUND &&
                    (HAS_ABILITY_PERMANENT(ABILITY_SUPER_GROUND_POUND)))
                {
                    ability = ABILITY_SUPER_GROUND_POUND;
                } else if (ability == ABILITY_GRAB &&
                           (HAS_ABILITY_PERMANENT(ABILITY_HEAVY_GRAB)))
                {
                    ability = ABILITY_HEAVY_GRAB;
                }
            }
            dmaCopy(PassageTreasurePalettes[0],
                    palette_dest,
                    (16 - 5) * sizeof(u16));
            dmaCopy(PassagePaletteTable[AbilityPaletteTable[ability]],
                    palette_dest + 16 - 5,
                    5 * sizeof(u16));
            dmaCopy(AbilityIconTilesTop + 2 * ability,
                    item_dest,
                    2 * sizeof(Tile4bpp));
            dmaCopy(AbilityIconTilesBottom + 2 * ability,
                    item_dest + TILE_OFFSET(0, 1),
                    2 * sizeof(Tile4bpp));
            break;
        case ITEMTYPE_CD: {
            int passage = (IncomingItemID >> 2) & 7;
            dmaCopy(PortalPaletteETable[passage - 1],
                    palette_dest,
                    16 * sizeof(u16));
            dmaCopy(PortalOBJTileset + 0x4A, item_dest, 100 * sizeof(Tile4bpp));
            break;
        }
        case ITEMTYPE_GEM: {
            int passage = (IncomingItemID >> 2) & 7;
            int piece = IncomingItemID & 3;
            dmaCopy(PortalPaletteDTable[passage],
                    palette_dest,
                    16 * sizeof(u16));
            dmaCopy(PortalOBJTileset + gem_tile_offsets[piece],
                    item_dest,
                    34 * sizeof(Tile4bpp));
            break;
        }
        case ITEMTYPE_TREASURE: {
            int treasure = IncomingItemID & 0xF;
            dmaCopy(GoldenTreasurePalette,
                    palette_dest,
                    16 * sizeof(u16));
            dmaCopy(GoldenTreasureTiles[treasure],
                    item_dest,
                    4 * sizeof(Tile4bpp));
            dmaCopy(GoldenTreasureTiles[treasure] + 4,
                    item_dest + TILE_OFFSET(0, 1),
                    4 * sizeof(Tile4bpp));
            dmaCopy(GoldenTreasureTiles[treasure] + 8,
                    item_dest + TILE_OFFSET(0, 2),
                    4 * sizeof(Tile4bpp));
            dmaCopy(GoldenTreasureTiles[treasure] + 12,
                    item_dest + TILE_OFFSET(0, 3),
                    4 * sizeof(Tile4bpp));
            break;
        }
        default:
            break;
    }
}


extern u16 SaveTutorialTilemap[];
extern Tile4bpp PortalTileset2[];

// Load the background for the vanilla autosave tutorial
static void LoadMessageBG() {
    REG_BG3CNT = BG_SIZE_0 | SCREEN_BASE(0x1E) | BG_16_COLOR | CHAR_BASE(2) | BG_PRIORITY(0);
    // Miraculously, BGP 6 color 2 isn't used at all as far as I can tell
    BG_PALETTE[6 * 16 + 2] = 0x7FFF;

    dmaCopy(SaveTutorialTilemap, SCREEN_BASE_BLOCK(0x1E), 0x1000);
    dmaCopy(PortalTileset2, CHAR_BASE_BLOCK(2), 0x4000);
}


extern u16 PortalTilemap3[];

// Load the original pyramid background 3 graphics
static void LoadPyramidBG3() {
    REG_BG3CNT = BG_SIZE_0 | SCREEN_BASE(0x1E) | BG_16_COLOR | CHAR_BASE(0) | BG_PRIORITY(0);
    dmaCopy(PortalTilemap3, SCREEN_BASE_BLOCK(0x1E), 0x1000);
}


#ifdef DEBUG
// Ported from debug build (https://tcrf.net/Wario_Land_4/Debug_Build#Controls)
// - L + B: No items or bosses
// - L + Select: All items in Entry Passage, no Spoiled Rotten
// - L + D-pad up: All items + Spoiled Rotten only
// - L + R: All items + bosses
//
// Rando:
// - L + D-pad left: All locations
// - L + D-pad right: No locations
// - L + D-pad down: No abilities
// - L + Start: All abilities
void PassageSelect_DebugSetFlagsWithL() {
    if (KeyPressContinuous & KEY_L) {
        if (usTrg_KeyPress1Frame & KEY_B) {
            for (int i = PASSAGE_ENTRY; i < PASSAGE_MAX; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] &= ~0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0;
            }
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }
        if (usTrg_KeyPress1Frame & KEY_SELECT) {
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_1] |= 0xFF;
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_BOSS] = 0;
            for (int i = PASSAGE_EMERALD; i < PASSAGE_MAX; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] &= ~0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0;
            }
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }
        if (usTrg_KeyPress1Frame & KEY_UP) {
            for (int i = PASSAGE_ENTRY; i < PASSAGE_MAX; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] |= 0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0;
            }
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_BOSS] = 0x27;
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }
        if (usTrg_KeyPress1Frame & KEY_R) {
            for (int i = PASSAGE_ENTRY; i < PASSAGE_GOLDEN; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] |= 0xFF;
                }
                W4ItemStatus[i][LEVEL_BOSS] = 0x27;
            }
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_1] |= 0xFF;
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_BOSS] = 0;
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }

        // Rando

        // Locations
        if (usTrg_KeyPress1Frame & KEY_LEFT) {
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_1] &= 0xFF;
            for (int i = PASSAGE_EMERALD; i < PASSAGE_GOLDEN; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] &= 0xFF;
                }
            }
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_1] &= 0xFF;
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }
        if (usTrg_KeyPress1Frame & KEY_RIGHT) {
            W4ItemStatus[PASSAGE_ENTRY][LEVEL_1] |= 0xFF << 8;
            for (int i = PASSAGE_EMERALD; i < PASSAGE_GOLDEN; i++) {
                for (int j = 0; j < LEVEL_MAXLEVEL; j++) {
                    W4ItemStatus[i][j] |= 0xFF << 8;
                }
            }
            W4ItemStatus[PASSAGE_GOLDEN][LEVEL_1] |= 0xFF << 8;
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }

        // Abilities
        if (usTrg_KeyPress1Frame & KEY_START) {
            WarioAbilities = 0xFF;
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }
        if (usTrg_KeyPress1Frame & KEY_DOWN) {
            WarioAbilities = 0;
            m4aSongNumStart(SE_RESULTS_FOUND_CD);
        }
    }
}
#endif
