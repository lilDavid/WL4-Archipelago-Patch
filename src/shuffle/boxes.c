#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "entity.h"
#include "item.h"
#include "item_table.h"
#include "graphics.h"
#include "multiworld.h"


void CollectRandomItem(void);


// Identify a heart box based on what room the player is in. Returns 1 if the
// player is in Pinball Zone but not in the pink room with the heart box, and 0
// otherwise.
u32 GetHeartBoxID() {
    return PassageID == PASSAGE_RUBY &&
           InPassageLevelID == LEVEL_4 &&
           CurrentRoomId != 8;
}


extern TAnmDef zako_takara_box_Anm_02[];
extern TAnmDef zako_takara_box_Anm_11[];

// Check if this box has been opened before and release the item if it has.
// Unless it contains a junk item, in which case always open it.
void SpawnRandomizedItemFromBox() {
    int box_id = CurrentEnemyData.GlobalId;
    int box_contents = box_id;
    if (box_id == ENTITY_BOX_HEART) {
        box_contents += GetHeartBoxID();
        CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = zako_takara_box_Anm_11;
    } else if (box_id == ENTITY_BOX_CD) {
        CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = zako_takara_box_Anm_02;
    }

    u32 item_id = ItemInCurrentLevel(box_id);
    BoxContents[box_contents] = item_id;
    const ExtData* multi = ExtDataInCurrentLevel(box_contents);
    BoxExtData[box_contents] = multi;

    if (!HAS_BOX(box_contents) || (multi == NULL && (item_id & ITEMBIT_JUNK))) {
        EnemyChildSet(ENTITY_TREASURE_GEM1 + box_id,
                      CurrentEnemyData.RoomEntitySlotId,
                      0,
                      CurrentEnemyData.YPos - 0x80,
                      CurrentEnemyData.XPos);
        EnemyChildSet(ENTITY_LIGHT_GEM1 + box_id,
                      CurrentEnemyData.RoomEntitySlotId,
                      0,
                      CurrentEnemyData.YPos - 0x80,
                      CurrentEnemyData.XPos);
    } else {
        EntityLeftOverStateList[CurrentRoomId].data[CurrentEnemyData.RoomEntitySlotId] = 0x21;
    }

    if (item_id == ITEM_LIGHTNING_TRAP || item_id == ITEM_WARIO_FORM_TRAP)
        WarioVoiceSet(WV_HURT);
    else
        WarioVoiceSet(WV_TREASURE);
}


extern u8 CurrentJewelIconPosition;

extern TAnmDef takara_Anm_00[];  // CD
extern TAnmDef takara_Anm_01[];  // Full health item
extern TAnmDef takara_Anm_02[];  // NW gem piece
extern TAnmDef takara_Anm_03[];  // SW gem piece
extern TAnmDef takara_Anm_04[];  // NE jewel piece
extern TAnmDef takara_Anm_05[];  // SE jewel piece
extern TAnmDef APLogoAnm[];
extern TAnmDef HeartAnm[];
extern TAnmDef EmptyAnm[];

static TAnmDef* gem_animations[] = {
    takara_Anm_02, takara_Anm_03, takara_Anm_04, takara_Anm_05,
};

static TAnmDef* RandomItemTilesCreate(const Tile4bpp* top, const Tile4bpp* bottom) {
    int position = CurrentJewelIconPosition % 4;
    CurrentJewelIconPosition += 1;

    Tile4bpp* destination = (Tile4bpp*) (0x6011C40 + 2 * sizeof(Tile4bpp) * position);
    dmaCopy(top, destination, 2 * sizeof(Tile4bpp));
    destination += TILE_OFFSET(0, 1);
    dmaCopy(bottom, destination, 2 * sizeof(Tile4bpp));

    return gem_animations[position];
}

