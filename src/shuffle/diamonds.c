#include "unsorted/functions.h"
#include "unsorted/macros.h"
#include "unsorted/variables.h"
#include "entity.h"
#include "item_table.h"
#include "randomizer.h"
#include "wario.h"


LONGCALL void SpriteAI_Diamond(void);
LONGCALL void Diamond_Init(void);
LONGCALL void Diamond_Main(void);


u8 DiamondIdTable[][5] = {
    // Hall of Hieroglyphs
    { PASSAGE_ENTRY,    0,  5, 0x14, DIAMOND_1 },
    { PASSAGE_ENTRY,    0,  6, 0x2A, DIAMOND_2 },
    { PASSAGE_ENTRY,    0,  9, 0x10, DIAMOND_3 },
    { PASSAGE_ENTRY,    0, 10, 0x05, DIAMOND_4 },
    { PASSAGE_ENTRY,    0, 10, 0x15, DIAMOND_5 },
    // Palm Tree Paradise
    { PASSAGE_EMERALD,  0,  1, 0x23, DIAMOND_1 },  // In unused room
    { PASSAGE_EMERALD,  0,  2, 0x34, DIAMOND_2 },
    { PASSAGE_EMERALD,  0,  3, 0x28, DIAMOND_3 },
    { PASSAGE_EMERALD,  0,  4, 0x05, DIAMOND_4 },
    { PASSAGE_EMERALD,  0,  8, 0x0A, DIAMOND_5 },
    { PASSAGE_EMERALD,  0, 10, 0x09, DIAMOND_6 },
    { PASSAGE_EMERALD,  0, 11, 0x03, DIAMOND_7 },
    // Wildflower Fields
    { PASSAGE_EMERALD,  1,  0, 0x04, DIAMOND_1 },
    { PASSAGE_EMERALD,  1,  2, 0x05, DIAMOND_2 },
    { PASSAGE_EMERALD,  1,  3, 0x12, DIAMOND_3 },
    { PASSAGE_EMERALD,  1,  4, 0x27, DIAMOND_4 },
    { PASSAGE_EMERALD,  1,  7, 0x0B, DIAMOND_5 },
    { PASSAGE_EMERALD,  1,  8, 0x2E, DIAMOND_6 },
    { PASSAGE_EMERALD,  1, 12, 0x0D, DIAMOND_7 },
    { PASSAGE_EMERALD,  1, 13, 0x04, DIAMOND_8 },
    // Mystic Lake
    { PASSAGE_EMERALD,  2,  2, 0x27, DIAMOND_1 },
    { PASSAGE_EMERALD,  2,  2, 0x34, DIAMOND_2 },
    { PASSAGE_EMERALD,  2,  3, 0x06, DIAMOND_3 },
    { PASSAGE_EMERALD,  2,  3, 0x21, DIAMOND_4 },
    { PASSAGE_EMERALD,  2,  5, 0x31, DIAMOND_5 },
    { PASSAGE_EMERALD,  2, 10, 0x0F, DIAMOND_6 },
    { PASSAGE_EMERALD,  2, 11, 0x07, DIAMOND_7 },
    // Monsoon Jungle
    { PASSAGE_EMERALD,  3,  4, 0x18, DIAMOND_1 },
    { PASSAGE_EMERALD,  3,  9, 0x06, DIAMOND_2 },
    { PASSAGE_EMERALD,  3, 11, 0x05, DIAMOND_3 },
    { PASSAGE_EMERALD,  3, 14, 0x24, DIAMOND_4 },
    { PASSAGE_EMERALD,  3, 15, 0x29, DIAMOND_5 },
    // The Curious Factory
    { PASSAGE_RUBY,     0,  1, 0x05, DIAMOND_1 },
    { PASSAGE_RUBY,     0,  7, 0x0F, DIAMOND_2 },
    { PASSAGE_RUBY,     0,  8, 0x11, DIAMOND_3 },
    { PASSAGE_RUBY,     0,  9, 0x04, DIAMOND_4 },
    { PASSAGE_RUBY,     0, 10, 0x0B, DIAMOND_5 },
    // The Toxic Landfill
    { PASSAGE_RUBY,     1,  1, 0x05, DIAMOND_1 },
    { PASSAGE_RUBY,     1,  2, 0x0B, DIAMOND_2 },
    { PASSAGE_RUBY,     1,  3, 0x37, DIAMOND_3 },
    { PASSAGE_RUBY,     1,  4, 0x10, DIAMOND_4 },
    { PASSAGE_RUBY,     1,  6, 0x05, DIAMOND_5 },
    { PASSAGE_RUBY,     1,  7, 0x04, DIAMOND_6 },
    { PASSAGE_RUBY,     1,  7, 0x06, DIAMOND_7 },
    { PASSAGE_RUBY,     1, 11, 0x13, DIAMOND_8 },
    // 40 Below Fridge
    { PASSAGE_RUBY,     2,  1, 0x04, DIAMOND_1 },
    { PASSAGE_RUBY,     2,  4, 0x2B, DIAMOND_2 },
    { PASSAGE_RUBY,     2,  4, 0x32, DIAMOND_3 },
    { PASSAGE_RUBY,     2,  5, 0x25, DIAMOND_4 },
    { PASSAGE_RUBY,     2,  6, 0x1D, DIAMOND_5 },
    { PASSAGE_RUBY,     2,  7, 0x08, DIAMOND_6 },
    { PASSAGE_RUBY,     2,  7, 0x29, DIAMOND_7 },
    { PASSAGE_RUBY,     2,  7, 0x2B, DIAMOND_8 },
    { PASSAGE_RUBY,     2,  7, 0x34, DIAMOND_9 },
    { PASSAGE_RUBY,     2,  9, 0x11, DIAMOND_10 },
    { PASSAGE_RUBY,     2, 10, 0x0C, DIAMOND_11 },
    // Pinball Zone
    { PASSAGE_RUBY,     3,  0, 0x24, DIAMOND_1 },
    { PASSAGE_RUBY,     3,  2, 0x05, DIAMOND_2 },
    { PASSAGE_RUBY,     3,  4, 0x05, DIAMOND_3 },
    { PASSAGE_RUBY,     3,  4, 0x04, DIAMOND_4 },
    { PASSAGE_RUBY,     3,  4, 0x03, DIAMOND_5 },
    // Toy Block Tower
    { PASSAGE_TOPAZ,    0,  3, 0x24, DIAMOND_1 },
    { PASSAGE_TOPAZ,    0, 11, 0x06, DIAMOND_2 },
    { PASSAGE_TOPAZ,    0, 12, 0x18, DIAMOND_3 },
    { PASSAGE_TOPAZ,    0, 14, 0x05, DIAMOND_4 },
    { PASSAGE_TOPAZ,    0, 15, 0x0F, DIAMOND_5 },
    // The Big Board
    { PASSAGE_TOPAZ,    1,  1, 0x08, DIAMOND_1 },
    { PASSAGE_TOPAZ,    1,  2, 0x2B, DIAMOND_2 },
    { PASSAGE_TOPAZ,    1,  3, 0x1B, DIAMOND_3 },
    { PASSAGE_TOPAZ,    1,  5, 0x22, DIAMOND_4 },
    { PASSAGE_TOPAZ,    1, 12, 0x12, DIAMOND_5 },
    // Doodle Woods
    { PASSAGE_TOPAZ,    2,  2, 0x5E, DIAMOND_1 },
    { PASSAGE_TOPAZ,    2,  7, 0x06, DIAMOND_2 },
    { PASSAGE_TOPAZ,    2,  9, 0x06, DIAMOND_3 },
    { PASSAGE_TOPAZ,    2, 10, 0x5B, DIAMOND_4 },
    { PASSAGE_TOPAZ,    2, 11, 0x22, DIAMOND_5 },
    // Domino Row
    { PASSAGE_TOPAZ,    3,  9, 0x0E, DIAMOND_1 },
    { PASSAGE_TOPAZ,    3, 11, 0x09, DIAMOND_2 },
    // Crescent Moon Village
    { PASSAGE_SAPPHIRE, 0,  1, 0x27, DIAMOND_1 },
    { PASSAGE_SAPPHIRE, 0,  3, 0x17, DIAMOND_2 },
    { PASSAGE_SAPPHIRE, 0,  4, 0x46, DIAMOND_3 },
    { PASSAGE_SAPPHIRE, 0,  7, 0x04, DIAMOND_4 },
    { PASSAGE_SAPPHIRE, 0,  8, 0x08, DIAMOND_5 },
    { PASSAGE_SAPPHIRE, 0, 11, 0x0C, DIAMOND_6 },
    // Arabian Night
    { PASSAGE_SAPPHIRE, 1,  2, 0x27, DIAMOND_1 },
    { PASSAGE_SAPPHIRE, 1,  3, 0x10, DIAMOND_2 },
    { PASSAGE_SAPPHIRE, 1,  5, 0x05, DIAMOND_3 },
    { PASSAGE_SAPPHIRE, 1,  7, 0x13, DIAMOND_4 },
    { PASSAGE_SAPPHIRE, 1,  8, 0x3C, DIAMOND_5 },
    { PASSAGE_SAPPHIRE, 1,  9, 0x1A, DIAMOND_6 },
    { PASSAGE_SAPPHIRE, 1,  9, 0x21, DIAMOND_7 },
    { PASSAGE_SAPPHIRE, 1,  9, 0x29, DIAMOND_8 },
    { PASSAGE_SAPPHIRE, 1, 11, 0x18, DIAMOND_9 },
    // Fiery Cavern
    { PASSAGE_SAPPHIRE, 2,  1, 0x0F, DIAMOND_1 },
    { PASSAGE_SAPPHIRE, 2,  2, 0x0F, DIAMOND_1 },
    { PASSAGE_SAPPHIRE, 2,  3, 0x0D, DIAMOND_2 },
    { PASSAGE_SAPPHIRE, 2,  4, 0x0D, DIAMOND_2 },
    { PASSAGE_SAPPHIRE, 2,  6, 0x3B, DIAMOND_3 },
    { PASSAGE_SAPPHIRE, 2,  7, 0x07, DIAMOND_4 },
    { PASSAGE_SAPPHIRE, 2, 12, 0x04, DIAMOND_5 },
    { PASSAGE_SAPPHIRE, 2, 13, 0x08, DIAMOND_6 },
    { PASSAGE_SAPPHIRE, 2, 14, 0x08, DIAMOND_7 },
    // Hotel Horror
    { PASSAGE_SAPPHIRE, 3,  4, 0x14, DIAMOND_1 },
    { PASSAGE_SAPPHIRE, 3,  8, 0x1B, DIAMOND_2 },
    { PASSAGE_SAPPHIRE, 3,  9, 0x03, DIAMOND_3 },
    { PASSAGE_SAPPHIRE, 3, 11, 0x08, DIAMOND_4 },
    { PASSAGE_SAPPHIRE, 3, 13, 0x11, DIAMOND_5 },
//  { PASSAGE_SAPPHIRE, 3, 13, 0x11, DIAMOND_6 },  // Same X position, checked specially
    // Golden Passage
    { PASSAGE_GOLDEN,   0,  0, 0x54, DIAMOND_1 },
    { PASSAGE_GOLDEN,   0,  0, 0x7D, DIAMOND_2 },
    { PASSAGE_GOLDEN,   0,  2, 0x41, DIAMOND_3 },
    { PASSAGE_GOLDEN,   0,  3, 0x1A, DIAMOND_4 },
    { PASSAGE_GOLDEN,   0,  4, 0x17, DIAMOND_5 },
    { PASSAGE_GOLDEN,   0,  4, 0x24, DIAMOND_6 },
    { PASSAGE_GOLDEN,   0,  5, 0x0F, DIAMOND_7 },
    { PASSAGE_GOLDEN,   0,  5, 0x4A, DIAMOND_8 },
    { PASSAGE_GOLDEN,   0,  6, 0x04, DIAMOND_9 },
    { PASSAGE_GOLDEN,   0,  6, 0x3E, DIAMOND_10 },
};


