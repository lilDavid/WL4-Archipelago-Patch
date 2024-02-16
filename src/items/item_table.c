#include <gba.h>

#include "unsorted/types.h"
#include "unsorted/variables.h"
#include "item.h"
#include "item_table.h"
#include "graphics.h"


u8 ItemInCurrentLevel(u32 boxtype) {
    return ItemLocationTable[PassageID][InPassageLevelID][boxtype];
}

const ExtData* ExtDataInCurrentLevel(u32 boxtype) {
    return ItemExtDataTable[PassageID][InPassageLevelID][boxtype];
}

static void GiveItem_Gem(u8 item_id);
static void GiveItem_CD(u8 item_id);
static void GiveItem_Ability(u8 item_id);
static void GiveItem_Junk(u8 item_id);

void GiveItem(u8 item_id, const ExtData* multiworld) {
    if (item_id == ITEM_NONE)
        return;

    // Other players' items are handled outside the game itself by reading save
    // data
    if (multiworld != NULL)
        return;

    switch (Item_GetType(item_id)) {
        case ITEMTYPE_GEM:     GiveItem_Gem(item_id); break;
        case ITEMTYPE_CD:      GiveItem_CD(item_id); break;
        case ITEMTYPE_ABILITY: GiveItem_Ability(item_id); break;
        case ITEMTYPE_JUNK:    GiveItem_Junk(item_id); break;
        default: break;
    }
}

// Loop through the passage's level statuses to find the one with this piece's
// bit set. If all four are set, do nothing because this one's an extra.
static void GiveItem_Gem(u8 item_id) {
    int passage = item_id >> 2;
    int quad = item_id & 3;
    for (int i = 0; i < LEVEL_MAXLEVEL; i++) {
        if ((W4ItemStatus[passage][i] & (1 << quad)) == 0) {
            W4ItemStatus[passage][i] |= (1 << quad);
            break;
        }
    }
}

// Set the corresponding item status bit.
static void GiveItem_CD(u8 item_id) {
    int passage = (item_id >> 2) & 7;
    int level = item_id & 3;
    W4ItemStatus[passage][level] |= ISB_CD;
}

// Ground pound and grab are progressive, so choose a high bit if you already
// have the corresponding item.
static void GiveItem_Ability(u8 item_id) {
    int ability = item_id & 7;
    if (ability == ABILITY_GROUND_POUND || ability == ABILITY_GRAB) {
        if (HAS_ABILITY_PERMANENT(ability)) {
            ability = (ability == ABILITY_GROUND_POUND) ? ABILITY_SUPER_GROUND_POUND
                                                        : ABILITY_HEAVY_GRAB;
        }
    }
    WarioAbilities |= (1 << ability);
}

static void GiveItem_Junk(u8 item_id) {
    switch (item_id) {
        case ITEM_FULL_HEALTH_ITEM:
            QueuedFullHealthItem = 1;
            break;
        case ITEM_WARIO_FORM_TRAP:
            QueuedFormTraps += 1;
            break;
        case ITEM_HEART:
            QueuedHearts += 1;
            break;
        case ITEM_LIGHTNING_TRAP:
            QueuedLightningTraps += 1;
            break;
        case ITEM_MINIGAME_COIN:
            MiniGameCoinNum += 1;
            break;
    }
}

ItemType Item_GetType(u8 item_id) {
    if ((item_id & 0b11100000) == 0b00000000) return ITEMTYPE_GEM;
    if ((item_id & 0b11100000) == 0b00100000) return ITEMTYPE_CD;
    if ((item_id & 0b11111000) == 0b01000000) return ITEMTYPE_ABILITY;
    if ((item_id & 0b11110000) == 0b10000000) return ITEMTYPE_JUNK;
    if ((item_id & 0b11111000) == 0b11110000) return ITEMTYPE_AP;
    return ITEMTYPE_NONE;
}


const u16 AbilityPaletteTable[ABILITY_MAX] = {
    /* Ground Pound */       PAL_ENTRY,
    /* Swim */               PAL_SAPPHIRE,
    /* Head Smash */         PAL_HELMET,
    /* Grab */               PAL_SAPPHIRE,
    /* Dash Attack */        PAL_GARLIC,
    /* Enemy Jump */         PAL_EMERALD,
    /* Super Ground Pound */ PAL_ENTRY,
    /* Heavy Grab */         PAL_RUBY,
};