// Load the appropriate animation for a randomized item found in a box. The
// item used and resulting animation chosen is based on the item's entity ID.
void LoadRandomItemAnimation() {
    int box_type = CurrentEnemyData.GlobalId - ENTITY_TREASURE_GEM1;
    if (CurrentEnemyData.GlobalId == ENTITY_TREASURE_HEART) {
        box_type += GetHeartBoxID();
    }
    int item_id = BoxContents[box_type];

    TAnmDef* animation;
    int give_immediately = 0;

    if (item_id == ITEM_ARCHIPELAGO_ITEM) {
        SetTreasurePalette(PAL_AP);
        animation = APLogoAnm;

    } else if (item_id & ITEMBIT_JUNK) {
        switch (item_id) {
            case ITEM_FULL_HEALTH_ITEM:
                animation = takara_Anm_01;
                break;
            case ITEM_HEART:
                animation = HeartAnm;
                break;
            case ITEM_MINIGAME_COIN:
                animation = RandomItemTilesCreate(MinigameCoinTiles, MinigameCoinTiles + 2);
                SetTreasurePalette(PAL_MINGAME_COIN);
                break;
            default:
                animation = EmptyAnm;
                give_immediately = 1;
                break;
        }

    } else if (item_id & ITEMBIT_ABILITY || !(item_id & ITEMBIT_CD)) {
        const Tile4bpp* top_row_source;
        const Tile4bpp* bottom_row_source;
        if (item_id & ITEMBIT_ABILITY) {
            int ability = item_id & 7;
            if (ability == ABILITY_GROUND_POUND && HAS_ABILITY_TEMPORARY(ABILITY_GROUND_POUND)) {
                ability = ABILITY_SUPER_GROUND_POUND;
            } else if (ability == ABILITY_GRAB && HAS_ABILITY_TEMPORARY(ABILITY_GRAB)) {
                ability = ABILITY_HEAVY_GRAB;
            }
            SetTreasurePalette(AbilityPaletteTable[ability]);
            top_row_source = AbilityIconTilesTop + 2 * ability;
            bottom_row_source = AbilityIconTilesBottom + 2 * ability;
        } else {
            SetTreasurePalette((item_id >> 2) & 7);
            // HasGemPiece(n) does not agree with the tile order here
            // I could've done this with a table or if-else, but noooo, I haaad
            // to do it with math instead
            // 0 -> 2; 1 -> 3; 2 -> 1; 3 -> 0
            int piece = item_id & 3;
            int position = piece ^ ((piece >> 1) | 2);
            top_row_source = BasicElementTiles + TILE_OFFSET(2 * position + 2, 3);
            bottom_row_source = top_row_source + TILE_OFFSET(0, 1);
        }

        animation = RandomItemTilesCreate(top_row_source, bottom_row_source);
    } else {  // CD
        SetTreasurePalette((item_id >> 2) & 7);
        animation = takara_Anm_00;
    }

    CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = animation;
    EntityAI_INITIAL_takara_kakera();
    if (give_immediately)
        CollectRandomItem();
}


// Collect this item. If it's your own junk item, it gets immediately given.
// Otherwise, this marks the item as collected so that it can be properly given
// to you or another player when you escape the level.
void CollectRandomItem() {
    int box_type = CurrentEnemyData.GlobalId - ENTITY_TREASURE_GEM1;
    if (CurrentEnemyData.GlobalId == ENTITY_TREASURE_HEART) {
        box_type += GetHeartBoxID();
    }
    HAS_BOX(box_type) = 1;

    EntityLeftOverStateList[CurrentRoomId].data[CurrentEnemyData.RoomEntitySlotId] = 0x21;
    int item_id = BoxContents[box_type];
    const ExtData* multi = BoxExtData[box_type];

    if (multi != NULL) {
        const u8* namebytes = multi->item_name;
        namebytes = LoadSpriteString(namebytes, (Tile4bpp*) 0x06012180, 12);
        namebytes = LoadSpriteString(namebytes, (Tile4bpp*) 0x06012600, 8);
        LoadSpriteString(namebytes, (Tile4bpp*) 0x06012A00, 8);

        MultiworldState = MW_TEXT_SENDING_ITEM;
        TextTimer = 120;
        m4aSongNumStart(SE_GEM_GET);
        return;
    }

    if (item_id & ITEMBIT_JUNK) {
        switch (item_id) {
            case ITEM_FULL_HEALTH_ITEM: GiveWarioHearts(8); break;
            case ITEM_WARIO_FORM_TRAP: GiveTransformTrap(); break;
            case ITEM_HEART: GiveWarioHearts(1); break;
            case ITEM_LIGHTNING_TRAP: GiveLightningTrap(); break;
            case ITEM_MINIGAME_COIN: m4aSongNumStart(SE_MINIGAME_COIN_GET); break;
        }
        return;
    }

    LastCollectedItemID = item_id;

    if (item_id & ITEMBIT_ABILITY) {
        int new_ability = item_id & 7;
        if (item_id == ITEM_GROUND_POUND || item_id == ITEM_GRAB) {
            if (HAS_ABILITY_TEMPORARY(new_ability)) {
                if (item_id == ITEM_GROUND_POUND)
                    new_ability = ABILITY_SUPER_GROUND_POUND;
                else
                    new_ability = ABILITY_HEAVY_GRAB;
            }
            SpawnCollectionIndicator(0, 0);
        } else {
            SpawnCollectionIndicator(1, 0);
        }
        AbilitiesInThisLevel |= 1 << new_ability;
        m4aSongNumStart(SE_CD_GET);
        return;
    }

    if (item_id & ITEMBIT_CD) {
        SpawnCollectionIndicator(1, 0);
        m4aSongNumStart(SE_CD_GET);
    } else /* Gem piece */ {
        SpawnCollectionIndicator(0, 0);
        m4aSongNumStart(SE_GEM_GET);
    }
}
