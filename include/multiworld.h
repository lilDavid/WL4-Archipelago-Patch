#pragma once

#include <gba.h>

#include "unsorted/macros.h"


typedef enum {
    MW_IDLE,
    MW_ITEM_READY,
    MW_TEXT_RECEIVED_ITEM,
    MW_TEXT_SENDING_ITEM,
    MW_TEXT_FOUND_BOSS_ITEMS,
} MultiworldStatus;

extern u16 ReceivedItemCount;
extern u8 IncomingItemID;
extern u8 MultiworldState;
extern u8 IncomingItemSender[2 * 16 + sizeof(" ()")];
extern u8 TextTimer;
extern u8 VblkStatus;

extern const u8 SendMultiworldItemsImmediately;

extern const u8 StrItemSent[LEN("Sent ")];
extern const u8 StrItemTo[LEN(" to ")];
extern const u8 StrItemReceived[LEN("Received ")];
extern const u8 StrItemFrom[LEN("from ")];
extern const u8 StrScreenFiller[];

void ItemReceivedFeedbackSound(u8 item_id);
u8 ReceiveNextItem();
void LoadReceivedText();
