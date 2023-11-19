#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item.h"


// Refill Wario's health by a specified amount
void GiveWarioHearts(u32 hearts) {
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
