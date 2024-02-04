#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item.h"


extern u8 ucSmapWarioStatus;

u32 CutsceneSkip_EnterLevel() {
    if (usTrg_KeyPress1Frame & KEY_A) {
        switch (ucSmapWarioStatus) {
            case 1:  // Entering
                sGameSeq = 0x0A;
                return 1;
            case 2:  // Died or gave up
            case 3:  // Died in boss
            case 4:  // Exited without Keyzer
                sGameSeq = 0x27;
                return 1;
            case 5:  // Exited with Keyzer
                sGameSeq = 0x25;
                return 1;
        }
    }
    return 0;
}

u32 CutsceneSkip_ThruPortal() {
    if (usTrg_KeyPress1Frame & KEY_A) {
        sGameSeq = 0x13;
        return 1;
    }
    return 0;
}

u32 CutsceneSkip_GiveUp() {
    if (usTrg_KeyPress1Frame & KEY_A) {
        sGameSeq = 0x18;
        return 1;
    }
    return 0;
}

u32 CutsceneSkip_Clear() {
    if (usTrg_KeyPress1Frame & KEY_A) {
        sGameSeq = 0x20;
        return 1;
    }
    return 0;
}
