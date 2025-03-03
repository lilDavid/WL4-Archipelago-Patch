#include <gba.h>

#include "item.h"
#include "item_table.h"
#include "game_state.h"
#include "graphics.h"
#include "multiworld.h"
#include "sound.h"
#include "sprite.h"
#include "units.h"
#include "wario.h"


#define BOX_INIT_DISAPPEARING ANIMATION_INIT3

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
    int box_id = gCurrentSprite.GlobalId;
    int item_index = box_id;
    if (box_id == PSPRITE_BOX_HEART) {
        item_index += GetHeartBoxID();
        gCurrentSprite.OAMDataPackPointerForCurrentAnimation = zako_takara_box_Anm_11;
    } else if (box_id == PSPRITE_BOX_CD) {
        gCurrentSprite.OAMDataPackPointerForCurrentAnimation = zako_takara_box_Anm_02;
    }

    u32 item_id = ItemInCurrentLevel(item_index);
    const MultiworldData* multi = MultiworldDataInCurrentLevel(item_index);

    if (!HasItemInLevel(item_index) || (multi == NULL && Item_GetType(item_id) == ITEMTYPE_JUNK)) {
        Sprite_SpawnAsChild(PSPRITE_TREASURE_GEM1 + box_id,
                      gCurrentSprite.RoomEntitySlotId,
                      0,
                      gCurrentSprite.YPos - 2 * BLOCK_SIZE,
                      gCurrentSprite.XPos);
        Sprite_SpawnAsChild(PSPRITE_LIGHT_GEM1 + box_id,
                      gCurrentSprite.RoomEntitySlotId,
                      0,
                      gCurrentSprite.YPos - 2 * BLOCK_SIZE,
                      gCurrentSprite.XPos);
    } else {
        gPersistentSpriteData[CurrentRoomId][gCurrentSprite.RoomEntitySlotId] = MAKE_SPRITE_DATA(SPRITE_LOADED, BOX_INIT_DISAPPEARING);
    }

    if (item_id == ITEM_LIGHTNING_TRAP || item_id == ITEM_WARIO_FORM_TRAP || item_id == ITEM_AP_TRAP) {
        if (multi) {
            WarioVoiceSet(WV_SORRY);
        } else if (item_id == ITEM_LIGHTNING_TRAP) {
            m4aSongNumStop(SE_BOX_OPEN);
        }
    } else {
        WarioVoiceSet(WV_TREASURE);
    }
}


#define RANDO_BOX_REWARD_INDEX gCurrentSprite.WorkVariable0
#define RANDO_BOX_REWARD_ITEM gCurrentSprite.WorkVariable1
#define BOX_REWARD_FLOATING_ANIMATION_FRAME gCurrentSprite.WorkVariable3


// Load the appropriate animation for a randomized item found in a box. The
// item used and resulting animation chosen is based on the item's sprite ID.
void LoadRandomItemAnimation() {
    int item_index = gCurrentSprite.GlobalId - PSPRITE_TREASURE_GEM1;
    if (gCurrentSprite.GlobalId == PSPRITE_TREASURE_HEART)
        item_index += GetHeartBoxID();
    u32 item_id = ItemInCurrentLevel(item_index);

    const TAnmDef* animation = ItemLoadInGameGraphics(item_index);
    if (animation == NULL)
        animation = EmptyAnm;
    if (item_id == ITEM_DIAMOND)
        gCurrentSprite.YPos += 3 * QUARTER_BLOCK_SIZE;

    RANDO_BOX_REWARD_INDEX = item_index;
    RANDO_BOX_REWARD_ITEM = item_id;
    if (item_id == ITEM_WARIO_FORM_TRAP || item_id == ITEM_LIGHTNING_TRAP || item_id == ITEM_AP_TRAP) {
        CollectRandomItem();
    } else {
        gCurrentSprite.OAMDataPackPointerForCurrentAnimation = animation;
        gCurrentSprite.usStatus = (gCurrentSprite.usStatus & 0xFFFB) | 0x8400;
        ItemSetHitboxAndDrawDistance(item_id);
        gCurrentSprite.GuardAndDamageParam = 6;
        gCurrentSprite.RealFrameCountForCurrentAnimationFrame = 0;
        gCurrentSprite.CurrentAnimationFrameId = 0;
        BOX_REWARD_FLOATING_ANIMATION_FRAME = 0;
        gCurrentSprite.CurrentAnimationId = ANIMATION_WALK;
    }
}


void CollectRandomItem() {
    if (RANDO_BOX_REWARD_INDEX <= BOX_CD)
        HAS_BOX(RANDO_BOX_REWARD_INDEX) = 1;

    gPersistentSpriteData[CurrentRoomId][gCurrentSprite.RoomEntitySlotId] = MAKE_SPRITE_DATA(SPRITE_LOADED, BOX_INIT_DISAPPEARING);
    gCurrentSprite.usStatus = 0;
    CollectItemInLevel(RANDO_BOX_REWARD_INDEX);

    if (MultiworldDataInCurrentLevel(RANDO_BOX_REWARD_INDEX) &&
        !(RANDO_BOX_REWARD_ITEM == ITEM_WARIO_FORM_TRAP ||
          RANDO_BOX_REWARD_ITEM == ITEM_LIGHTNING_TRAP ||
          RANDO_BOX_REWARD_ITEM == ITEM_AP_TRAP)) {
        m4aSongNumStart(SE_GEM_GET);
    }
}
