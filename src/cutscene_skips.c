#include <gba.h>

#include "unsorted/functions.h"
#include "unsorted/variables.h"
#include "item.h"


extern u8 ucSmapWarioStatus;
extern u8 ucSelectVector[4];
extern u8 ucDoraFlg[5];

u32 CutsceneSkip_EnterLevel() {
    if (usTrg_KeyPress1Frame & KEY_A) {
        switch (ucSmapWarioStatus) {
            case 1:  // Entering
                return 1;
            case 2:  // Died or gave up
            case 3:  // Died in boss
            case 4:  // Exited without Keyzer
            case 5:  // Exited with Keyzer
                ucSelectVector[0] = 1;
                return 1;
        }
    }
    return 0;
}

u32 CutsceneSkip_ThruPortal() {
    if (ucDoraFlg[0] != 0)
        return 0;
    if (usTrg_KeyPress1Frame & KEY_A)
        return 1;
    return 0;
}

u32 CutsceneSkip_GiveUp_Clear() {
    if (usTrg_KeyPress1Frame & KEY_A)
        return 1;
    return 0;
}
