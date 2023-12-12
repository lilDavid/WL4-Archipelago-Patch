#include <gba.h>

#include "unsorted/variables.h"
#include "unsorted/functions.h"
#include "unsorted/macros.h"
#include "unsorted/types.h"
#include "graphics.h"
#include "item.h"
#include "item_table.h"
#include "multiworld.h"


#ifdef DEBUG
void PassageSelect_DebugSetFlagsWithL();
#endif

static void LoadPyramidBG3();
static void LoadMessageBG();
void PassageSelect_ShowReceivedItem();

u32 PassageSelect_Rando() {
#ifdef DEBUG
    PassageSelect_DebugSetFlagsWithL();
#endif

    if (MultiworldState == MW_TEXT_RECEIVED_ITEM) {
        if (TextTimer == 0) {
            if (usTrg_KeyPress1Frame & KEY_A) {
                m4aSongNumStart(SE_CONFIRM);
                LoadPyramidBG3();
                MultiworldState = MW_IDLE;
            }
        } else {
            TextTimer -= 1;
        }
        return 1;
    }

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
    LoadMessageBG();
    PassageSelect_ShowReceivedItem();
    TextTimer = 15;

    return 0;
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


void PassageSelect_CreateReceivedOAM() {
    if (MultiworldState != MW_TEXT_RECEIVED_ITEM)
        return;

    const int center_x = SCREEN_WIDTH / 2;

    if (IncomingItemID & ITEMBIT_JUNK) {
        switch (IncomingItemID) {
            case ITEM_FULL_HEALTH_ITEM:
                OamBuf_AddObj(
                    ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(104),
                    ATTR1_SIZE_16 | OBJ_X(center_x - 8),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x200)
                );
                OamBuf_AddObj(
                    ATTR0_WIDE | ATTR0_COLOR_16 | OBJ_Y(104 - 8),
                    ATTR1_SIZE_8 | OBJ_X(center_x - 8),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x202)
                );
                break;

            case ITEM_HEART:
                OamBuf_AddObj(
                    ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(104),
                    ATTR1_SIZE_16 | OBJ_X(center_x - 8),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x200)
                );
                break;

            case ITEM_MINIGAME_COIN:
                OamBuf_AddObj(
                    ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(104),
                    ATTR1_SIZE_16 | OBJ_X(center_x - 8),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x200)
                );
                break;

            default:
                OamBuf_AddObj(
                    ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(104 - 4),
                    ATTR1_SIZE_16 | OBJ_X(center_x - 12),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x200)
                );
                OamBuf_AddObj(
                    ATTR0_TALL | ATTR0_COLOR_16 | OBJ_Y(104 - 4),
                    ATTR1_SIZE_8 | OBJ_X(center_x + 4),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x202)
                );
                int x = center_x - 12;
                if (IncomingItemID == ITEM_WARIO_FORM_TRAP)
                    x -= 8;  // Wario is padded on left, and lightning on right
                OamBuf_AddObj(
                    ATTR0_WIDE | ATTR0_COLOR_16 | OBJ_Y(104 - 4 + 16),
                    ATTR1_SIZE_16 | OBJ_X(x),
                    ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x203)
                );
                break;
        }
    } else if ((IncomingItemID & ITEMBIT_CD) &&
               !(IncomingItemID & ITEMBIT_ABILITY)) {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(104 - 16),
            ATTR1_SIZE_32 | OBJ_X(center_x - 8),
            ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x200)
        );
    } else /* Gem piece or ability */ {
        OamBuf_AddObj(
            ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(104),
            ATTR1_SIZE_16 | OBJ_X(center_x - 8),
            ATTR2_PALETTE(0xF) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x200)
        );
    }
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
    if (IncomingItemID & ITEMBIT_JUNK) {
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

            default:
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
    } else if (IncomingItemID & ITEMBIT_ABILITY) {
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
    } else {
        int passage = (IncomingItemID >> 2) & 7;
        if (IncomingItemID & ITEMBIT_CD) {
            dmaCopy(PortalPaletteETable[passage - 1],
                    palette_dest,
                    16 * sizeof(u16));
            dmaCopy(PortalOBJTileset + 0x4A, item_dest, 100 * sizeof(Tile4bpp));
        } else {
            int piece = IncomingItemID & 3;
            dmaCopy(PortalPaletteDTable[passage],
                    palette_dest,
                    16 * sizeof(u16));
            dmaCopy(PortalOBJTileset + gem_tile_offsets[piece],
                    item_dest,
                    34 * sizeof(Tile4bpp));
        }
    }
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