#define RANDO_DIAMOND_INDEX CurrentEnemyData.WorkVariable0
#define RANDO_DIAMOND_ITEM CurrentEnemyData.WorkVariable1
#define RANDO_DIAMOND_IS_MULTIWORLD CurrentEnemyData.WorkVariable2
#define DIAMOND_FLOATING_ANIMATION_FRAME CurrentEnemyData.WorkVariable3


static u8 DiamondIdentify(void) {
    int x = (CurrentEnemyData.XPos - 0x20) / 0x40;
    for (u32 i = 0; i < ARRAY_SIZE(DiamondIdTable); i++) {
        u8 *entry = DiamondIdTable[i];
        if (entry[0] == PassageID &&
            entry[1] == InPassageLevelID &&
            entry[2] == CurrentRoomId &&
            entry[3] == x)
            return entry[4];
    }
    return 0xFF;
}

static void DiamondDespawn(void) {
    CurrentEnemyData.usStatus = 0;
    EntityLeftOverStateList[CurrentRoomId][CurrentEnemyData.RoomEntitySlotId] = 2;
}

static void RandoDiamond_Init(void) {
    int index = DiamondIdentify();
    if (PassageID == PASSAGE_SAPPHIRE &&
            InPassageLevelID == 3 &&
            CurrentRoomId == 13 &&
            (CurrentEnemyData.YPos - 0x40) / 0x40 == 0x17)
        index = DIAMOND_6;


    if (index == 0xFF || ItemInCurrentLevel(index) == ITEM_NONE) {
        DiamondDespawn();
        return;
    }

    int item_id = ItemInCurrentLevel(index);
    const ExtData* multi = ExtDataInCurrentLevel(index);
    if (HasItemInLevel(index) && (multi || Item_GetType(item_id) != ITEMTYPE_JUNK)) {
        DiamondDespawn();
        return;
    }

    const TAnmDef* animation = ItemLoadInGameGraphics(index);
    if (animation == NULL)
        animation = DiamondAnm;  // TODO: Fake item graphics

    CurrentEnemyData.usStatus |= 0x408;
    ItemSetHitboxAndDrawDistance(item_id);
    CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = animation;
    CurrentEnemyData.RealFrameCountForCurrentAnimationFrame = 0;
    CurrentEnemyData.CurrentAnimationFrameId = 0;
    RANDO_DIAMOND_INDEX = index;
    RANDO_DIAMOND_ITEM = item_id;
    RANDO_DIAMOND_IS_MULTIWORLD = multi != NULL;
    DIAMOND_FLOATING_ANIMATION_FRAME = 0;
    CurrentEnemyData.CurrentAnimationId = 0x10;
    CurrentEnemyData.GuardAndDamageParam = 6;
    CurrentEnemyData.YPos -= 4;
    CurrentEnemyData.XPos += 32;

    if (RANDO_DIAMOND_ITEM != ITEM_DIAMOND) {
        CurrentEnemyData.YPos -= 48;
        CurrentEnemyData.HitboxBottom += 32;
    }
}

static void RandoDiamond_Collect(void) {
    DiamondDespawn();
    CollectItemInLevel(RANDO_DIAMOND_INDEX);
    if (RANDO_DIAMOND_IS_MULTIWORLD) {
        m4aSongNumStart(SE_GEM_GET);
    }
    if (RANDO_DIAMOND_ITEM == ITEM_WARIO_FORM_TRAP ||
        RANDO_DIAMOND_ITEM == ITEM_LIGHTNING_TRAP ||
        RANDO_DIAMOND_ITEM == ITEM_AP_TRAP) {
        if (RANDO_DIAMOND_IS_MULTIWORLD) {
            WarioVoiceSet(WV_SORRY);
        }
    } else {
        WarioVoiceSet(WV_TREASURE);
    }
}

void RandoSpriteAI_Diamond(void) {
    if (!DiamondShuffle) {
        SpriteAI_Diamond();
        return;
    }

    switch (CurrentEnemyData.CurrentAnimationId) {
        case 0: RandoDiamond_Init(); break;
        case 0x10: Diamond_Main(); break;
        case 0x31: RandoDiamond_Collect(); break;
    }
}
