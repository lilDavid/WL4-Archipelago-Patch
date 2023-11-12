#include <gba.h>

#include "vanilla/variables.h"
#include "item.h"
#include "item_table.h"

void CheckLocations() {
    static const u8* has_boxes[BOX_MAX] = {
        &HasJewelPiece1, &HasJewelPiece2, &HasJewelPiece3, &HasJewelPiece4,
        &HasCD, &HasFullHealthItem, &HasFullHealthItem2
    };

    for (int i = 0; i <= BOX_CD; i++) {
        if (!*has_boxes[i])
            continue;
        W4ItemStatus[PassageID][InPassageLevelID] |= 1 << (8 + i);
        int item_id = ItemInCurrentLevel(i);
        const ExtData* multiworld_data = ExtDataInCurrentLevel(i);
        if (multiworld_data == NULL || !(item_id & ITEMBIT_JUNK))
            GiveItem(item_id, multiworld_data);
    }

    if (HasKeyzer) {
        W4ItemStatus[PassageID][InPassageLevelID] |= ISB_KEYZER;
    }

    for (int i = BOX_HEART; i < BOX_MAX; i++) {
        if (!*has_boxes[i])
            continue;
        W4ItemStatus[PassageID][InPassageLevelID] |= 1 << (8 + i + 1);
        int item_id = ItemInCurrentLevel(i);
        const ExtData* multiworld_data = ExtDataInCurrentLevel(i);
        if (multiworld_data == NULL || !(item_id & ITEMBIT_JUNK))
            GiveItem(item_id, multiworld_data);
    }
}
