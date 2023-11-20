#include <gba.h>

#include "graphics.h"
#include "item.h"
#include "multiworld.h"
#include "unsorted/functions.h"


void ItemReceivedFeedbackSound(u8 item_id) {
    if (item_id & ITEMBIT_JUNK)
        return;

    if (item_id & ITEMBIT_ABILITY || item_id & ITEMBIT_CD)
        m4aSongNumStart(0x13C);
    else
        m4aSongNumStart(0x13B);
    WarioVoiceSet(1);
}

// Get the next incoming item. If nothing has been received, return ITEM_NONE.
// (ITEM_NONE in C == ItemID_None in Asm)
u8 ReceiveNextItem() {
    if (MultiworldState != MW_ITEM_READY)
        return ITEM_NONE;

    MultiworldState = MW_TEXT_RECEIVED_ITEM;
    ReceivedItemCount += 1;

    u8 incoming_item = IncomingItemID;
    if (incoming_item & ITEMBIT_JUNK)
        return incoming_item;

    LastCollectedItemID = incoming_item;
    return incoming_item;
}

// Load the text "Received from <player>" into some sprite tiles in VRAM.
void LoadReceivedText() {
    LoadSpriteString(StrItemReceived, TilesReceived8, 8);
    LoadSpriteString(StrItemFrom, TilesFrom4, 4);
    const u8* restofstr = LoadSpriteString(IncomingItemSender, TilesSenderA8, 8);
    LoadSpriteString(restofstr, TilesSenderB8, 8);
}


extern Tile4bpp* LetterToSpriteTile[256];

// Copy text sprites into the sprite table. On encountering 0xFE, copies blank
// spaces into the remaining area.
// Returns a pointer to the byte after the last one loaded. This can be passed
// to additional calls to load more characters or spaces into other tiles in
// VRAM.
const u8* LoadSpriteString(const u8* w4Str, Tile4bpp* tiles, u32 length) {
    // Override OBP3 color 2 with white
    // TODO: find usage of overridden purple color and change if needed
    SPRITE_PALETTE[3 * 16 + 0x2] = 0x7FFF;

    for (unsigned int i = 0; i < length; i++) {
        const Tile4bpp* source = LetterToSpriteTile[*w4Str];
        dmaCopy(source, tiles, sizeof(Tile4bpp));
        if (*w4Str != 0xFE)
            w4Str++;
        tiles++;
    }
    return w4Str;
}
