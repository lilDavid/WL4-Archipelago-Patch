#include "rando_sprite_util.h"

#include "game_state.h"
#include "item_table.h"
#include "sound.h"
#include "sprite.h"
#include "wario.h"


void RandoSpriteUtil_Despawn(void) {
    gCurrentSprite.usStatus = 0;
    gPersistentSpriteData[CurrentRoomId][gCurrentSprite.RoomEntitySlotId] = SPRITE_DESPAWNED;
}

void RandoSpriteUtil_CollectItem(u8 index, u8 item_id) {
    RandoSpriteUtil_Despawn();
    CollectItemInLevel(index);
    const MultiworldData* multi = MultiworldDataInCurrentLevel(index);
    if (multi) {
        m4aSongNumStart(SE_GEM_GET);
    }
    if (item_id == ITEM_WARIO_FORM_TRAP ||
        item_id == ITEM_LIGHTNING_TRAP ||
        item_id == ITEM_AP_TRAP) {
        if (multi) {
            WarioVoiceSet(WV_SORRY);
        }
    } else if (Item_GetType(item_id) == ITEMTYPE_KEYZER && !multi) {
        WarioVoiceSet(WV_KEYZER);
    } else if (item_id != ITEM_HEART && item_id != ITEM_AP_FILLER) {
        WarioVoiceSet(WV_TREASURE);
    }
}
