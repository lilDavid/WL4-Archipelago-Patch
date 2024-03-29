#include "unsorted/functions.h"
#include "unsorted/types.h"
#include "unsorted/variables.h"
#include "item.h"
#include "wario.h"


// Refill Wario's health by a specified amount
void GiveWarioHearts(u32 hearts) {
    WarioHeart[1] += hearts;
    if (WarioHeart[0] + WarioHeart[1] > 8) {
        WarioHeart[1] = 8 - WarioHeart[0];
    }
    WarioHeart[2] = WarioHeart[1] << 3;
    WarioHeart[3] = 0;
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

// Summon lightnining to strike and damage Wario, same as on The Big Board
// If underwater, apply the correct status because it's different
void GiveLightningTrap() {
    ucFlashLoop = 5;
    WarioChng_React[Wario.ucReact](Wario.ucReact == REACT_NORMAL ? 19 : 6);
    WarioCoinSet(-0x28);
}
