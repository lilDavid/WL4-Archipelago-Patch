#include <gba.h>

#include "unsorted/functions.h"
#include "game_state.h"
#include "graphics.h"
#include "item.h"
#include "sprite.h"
#include "units.h"
#include "wario.h"


extern const Tile4bpp EmptyJewel1Tile, CarryingJewel1Tile, HasJewel1Tile,
                      EmptyJewel2Tile, CarryingJewel2Tile, HasJewel2Tile,
                      EmptyJewel3Tile, CarryingJewel3Tile, HasJewel3Tile,
                      EmptyJewel4Tile, CarryingJewel4Tile, HasJewel4Tile;

extern const Tile4bpp EmptyGroundPound1Tile,    EmptyGroundPound2Tile,
                      EmptyGrab1Tile,           EmptyGrab2Tile,
                      CarryingGroundPound1Tile, CarryingGroundPound2Tile,
                      CarryingGrab1Tile,        CarryingGrab2Tile,
                      HasGroundPound1Tile,      HasGroundPound2Tile,
                      HasGrab1Tile,             HasGrab2Tile;

extern const Tile4bpp EmptyTile;


typedef struct {
    const Tile4bpp* empty;
    const Tile4bpp* carrying;
    const Tile4bpp* has;
    Tile4bpp* vram;
} GemTiles;

const GemTiles gem_graphics[4] = {
    { &EmptyJewel1Tile, &CarryingJewel1Tile, &HasJewel1Tile, (Tile4bpp*) 0x6011C20 },
    { &EmptyJewel2Tile, &CarryingJewel2Tile, &HasJewel2Tile, (Tile4bpp*) 0x6012020 },
    { &EmptyJewel3Tile, &CarryingJewel3Tile, &HasJewel3Tile, (Tile4bpp*) 0x6012000 },
    { &EmptyJewel4Tile, &CarryingJewel4Tile, &HasJewel4Tile, (Tile4bpp*) 0x6011C00 },
};


// Spawn the jewel piece or CD icon when you've collected one of them.
void SpawnCollectionIndicator(u32 is_cd, u32 is_permanent) {
    u16 x;
    if ((Wario.usMukiX & 0x20) == 0)
        x = Wario.usPosX + 72;
    else
        x = Wario.usPosX - (is_cd ? 136 : 200);
    Sprite_SpawnSecondary(Wario.usPosY - 160, x, is_cd + SSPRITE_TREASURE_ICONS);
    LastCollectedItemStatus = 2 * is_permanent + 1;
}


static u32 GemIcons_DecideSeq(void);
static void GemIcons_SetUpGraphics(void);

void GemIcons_Init() {
    gCurrentSecondarySprite.ucStatus |= 4;
    if (LastCollectedItemStatus & 1) {
        gCurrentSecondarySprite.ucSeq = GemIcons_DecideSeq();
        GemIcons_SetUpGraphics();
        LastCollectedItemStatus += 1;
    } else {
        if (ucTimeUp < 8)
            return;
        if (LastCollectedItemStatus == 0)
            return;
        gCurrentSecondarySprite.ucSeq = 6;
    }
    gCurrentSecondarySprite.WorkVariable0 = 0;
}

static u32 GemIcons_DecideSeq(void) {
    if (Item_GetType(LastCollectedItemID) != ITEMTYPE_ABILITY)
        return (LastCollectedItemID & 3) + 1;

    int ability;
    if (LastCollectedItemID == ITEM_GROUND_POUND)
        ability = ABILITY_SUPER_GROUND_POUND;
    else
        ability = ABILITY_HEAVY_GRAB;

    if (HAS_ABILITY_TEMPORARY(ability))
        return 2;

    return 3;
}

static void GemIcons_SetUpPieces();
static void GemIcons_SetUpAbilities();

void GemIcons_SetUpGraphics(void) {
    if (Item_GetType(LastCollectedItemID) == ITEMTYPE_ABILITY)
        GemIcons_SetUpAbilities();
    else
        GemIcons_SetUpPieces();
}

// Initializes the collection indicator for jewels. Unlike vanilla, this looks
// at what was just grabbed and causes it to display only that. This is done
// because we want it to reflect the item, not the level state. Since jewel
// pieces are progressive in the randomizer, it's meaningless to display
// anything in the other three parts anyway.
static void GemIcons_SetUpPieces() {
    dmaCopy(&EmptyJewel4Tile, (void*) 0x6011C00, 2 * sizeof(Tile4bpp));
    dmaCopy(&EmptyJewel3Tile, (void*) 0x6012000, 2 * sizeof(Tile4bpp));
}

