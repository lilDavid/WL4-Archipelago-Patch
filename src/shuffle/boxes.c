#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "entity.h"
#include "item.h"
#include "item_table.h"
#include "graphics.h"
#include "multiworld.h"
#include "wario.h"


void CollectRandomItem(void);


// Identify a heart box based on what room the player is in. Returns 1 if the
// player is in Pinball Zone but not in the pink room with the heart box, and 0
// otherwise.
static u32 GetHeartBoxID() {
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
    int item_index = box_id;
    if (box_id == ENTITY_BOX_HEART) {
        item_index += GetHeartBoxID();
        CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = zako_takara_box_Anm_11;
    } else if (box_id == ENTITY_BOX_CD) {
        CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = zako_takara_box_Anm_02;
    }

    u32 item_id = ItemInCurrentLevel(item_index);
    const ExtData* multi = ExtDataInCurrentLevel(item_index);

    if (!HasItemInLevel(item_index) || (multi == NULL && Item_GetType(item_id) == ITEMTYPE_JUNK)) {
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
        EntityLeftOverStateList[CurrentRoomId][CurrentEnemyData.RoomEntitySlotId] = 0x21;
    }

    if (item_id == ITEM_LIGHTNING_TRAP || item_id == ITEM_WARIO_FORM_TRAP)
        WarioVoiceSet(WV_HURT);
    else
        WarioVoiceSet(WV_TREASURE);
}


// Load the appropriate animation for a randomized item found in a box. The
// item used and resulting animation chosen is based on the item's entity ID.
void LoadRandomItemAnimation() {
    int item_index = CurrentEnemyData.GlobalId - ENTITY_TREASURE_GEM1;
    if (CurrentEnemyData.GlobalId == ENTITY_TREASURE_HEART)
        item_index += GetHeartBoxID();
    u32 item_id = ItemInCurrentLevel(item_index);

    const TAnmDef* animation = ItemLoadInGameGraphics(item_index);
    if (animation == NULL)
        animation = EmptyAnm;
    int give_immediately = item_id == ITEM_WARIO_FORM_TRAP || item_id == ITEM_LIGHTNING_TRAP;
    if (item_id == ITEM_DIAMOND)
        CurrentEnemyData.YPos += 48;

    CurrentEnemyData.OAMDataPackPointerForCurrentAnimation = animation;
    EntityAI_INITIAL_takara_kakera();
    if (give_immediately)
        CollectRandomItem();
}


void CollectRandomItem() {
    int item_index = CurrentEnemyData.GlobalId - ENTITY_TREASURE_GEM1;
    if (CurrentEnemyData.GlobalId == ENTITY_TREASURE_HEART)
        item_index += GetHeartBoxID();
    if (item_index <= BOX_CD)
        HAS_BOX(item_index) = 1;

    EntityLeftOverStateList[CurrentRoomId][CurrentEnemyData.RoomEntitySlotId] = 0x21;
    CollectItemInLevel(item_index);
}
