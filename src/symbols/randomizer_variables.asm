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

; Extends the existing box system to include full health boxes
.definelabel HasFullHealthItem, UnusedRamStart + 43  ; byte

; Pinball Zone has an extra one
.definelabel HasFullHealthItem2, UnusedRamStart + 44  ; byte

.definelabel LastCollectedBox, UnusedRamStart + 45  ; byte

.definelabel VblkStatus, UnusedRamStart + 46  ; byte

.definelabel QueuedJunk, UnusedRamStart + 48  ; bytes
    .definelabel QueuedFullHealthItem, QueuedJunk + 0
    .definelabel QueuedFormTraps, QueuedJunk + 1
    .definelabel QueuedHearts, QueuedJunk + 2
    .definelabel QueuedLightningTraps, QueuedJunk + 3

.definelabel BoxContents, QueuedJunk + 4  ; bytes
    .definelabel Jewel1BoxContents, BoxContents  ; bytes
    .definelabel Jewel2BoxContents, BoxContents + 1
    .definelabel Jewel3BoxContents, BoxContents + 2
    .definelabel Jewel4BoxContents, BoxContents + 3
    .definelabel CDBoxContents, BoxContents + 4
    .definelabel HealthBoxContents, BoxContents + 5
    .definelabel HealthBox2Contents, BoxContents + 6

.definelabel BoxExtData, Jewel1BoxContents + 8  ; words
    .definelabel Jewel1BoxExtData, BoxExtData
    .definelabel Jewel2BoxExtData, BoxExtData + 4
    .definelabel Jewel3BoxExtData, BoxExtData + 8
    .definelabel Jewel4BoxExtData, BoxExtData + 12
    .definelabel CDBoxExtData, BoxExtData + 16
    .definelabel HealthBoxExtData, BoxExtData + 20
    .definelabel HealthBox2ExtData, BoxExtData + 24
