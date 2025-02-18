.gba
.arm

; These extra bits of save data are stored in the upper parts of Entry Passage
; levels that don't exist.
; Most significant byte of the "third" level
.definelabel WarioAbilities, W4ItemStatus + 11
; Upper halfword of the "fourth" level
.definelabel ReceivedItemCount, W4ItemStatus + 14

; Items can be received one at a time w/o issue
.definelabel IncomingItemID, UnusedRamStart  ; byte

; 0 = Nothing
; 1 = Just received item
; 2 = Displaying text after receiving item
; 3 = Displaying text after collecting someone else's item
.definelabel MultiworldState, UnusedRamStart + 1  ; byte

.definelabel IncomingItemSender, UnusedRamStart + 2 ; 36 bytes

; The jewel piece or CD that you've most recently received or grabbed from a box
.definelabel LastCollectedItemID, UnusedRamStart + 38  ; byte
; Same values as "Has X" variables
.definelabel LastCollectedItemStatus, UnusedRamStart + 39  ; byte

.definelabel TextTimer, UnusedRamStart + 40  ; byte

; Indicates which icon is next to be selected and changed when a box is opened
; with treasure in it. Loops around four values, values of upper six bits
; unspecified and ignored.
.definelabel CurrentJewelIconPosition, UnusedRamStart + 41  ; byte

; Tracks what abilities you've found in this level but haven't properly
; collected yet.
.definelabel AbilitiesInThisLevel, UnusedRamStart + 42  ; byte

.definelabel LastCollectedItemIndex, UnusedRamStart + 43  ; byte

.definelabel LightningTrapTimer, UnusedRamStart + 44  ; byte

.definelabel gStoredMultiworldDiamonds, UnusedRamStart + 45  ; byte

.definelabel VblkStatus, UnusedRamStart + 47  ; byte

.definelabel QueuedJunk, UnusedRamStart + 48  ; bytes
    .definelabel QueuedFullHealthItem, QueuedJunk + 0
    .definelabel QueuedFormTraps, QueuedJunk + 1
    .definelabel QueuedHearts, QueuedJunk + 2
    .definelabel QueuedLightningTraps, QueuedJunk + 3

.definelabel CollectedItems, QueuedJunk + 4  ; word
.definelabel CollectedItemsCopy, QueuedJunk + 8  ; word
