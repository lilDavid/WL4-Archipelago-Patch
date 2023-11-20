#include "graphics.h"


static const u16 PassagePaletteTable[PAL_MAX][5] = {
    /* Entry Passage */        { 0x7B3E, 0x723C, 0x6576, 0x58B0, 0x4C07 },
    /* Emerald Passage */      { 0x5793, 0x578D, 0x4B20, 0x2E40, 0x1160 },
    /* Ruby Passage */         { 0x6B5F, 0x529F, 0x253F, 0x14B4, 0x14AE },
    /* Topaz Passage */        { 0x6BDF, 0x23DF, 0x139B, 0x1274, 0x0DAE },
    /* Sapphire Passage */     { 0x7F5A, 0x7E94, 0x7D29, 0x50A5, 0x38A5 },
    /* Golden Pyramid */       { 0x579F, 0x3B1F, 0x1A7F, 0x05DE, 0x00FB },
    /* Archipelago item */     { 0x3D9C, 0x327D, 0x2B28, 0x6A3B, 0x6DED },
    /* Garlic (Dash Attack) */ { 0x0000, 0x6BDF, 0x3FBF, 0x22FA, 0x0DAF },
    /* Helmet (Head Smash) */  { 0x0000, 0x0000, 0x0000, 0x05DE, 0x24C5 },
};

void SetTreasurePalette(u32 palette) {
    const u16* source = PassagePaletteTable[palette];
    u16* destination = &SPRITE_PALETTE[5 * 16 - 5];
    dmaCopy(source, destination, 5 * sizeof(u16));
}

void OamBuf_AddObj(u16 attr0, u16 attr1, u16 attr2) {
    if (ucCntObj >= 0x80)
        return;
    OamBuf[ucCntObj].attr0 = attr0;
    OamBuf[ucCntObj].attr1 = attr1;
    OamBuf[ucCntObj].attr2 = attr2;
    ucCntObj += 1;
}


// ------------- Graphics data ----------------


const TObjDef APLogoObj = {
    1,
    { { ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(-8),
        ATTR1_SIZE_16 | OBJ_X(-8),
        ATTR2_PALETTE(4) | ATTR2_PRIORITY(0) | OBJ_CHAR(0x12E), },
    }
};

const TObjDef HeartObj = {
    1,
    { { ATTR0_SQUARE | ATTR0_COLOR_16 | OBJ_Y(-8),
        ATTR1_SIZE_16 | OBJ_X(-8),
        ATTR2_PALETTE(7) | ATTR2_PRIORITY(0) | OBJ_CHAR(234), },
    }
};

const TObjDef EmptyObj = {
    1,
    { { ATTR0_SQUARE | ATTR0_COLOR_16 | ATTR0_DISABLED | OBJ_Y(0),
        ATTR1_SIZE_8 | OBJ_X(0),
        ATTR2_PALETTE(0) | ATTR2_PRIORITY(0) | OBJ_CHAR(0), },
    }
};


const TAnmDef APLogoAnm[] = {
    { &APLogoObj, 0xFF, {0}},
    { NULL,       0,    {0}}
};

const TAnmDef HeartAnm[] = {
    { &HeartObj, 0xFF, {0}},
    { NULL,      0,    {0}}
};

const TAnmDef EmptyAnm[] = {
    { &EmptyObj, 0xFF, {0}},
    { NULL,      0,    {0}}
};
