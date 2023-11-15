#include <gba.h>

#include "vanilla/functions.h"
#include "vanilla/variables.h"
#include "item_table.h"
#include "graphics.h"
#include "multiworld.h"


u32 W4strlen(const u8* str);

// Load the text for the next item collection message. If no items are left to
// show, indicate start fading the results screen.
u32 LevelResults_ShowNextItem() {
    usMojiCount = 1000;  // Fixed text position (?)

    for (int i = 0; i < BOX_MAX; i++) {
        if (HAS_BOX(i) == 1 && BoxExtData[i] != NULL) {
            u32 next_tile = 0x9000;
            MojiCreate(StrItemSent, next_tile, sizeof(StrItemSent));
            next_tile += sizeof(StrItemSent) * sizeof(Tile4bpp);

            const u8* item_name = BoxExtData[i]->item_name;
            u32 length = W4strlen(item_name);
            MojiCreate(item_name, next_tile, length);
            next_tile += length * sizeof(Tile4bpp);

            MojiCreate(StrItemTo, next_tile, sizeof(StrItemTo));
            next_tile += sizeof(StrItemTo) * sizeof(Tile4bpp);

            const u8* receiver = BoxExtData[i]->receiver;
            length = W4strlen(receiver);
            MojiCreate(item_name, next_tile, length);
            next_tile += length * sizeof(Tile4bpp);

            int chars_remaining = (0xA180 - next_tile) / sizeof(Tile4bpp);
            if (chars_remaining <= 0)
                return 1;
            MojiCreate(StrScreenFiller, next_tile, chars_remaining);
        }
        HAS_BOX(i) = 3;
    }
    return 0;
}

u8 ResultsScreen_SentItemTextBox() {
    if ((usTrg_KeyPress1Frame & KEY_A) == 0)
        return 9;  // This state
    m4aSongNumStart(0x125);
    u32 has_more = LevelResults_ShowNextItem();
    if (!has_more)
        return 7;  // All done, which in this case means fade out
    return 8;  // Start showing text
}