static void GemIcons_SetUpAbilities() {
    // Make the top two tiles empty by DMA-ing in zeros.
    DMA3COPY(&EmptyTile,
             0x6011C00,
             DMA_ENABLE | DMA_SRC_FIXED | DMA16 | 2 * sizeof(Tile4bpp) / 2);

    const Tile4bpp* ability_tile;
    if (LastCollectedItemID == ITEM_GROUND_POUND) {
        if (HAS_ABILITY_TEMPORARY(ABILITY_SUPER_GROUND_POUND)) {
            if (HAS_ABILITY_PERMANENT(ABILITY_GROUND_POUND)) {
                ability_tile = &HasGroundPound1Tile;
            } else {
                ability_tile = &CarryingGroundPound1Tile;
            }
        } else {
            ability_tile = &EmptyGroundPound1Tile;
        }
        dmaCopy(ability_tile, (void*) 0x6012000, sizeof(Tile4bpp));
        ability_tile = &EmptyGroundPound2Tile;
    } else /* Grab */ {
        // Hack to make the blue W work with the red palette active
        SPRITE_PALETTE[4 * 16 + 0xF] = 0x50A5;
        if (HAS_ABILITY_TEMPORARY(ABILITY_HEAVY_GRAB)) {
            if (HAS_ABILITY_PERMANENT(ABILITY_GRAB)) {
                ability_tile = &HasGrab1Tile;
            } else {
                ability_tile = &CarryingGrab1Tile;
            }
        } else {
            ability_tile = &EmptyGrab1Tile;
        }
        dmaCopy(ability_tile, (void*) 0x6012000, sizeof(Tile4bpp));
        ability_tile = &EmptyGrab2Tile;
    }

    dmaCopy(ability_tile, (void*) 0x6012020, sizeof(Tile4bpp));
}


LONGCALL void TKakeraComp_SE_Set(void);
LONGCALL void TKakeraIconDsp_sub(void);
static void GemIcons_SetCollectedAbility();
static void GemIcons_SetCollectedPiece();

void GemIcons_Update() {
    if (gCurrentSecondarySprite.WorkVariable0 >= CONVERT_SECONDS(1)) {
        gCurrentSecondarySprite.ucStatus = (gCurrentSecondarySprite.ucStatus & 0xfd) | 4;
        gCurrentSecondarySprite.ucSeq = 5;
        gCurrentSecondarySprite.WorkVariable0 = 0x14;
        gCurrentSecondarySprite.usPosX += 0x20;
        return;
    }

    if (gCurrentSecondarySprite.WorkVariable0 == CONVERT_SECONDS(1.0/3.0)) {
        if (Item_GetType(LastCollectedItemID) == ITEMTYPE_ABILITY)
            GemIcons_SetCollectedAbility();
        else
            GemIcons_SetCollectedPiece();
    }

    TKakeraIconDsp_sub();
}


static void GemIcons_SetCollectedPiece() {
    const GemTiles* piece = &gem_graphics[LastCollectedItemID & 3];
    const Tile4bpp* source;
    switch (LastCollectedItemStatus) {
        case 2: source = piece->carrying; break;
        case 4: source = piece->has; break;
        default: return;
    }
    Tile4bpp* destination = piece->vram;
    dmaCopy(source, destination, sizeof(Tile4bpp));
}


static void GemIcons_SetCollectedAbility() {
    const Tile4bpp* ability_tile;
    Tile4bpp* destination_tile;

    if (LastCollectedItemID == ITEM_GRAB) {
        ability_tile = &CarryingGrab1Tile;
    } else /* Ground Pound */ {
        ability_tile = &CarryingGroundPound1Tile;
    }

    destination_tile = (Tile4bpp*) 0x6012000;
    if (gCurrentSecondarySprite.ucSeq == 2) {
        ability_tile += &CarryingGrab2Tile - &CarryingGrab1Tile;
        destination_tile += 1;
    }
    if (LastCollectedItemStatus == 4) {
        ability_tile += &HasGrab1Tile - &CarryingGrab1Tile;
    }

    dmaCopy(ability_tile, destination_tile, sizeof(Tile4bpp));
}


extern Tile4bpp EmptyCDTile;
extern Tile4bpp HasCDTile;
extern Tile4bpp EmptyNonProgressiveAbilityTiles[4];
extern Tile4bpp FilledNonProgressiveAbilityTiles[4];

static const u8 np_ability_tile_indices[6] = {
    0,  // Ground pound (unused)
    0,  // Swim
    1,  // Head Smash
    0,  // Grab
    2,  // Dash Attack
    3,  // Stomp Jump
};

void CDIcon_Init() {
    u8 item_id = LastCollectedItemID;
    const Tile4bpp* icon;
    if (Item_GetType(item_id) == ITEMTYPE_CD) {
        icon = &EmptyCDTile;
    } else {
        icon = EmptyNonProgressiveAbilityTiles + np_ability_tile_indices[item_id & 7];
    }
    dmaCopy(icon, (void*) 0x60114C0, sizeof(Tile4bpp));
}

void CDIcon_Update() {
    if (gCurrentSecondarySprite.WorkVariable0 != CONVERT_SECONDS(1.0/3.0))
        return;
    u8 item_id = LastCollectedItemID;
    const Tile4bpp* icon;
    if (Item_GetType(item_id) == ITEMTYPE_CD) {
        icon = &HasCDTile;
    } else {
        icon = FilledNonProgressiveAbilityTiles + np_ability_tile_indices[item_id & 7];
    }
    dmaCopy(icon, (void*) 0x60114C0, sizeof(Tile4bpp));
}
