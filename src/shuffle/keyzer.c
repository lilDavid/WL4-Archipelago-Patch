#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item_table.h"
#include "game_state.h"
#include "rando_sprite_util.h"
#include "sprite.h"
#include "units.h"


extern const s16 sKeyzerYSpeed[];


#define RANDO_KEYZER_ITEM gCurrentSprite.WorkVariable0
#define RANDO_KEYZER_FAKE_SPRITE gCurrentSprite.WorkVariable1
#define KEYZER_FLOATING_ANIMATION_FRAME gCurrentSprite.WorkVariable3


static void RandoKeyzer_Init(void) {
    if (ItemInCurrentLevel(KEYZER) == ITEM_NONE) {
        RandoSpriteUtil_Despawn();
        return;
    }

    int item_id = ItemInCurrentLevel(KEYZER);
    const MultiworldData* multi = MultiworldDataInCurrentLevel(KEYZER);
    if (HasItemInLevel(KEYZER) && (multi || Item_GetType(item_id) != ITEMTYPE_JUNK)) {
        RandoSpriteUtil_Despawn();
        return;
    }

    const TAnmDef* animation;
    u8 item_sprite;
    if (item_id == ITEM_WARIO_FORM_TRAP || item_id == ITEM_LIGHTNING_TRAP || item_id == ITEM_AP_TRAP) {
        item_sprite = ItemChooseFakeSprite();
        animation = ItemLoadInGameGraphicsForID(item_sprite);
    } else {
        item_sprite = item_id;
        animation = ItemLoadInGameGraphics(KEYZER);
    }
    if (animation == NULL)
        animation = KeyzerAnm;

    gCurrentSprite.usStatus |= 0x408;
    gCurrentSprite.GuardAndDamageParam = 6;
    ItemSetHitboxAndDrawDistance(item_id);
    gCurrentSprite.OAMDataPackPointerForCurrentAnimation = animation;
    gCurrentSprite.RealFrameCountForCurrentAnimationFrame = 0;
    gCurrentSprite.CurrentAnimationFrameId = 0;
    RANDO_KEYZER_ITEM = item_id;
    RANDO_KEYZER_FAKE_SPRITE = item_sprite;
    KEYZER_FLOATING_ANIMATION_FRAME = 0;
    gCurrentSprite.CurrentAnimationId = ANIMATION_WALK;
    gCurrentSprite.YPos -= BLOCK_SIZE;
    gCurrentSprite.XPos -= HALF_BLOCK_SIZE;
}

static void RandoKeyzer_Idle(void) {
    if (sKeyzerYSpeed[KEYZER_FLOATING_ANIMATION_FRAME] == 0x7FFF) {
        KEYZER_FLOATING_ANIMATION_FRAME = 0;
    } else {
        gCurrentSprite.YPos += sKeyzerYSpeed[KEYZER_FLOATING_ANIMATION_FRAME];
        KEYZER_FLOATING_ANIMATION_FRAME += 1;
    }
}

void RandoSpriteAI_Keyzer(void) {
    switch (gCurrentSprite.CurrentAnimationId) {
        case ANIMATION_INIT:
            RandoKeyzer_Init();
            break;

        case ANIMATION_Q_HIP:
            RandoSpriteUtil_CollectItem(KEYZER, RANDO_KEYZER_ITEM);
            // fallthrough
        default:
            RandoKeyzer_Idle();
            break;
    }
    if (gCurrentSprite.usStatus != 0) {
        T_NoCorrectionBgAttack();
        if (ucTHit2 & 0xF) {
            gCurrentSprite.usStatus |= 2;
        } else {
            gCurrentSprite.usStatus &= ~2;
        }
    }
}
