#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "graphics.h"
#include "item.h"


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


#define TOptObjSet THUMB_FUNCTION(0x80766E8, void, u16, u16, u8)

// Spawn the jewel piece or CD icon when you've collected one of them.
void SpawnCollectionIndicator(u32 is_cd, u32 is_permanent) {
    u16 x;
    if ((Wario_usMukiX & 0x20) == 0)
        x = Wario_usPosX + 72;
    else
        x = Wario_usPosX - 200;
    TOptObjSet(Wario_usPosY - 160, x, is_cd + 0x41);
    LastCollectedItemStatus = 2 * is_permanent + 1;
}


static u32 GemIcons_SetUpPieces();
static u32 GemIcons_SetUpAbilities();

void GemIcons_Init() {
    Scbuf_ucStatus |= 4;
    if (LastCollectedItemStatus & 1) {
        int seq;
        if (LastCollectedItemID & ITEMBIT_ABILITY)
            seq = GemIcons_SetUpAbilities();
        else
            seq = GemIcons_SetUpPieces();
        Scbuf_ucSeq = seq;
        LastCollectedItemStatus += 1;
    } else {
        if (ucTimeUp < 8)
            return;
        if (LastCollectedItemStatus == 0)
            return;
        Scbuf_ucSeq = 6;
    }
    Scbuf_ucWork0 = 0;
}

// Initializes the collection indicator for jewels. Unlike vanilla, this looks
// at what was just grabbed and causes it to display only that. This is done
// because we want it to reflect the item, not the level state. Since jewel
// pieces are progressive in the randomizer, it's meaningless to display
// anything in the other three parts anyway.
static u32 GemIcons_SetUpPieces() {
    dmaCopy(&EmptyJewel4Tile, (void*) 0x6011C00, 2 * sizeof(Tile4bpp));
    dmaCopy(&EmptyJewel3Tile, (void*) 0x6012000, 2 * sizeof(Tile4bpp));
    return (LastCollectedItemID & 3) + 1;
}


u32 GemIcons_SetUpAbilities() {
    // Make the top two tiles empty by DMA-ing in zeros.
    DMA3COPY(&EmptyTile,
             0x6011C00,
             DMA_ENABLE | DMA_SRC_FIXED | DMA16 | 2 * sizeof(Tile4bpp) / 2);

    int abilities = WarioAbilities | AbilitiesInThisLevel;
    int new_seq;
    const Tile4bpp* ability_tile;
    if (LastCollectedItemID == ITEM_GROUND_POUND) {
        if (abilities & (1 << ABILITY_SUPER_GROUND_POUND)) {
            if (WarioAbilities & (1 << ABILITY_GROUND_POUND)) {
                new_seq = 2;
                ability_tile = &HasGroundPound1Tile;
            } else {
                new_seq = 2;
                ability_tile = &CarryingGroundPound1Tile;
            }
        } else {
            new_seq = 3;
            ability_tile = &EmptyGroundPound1Tile;
        }
        dmaCopy(ability_tile, (void*) 0x6012000, sizeof(Tile4bpp));
        ability_tile = &EmptyGroundPound2Tile;
    } else /* Grab */ {
        // Hack to make the blue W work with the red palette active
        SPRITE_PALETTE[4 * 16 + 0xF] = 0x50A5;
        if (abilities & (1 << ABILITY_HEAVY_GRAB)) {
            if (WarioAbilities & (1 << ABILITY_GRAB)) {
                new_seq = 2;
                ability_tile = &HasGrab1Tile;
            } else {
                new_seq = 2;
                ability_tile = &CarryingGrab1Tile;
            }
        } else {
            new_seq = 3;
            ability_tile = &EmptyGrab1Tile;
        }
        dmaCopy(ability_tile, (void*) 0x6012000, sizeof(Tile4bpp));
        ability_tile = &EmptyGrab2Tile;
    }

    dmaCopy(ability_tile, (void*) 0x6012020, sizeof(Tile4bpp));
    return new_seq;
}


#define TKakeraComp_SE_Set THUMB_FUNCTION(0x8078D60, void, void)
#define TKakeraIconDsp_sub THUMB_FUNCTION(0x8078D98, void, void)
static void GemIcons_SetCollectedAbility(u32);
static void GemIcons_SetCollectedPiece(u32);

void GemIcons_Update() {
    if (Scbuf_ucWork0 >= 60) {
        Scbuf_ucStatus = (Scbuf_ucStatus & 0xfd) | 4;
        Scbuf_ucSeq = 5;
        Scbuf_ucWork0 = 0x14;
        Scbuf_usPosX += 0x20;
        return;
    }

    if (Scbuf_ucWork0 == 20) {
        TKakeraComp_SE_Set();
        int position = LastCollectedItemID & 3;
        if (LastCollectedItemID & ITEMBIT_ABILITY)
            GemIcons_SetCollectedAbility(position);
        else
            GemIcons_SetCollectedPiece(position);
    }

    TKakeraIconDsp_sub();
}


static void GemIcons_SetCollectedPiece(u32 position) {
    const GemTiles* piece = &gem_graphics[position];
    const Tile4bpp* source;
    switch (LastCollectedItemStatus) {
        case 2: source = piece->carrying; break;
        case 4: source = piece->has; break;
        default: return;
    }
    Tile4bpp* destination = piece->vram;
    dmaCopy(source, destination, sizeof(Tile4bpp));
}


static void GemIcons_SetCollectedAbility(u32 position) {
    const Tile4bpp* ability_tile;
    Tile4bpp* destination_tile;

    if (LastCollectedItemID == ITEM_GRAB) {
        // Hack to make the blue W work with the red palette active
        SPRITE_PALETTE[4 * 16 + 0xF] = 0x50A5;
        ability_tile = &CarryingGrab1Tile;
    } else /* Ground Pound */ {
        ability_tile = &CarryingGroundPound1Tile;
    }

    destination_tile = (Tile4bpp*) 0x6012000;
    if (position == 2) {
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
    3,  // Enemy Jump
};

void CDIcon_Init() {
    u8 item_id = LastCollectedItemID;
    const Tile4bpp* icon;
    if ((item_id & ITEMBIT_ABILITY) == 0) {
        icon = &EmptyCDTile;
    } else {
        icon = EmptyNonProgressiveAbilityTiles + np_ability_tile_indices[item_id & 7];
    }
    // BUG: I made it do this every frame?
    dmaCopy(icon, (void*) 0x60114C0, sizeof(Tile4bpp));
}

void CDIcon_Update() {
    if (Scbuf_ucWork0 != 20)
        return;
    u8 item_id = LastCollectedItemID;
    const Tile4bpp* icon;
    if ((item_id & ITEMBIT_ABILITY) == 0) {
        icon = &HasCDTile;
    } else {
        icon = FilledNonProgressiveAbilityTiles + np_ability_tile_indices[item_id & 7];
    }
    dmaCopy(icon, (void*) 0x60114C0, sizeof(Tile4bpp));
}
