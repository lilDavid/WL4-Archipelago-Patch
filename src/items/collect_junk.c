#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item.h"


extern u8 ucFlashLoop;

#define WarioCoinSet THUMB_FUNCTION(0x80768B8, void, s32)

void GiveWarioHearts(int);
void GiveTransformTrap(void);
void GiveLightningTrap(void);

// Give and apply queued junk items given by, for example, another player in the
// multiworld.
void CollectJunkItems(void) {
    if (QueuedFullHealthItem) {
        GiveWarioHearts(8);
        QueuedFullHealthItem = 0;
    } else if (QueuedHearts > 0 && GlobalTimer % 32 == 0) {
        GiveWarioHearts(1);
        QueuedHearts -= 1;
    }

    if (Wario_ucMiss == 0 && Wario_ucReact == 0) {
        if (QueuedFormTraps > 0) {
            GiveTransformTrap();
            QueuedFormTraps -= 1;
        } else if (QueuedLightningTraps > 0) {
            GiveLightningTrap();
            QueuedLightningTraps -= 1;
        }
    }
}

// Refill Wario's health by a specified amount
void GiveWarioHearts(int hearts) {
    WarioHeart[1] += hearts;
    if (WarioHeart[0] + WarioHeart[1] > 8) {
        WarioHeart[1] = 8 - WarioHeart[0];
    }
    WarioHeart[2] = WarioHeart[1] << 3;
    WarioHeart[3] = 0;
    if (hearts == 8) {
        m4aSongNumStart(0x143);
    } else {
        m4aSongNumStart(0x140);
    }
}

void ChangeWarioReact_Fire(void);
void ChangeWarioReact_Fat(void);
void ChangeWarioReact_Frozen(void);

// Randomly transform Wario. For compatibility with all levels, only Flaming,
// Fat and Frozen Wario are allowed right now.
void GiveTransformTrap() {
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
void GiveLightningTrap() {
    ucFlashLoop = 5;
    WarioChng_React[Wario_ucReact](0x13);
    WarioCoinSet(-0x28);
}
