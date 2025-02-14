#include "unsorted/variables.h"
#include "game_state.h"
#include "item.h"
#include "sound.h"
#include "units.h"
#include "wario.h"


// Refill Wario's health by a specified amount
void GiveWarioHearts(u32 hearts) {
    WarioHeart.queuedFill += hearts;
    if (WarioHeart.count + WarioHeart.queuedFill > 8) {
        WarioHeart.queuedFill = 8 - WarioHeart.count;
    }
    WarioHeart.unk_02 = WarioHeart.queuedFill << 3;
    WarioHeart.timer = 0;
    if (hearts == 8) {
        m4aSongNumStart(SE_FULL_HEALTH_ITEM_GET);
    } else {
        m4aSongNumStart(SE_HEART_GET);
    }
}

void ChangeWarioReact_Fire(void);
void ChangeWarioReact_Fat(void);
void ChangeWarioReact_Frozen(void);

// If Wario is on land, randomly transform him. If the's swimming, bubble him.
// For compatibility with all levels, only Flaming, Fat, Frozen, and Bubble
// Wario are allowed right now.
void GiveTransformTrap() {
    if (Wario.ucReact == REACT_WATER) {
        WarioChng_React[Wario.ucReact](14);
        return;
    }

    typedef void (*ChangeWarioReactFunc)(void);
    static ChangeWarioReactFunc reactionList[] = {
        ChangeWarioReact_Fire,
        ChangeWarioReact_Fat,
        ChangeWarioReact_Frozen
    };

    int random = MiniRandomCreate();
    random = _modsi3(random, 3);
    reactionList[random]();
}

// Give lightning damage to Wario, flashing the screen and making him drop 400 coins
void ApplyLightningTrap() {
    ucFlashLoop = 5;
    // If underwater, apply the correct status because it's different
    WarioChng_React[Wario.ucReact](Wario.ucReact == REACT_NORMAL ? 19 : 6);
    WarioCoinSet(CONVERT_SCORE(-400));
}

// Summon lightnining to strike and damage Wario, same as on The Big Board
void GiveLightningTrap() {
    m4aSongNumStart(SE_THUNDER);
    LightningTrapTimer = CONVERT_SECONDS(0.75);
}
