#include <gba.h>

#include "unsorted/variables.h"
#include "item_table.h"
#include "graphics.h"
#include "multiworld.h"
#include "sound.h"
#include "text.h"


static u32 GotItem(u8 index) {
    if (index > BOX_CD)
        index += 1;

    int bit = CollectedItems & (1 << index);
    CollectedItems &= ~bit;
    return bit;
}

// Load the text for the next item collection message. If no items are left to
// show, indicate start fading the results screen.
u32 LevelResults_ShowNextItem() {
    usMojiCount = TEXT_FIXED;

    for (int i = 0; i < LOCATION_MAX; i++) {
        int has_item = GotItem(i);
        const MultiworldData* multi = MultiworldDataInCurrentLevel(i);
        if (!has_item || multi == NULL)
            continue;

        int char_size = 2 * sizeof(Tile4bpp);  // Each character is 2 tiles

        u32 next_tile = 0x9000;
        MojiCreate(StrItemSent, next_tile, sizeof(StrItemSent));
        next_tile += sizeof(StrItemSent) * char_size;

        const u8* item_name = multi->item_name;
        u32 length = W4strlen(item_name);
        MojiCreate(item_name, next_tile, length);
        next_tile += length * char_size;

        MojiCreate(StrItemTo, next_tile, sizeof(StrItemTo));
        next_tile += sizeof(StrItemTo) * char_size;

        const u8* receiver = multi->receiver;
        length = W4strlen(receiver);
        MojiCreate(receiver, next_tile, length);
        next_tile += length * char_size;

        int chars_remaining = (0xA180 - next_tile) / char_size;
        if (chars_remaining <= 0)
            return 1;
        MojiCreate(StrScreenFiller, next_tile, chars_remaining);
        return 1;
    }

    return 0;
}

u8 ResultsScreen_SentItemTextBox() {
    if ((usTrg_KeyPress1Frame & KEY_A) == 0)
        return 9;  // This state
    m4aSongNumStart(SE_CONFIRM);
    u32 has_more = LevelResults_ShowNextItem();
    if (!has_more)
        return 7;  // All done, which in this case means fade out
    return 8;  // Start showing text
}
