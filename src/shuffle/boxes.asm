.gba

; Change the boxes' opening routines to spawn their randomized item
hook 0x8029578, 0x8029592, SpawnRandomizedItemFromBox  ; 0x00 NE jewel box
hook 0x8029758, 0x8029772, SpawnRandomizedItemFromBox  ; 0x01 SE jewel box
hook 0x8029938, 0x8029952, SpawnRandomizedItemFromBox  ; 0x02 SW jewel box
hook 0x8029B18, 0x8029B32, SpawnRandomizedItemFromBox  ; 0x03 NW jewel box
hook 0x8029D06, 0x8029D24, SpawnRandomizedItemFromBox  ; 0x04 CD box
hook 0x8029F02, 0x8029F2A, SpawnRandomizedItemFromBox  ; 0x05 Full health item box


; Make the items do what they look like they do
hook 0x8029FBA, 0x802A012, CollectRandomItem  ; 0x86 NE jewel
hook 0x802A07E, 0x802A0D6, CollectRandomItem  ; 0x87 SE jewel
hook 0x802A142, 0x802A19A, CollectRandomItem  ; 0x88 SW jewel
hook 0x802A206, 0x802A25E, CollectRandomItem  ; 0x89 NW jewel
hook 0x802A2CA, 0x802A31E, CollectRandomItem  ; 0x8A CD
hook 0x802A38A, 0x802A3C4, CollectRandomItem  ; 0x8B Full health item


; Replace the items' graphics with the thing they give when collected
hook_branch 0x8029FA8, 0x8029FB8, 0x802A022, LoadRandomItemAnimation  ; 0x86 NE jewel
hook_branch 0x802A06C, 0x802A07C, 0x802A0E6, LoadRandomItemAnimation  ; 0x87 SE jewel
hook_branch 0x802A130, 0x802A140, 0x802A1AA, LoadRandomItemAnimation  ; 0x88 SW jewel
hook_branch 0x802A1F4, 0x802A204, 0x802A26E, LoadRandomItemAnimation  ; 0x89 NW jewel
hook_branch 0x802A2B8, 0x802A2C8, 0x802A32E, LoadRandomItemAnimation  ; 0x8A CD
hook_branch 0x802A378, 0x802A388, 0x802A3E6, LoadRandomItemAnimation  ; 0x8B Full health


.autoregion :: .importobj "obj/shuffle/boxes.o" :: .endautoregion
