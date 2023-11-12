#include <gba.h>

#include "vanilla/types.h"
#include "vanilla/variables.h"
#include "item_table.h"


u8 ItemInCurrentLevel(u32 boxtype) {
    return ItemLocationTable[PassageID][InPassageLevelID][boxtype];
}

const ExtData* ExtDataInCurrentLevel(u32 boxtype) {
    return ItemExtDataTable[PassageID][InPassageLevelID][boxtype];
}
