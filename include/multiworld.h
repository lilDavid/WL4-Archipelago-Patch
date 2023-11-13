#pragma once

#include <gba.h>


typedef enum {
    MW_IDLE,
    MW_ITEM_READY,
    MW_SHOWING_TEXT
} MultiworldStatus;

extern u16 ReceivedItemCount;
extern u8 IncomingItemID;
extern u8 MultiworldState;
extern u8 IncomingItemSender[17];
extern u8 TextTimer;

extern u8 DeathLinkEnabled;
extern const u8 DeathLinkFlag;

extern const u8 StrItemSent[sizeof("Sent ") - 1];
extern const u8 StrItemTo[sizeof(" to ") - 1];
extern const u8 StrItemReceived[sizeof("Received ") - 1];
extern const u8 StrItemFrom[sizeof("from ") - 1];

void ItemReceivedFeedbackSound(u8 item_id);
u8 ReceiveNextItem();
void LoadReceivedText();
