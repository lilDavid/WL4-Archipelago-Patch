.gba

; WRAM Variables
.arm
.definelabel PassageID, 0x3000002
.definelabel InPassageLevelID, 0x3000003
.definelabel CurrentRoomId, 0x3000024
.definelabel ucFlashLoop, 0x3000044
.definelabel ucTimeUp, 0x3000047
.definelabel CurrentEntityInfoList_TEbuf, 0x3000104
.definelabel EntityLeftOverStateList, 0x3000564
.definelabel CurrentEnemyData, 0x3000A24
.definelabel W4ItemStatus, 0x3000A68
.definelabel Scbuf_ucStatus, 0x3000BE0
.definelabel Scbuf_ucSeq, 0x3000BE3
.definelabel Scbuf_ucWork0, 0x3000BE4
.definelabel Scbuf_usPosX, 0x3000BEA
.definelabel Has1stGemPiece, 0x3000C07
.definelabel Has2ndGemPiece, 0x3000C08
.definelabel Has3rdGemPiece, 0x3000C09
.definelabel Has4thGemPiece, 0x3000C0A
.definelabel HasCD, 0x3000C0B
.definelabel HasKeyzer, 0x3000C0C
.definelabel sGameSeq, 0x3000C3C
.definelabel GlobalTimer, 0x3000C41
.definelabel OamBuf, 0x3001444
.definelabel KeyPressContinuous, 0x3001844
.definelabel KeyPressPrevious, 0x3001846
.definelabel usTrg_KeyPress1Frame, 0x3001848
.definelabel ucCntObj, 0x300184A
.definelabel Wario_ucReact, 0x3001898
.definelabel Wario_ucMiss, 0x300189C
.definelabel Wario_usMukiX, 0x30018A6
.definelabel Wario_usPosX, 0x30018AA
.definelabel Wario_usPosY, 0x30018AC
.definelabel WarioHeart, 0x3001910
.definelabel WHit, 0x3001918
.definelabel WarioLift, 0x3001940
.definelabel usWarStopFlg, 0x30019F6
.definelabel ucWarOffFlg, 0x30019F8
.definelabel ucSeldemoSeq, 0x3003C39
.definelabel usMojiCount, 0x3004A30  ; ???

; Functions
.thumb
.definelabel ChangeWarioReact_Fire, 0x801EA3C
.definelabel ChangeWarioReact_Fat, 0x801EA64
.definelabel ChangeWarioReact_Frozen, 0x801EB54
.definelabel PanelPartWork_Broken_Main, 0x806EE98

; ROM data
.arm
.definelabel WarioChng_React, 0x82DECA0
.definelabel BigBoardEntityTiles, 0x83644F0
.definelabel BigBoardEntityPalettes, 0x83B1950
.definelabel takara_Anm_00, 0x83B4BC8
.definelabel takara_Anm_01, 0x83B4BD8
.definelabel takara_Anm_02, 0x83B4C00
.definelabel takara_Anm_03, 0x83B4C10
.definelabel takara_Anm_04, 0x83B4C20
.definelabel takara_Anm_05, 0x83B4C30
.definelabel zako_takara_box_Anm_02, 0x83B4F34
.definelabel zako_takara_box_Anm_11, 0x83B5004
.definelabel CommonRoomEntityPalettes4, 0x8400A68
.definelabel EmptyCDTile, 0x8400FA8
.definelabel HasCDTile, 0x8404648
.definelabel BasicElementTiles, 0x8400AE8
.definelabel EmptyJewel4Tile, 0x84016E8
.definelabel EmptyJewel1Tile, 0x8401708
.definelabel EmptyJewel3Tile, 0x8401AE8
.definelabel EmptyJewel2Tile, 0x8401B08
.definelabel CarryingJewel1Tile, 0x8404548
.definelabel CarryingJewel2Tile, 0x8404568
.definelabel CarryingJewel3Tile, 0x8404588
.definelabel CarryingJewel4Tile, 0x84045A8
.definelabel HasJewel1Tile, 0x84045C8
.definelabel HasJewel2Tile, 0x84045E8
.definelabel HasJewel3Tile, 0x8404608
.definelabel HasJewel4Tile, 0x8404628
.definelabel PassageTreasurePalettes, 0x8414A64
.definelabel PortalTilemap3, 0x864BF58
.definelabel PortalOBJTileset, 0x868269C
.definelabel SaveTutorialTilemap, 0x868B15C
.definelabel PortalTileset2, 0x868681C
.definelabel PortalPaletteDTable, 0x868A81C
.definelabel PortalPaletteETable, 0x868A8DC
.definelabel PassageRoomTileSet, 0x869CE48
