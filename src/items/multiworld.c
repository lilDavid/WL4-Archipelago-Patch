#include <gba.h>

#include "graphics.h"
#include "item.h"
#include "multiworld.h"
#include "vanilla/functions.h"


void ItemReceivedFeedbackSound(u8 item_id) {
    if (item_id & ITEMBIT_JUNK)
        return;

    if (item_id & ITEMBIT_ABILITY || item_id & ITEMBIT_CD)
        m4aSongNumStart(0x13C);
    else
        m4aSongNumStart(0x13B);
}

// Get the next incoming item. If nothing has been received, return ITEM_NONE.
// (ITEM_NONE in C == ItemID_None in Asm)
u8 ReceiveNextItem() {
    if (MultiworldState != MW_ITEM_READY)
        return ITEM_NONE;

    MultiworldState = MW_SHOWING_TEXT;
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
    u8* restofstr = LoadSpriteString(IncomingItemSender, TilesSenderA8, 8);
    LoadSpriteString(restofstr, TilesSenderB8, 8);
}
