#include "unsorted/functions.h"
#include "unsorted/macros.h"
#include "game_state.h"
#include "item.h"
#include "randomizer.h"
#include "text.h"


extern const u8 sRandoClearText[32];

void Stats_CountInGameTime(void) {
    if (gInGameTime >= MAX_IN_GAME_TIME)
        return;
    if (W4ItemStatus[PASSAGE_GOLDEN][LEVEL_BOSS] & ISB_GOLDEN_DIVA)
        return;
    gInGameTime++;
}

static u32 Stats_CountBits(u32 bitfield) {
    u32 bits = 0;
    while (bitfield > 0) {
        bits += bitfield & 1;
        bitfield >>= 1;
    }
    return bits;
}

u32 Stats_GetLocationsChecked(void) {
    u32 locations = 0;

    locations += Stats_CountBits(W4ItemStatus[PASSAGE_ENTRY][0] >> 8);
    locations += Stats_CountBits(W4ItemStatus[PASSAGE_ENTRY][LEVEL_BOSS] >> 8);

    for (int passage = PASSAGE_EMERALD; passage < PASSAGE_GOLDEN; passage++)
        for (int level = 0; level <= LEVEL_BOSS; level++)
            locations += Stats_CountBits(W4ItemStatus[passage][level] >> 8);

    locations += Stats_CountBits(W4ItemStatus[PASSAGE_GOLDEN][0] >> 8);
    locations += Stats_CountBits(W4ItemStatus[PASSAGE_GOLDEN][LEVEL_BOSS] >> 8);

    return locations;
}

void Stats_LoadClearTimeAndCollectionRate(void) {
    u32 inGameTime = gInGameTime;
    if (inGameTime > MAX_IN_GAME_TIME)
        inGameTime = MAX_IN_GAME_TIME;

    u8 text[32];
    for (u32 i = 0; i < ARRAY_SIZE(text); i++)
        text[i] = sRandoClearText[i];

    // frames
    inGameTime = _divsi3(inGameTime, 60);
    // seconds
    text[31] = _modsi3(inGameTime, 10);
    inGameTime = _divsi3(inGameTime, 10);
    text[30] = _modsi3(inGameTime, 6);
    inGameTime = _divsi3(inGameTime, 6);
    // minutes
    text[29] = _modsi3(inGameTime, 10);
    inGameTime = _divsi3(inGameTime, 10);
    text[28] = _modsi3(inGameTime, 6);
    inGameTime = _divsi3(inGameTime, 6);
    // hours
    text[27] = _modsi3(inGameTime, 10);
    inGameTime = _divsi3(inGameTime, 10);
    if (inGameTime > 0)
        text[26] = inGameTime;

    u32 collectionRate = Stats_GetLocationsChecked();
    u32 totalLocations = sLocationCount;
    text[18] = _modsi3(collectionRate, 10);
    for (int i = 0; i < 2; i++) {
        collectionRate = _divsi3(collectionRate, 10);
        if (collectionRate == 0)
            break;
        text[17 - i] = _modsi3(collectionRate, 10);
    }
    for (int i = 0; i < 3; i++) {
        text[21 - i] = _modsi3(totalLocations, 10);
        totalLocations = _divsi3(totalLocations, 10);
    }

    usMojiCount = TEXT_FIXED;
    MojiCreate(text, 0x5C00, ARRAY_SIZE(text));
    BG_PALETTE[16 * 6 + 1] = 0x14A5;
    BG_PALETTE[16 * 6 + 5] = 0x14A5;

    dmaCopy(sRandoSlashTiles, (void*) 0x6005F80, 2 * 32);
}
