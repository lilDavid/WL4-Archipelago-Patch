.gba

; WRAM Variables
.arm
.definelabel PassageID, 0x3000002
.definelabel InPassageLevelID, 0x3000003
.definelabel MiniGameCoinNum, 0x3000008
.definelabel ucWorldNumBak, 0x3000010
.definelabel ucSaveFlg, 0x3000013
.definelabel CurrentRoomId, 0x3000024
.definelabel ucFlashLoop, 0x3000044
.definelabel ucTimeUp, 0x3000047
.definelabel gColorFading, 0x30000D0
.definelabel gSpriteData, 0x3000104
.definelabel SpriteLeftoverStateList, 0x3000564
.definelabel gCurrentSprite, 0x3000A24
.definelabel W4ItemStatus, 0x3000A68
.definelabel gCurrentSecondarySprite, 0x3000BE0
.definelabel iGmTotalScore, 0x3000BF4
.definelabel iGmStageScore, 0x3000BF8
.definelabel Has1stGemPiece, 0x3000C07
.definelabel Has2ndGemPiece, 0x3000C08
.definelabel Has3rdGemPiece, 0x3000C09
.definelabel Has4thGemPiece, 0x3000C0A
.definelabel HasCD, 0x3000C0B
.definelabel HasKeyzer, 0x3000C0C
.definelabel HasTreasure5, 0x3000C2C
.definelabel HasTreasure6, 0x3000C2D
.definelabel HasTreasure7, 0x3000C2E
.definelabel HasTreasure8, 0x3000C2F
.definelabel HasTreasure9, 0x3000C30
.definelabel HasTreasure10, 0x3000C31
.definelabel HasTreasure11, 0x3000C32
.definelabel HasTreasure12, 0x3000C33
.definelabel ucTakaraCount, 0x3000C34
.definelabel gPauseFlag, 0x3000C35
.definelabel GlobalGameMode, 0x3000C3A
.definelabel sGameSeq, 0x3000C3C
.definelabel GlobalTimer, 0x3000C41
.definelabel OamBuf, 0x3001444
.definelabel KeyPressContinuous, 0x3001844
.definelabel KeyPressPrevious, 0x3001846
.definelabel usTrg_KeyPress1Frame, 0x3001848
.definelabel ucCntObj, 0x300184A
.definelabel Wario, 0x3001898
.definelabel WarioHeart, 0x3001910
.definelabel WHit, 0x3001918
.definelabel WarioLift, 0x3001940
.definelabel usWarStopFlg, 0x30019F6
.definelabel ucWarOffFlg, 0x30019F8
.definelabel ucSelectVector, 0x3003C08
.definelabel ucDoraFlg, 0x3003C34
.definelabel ucSeldemoSeq, 0x3003C39
.definelabel ucSmapWarioStatus, 0x3003CA0
.definelabel usMojiCount, 0x3004A30  ; ???

.sym off

; Functions
.thumb
.definelabel ClearOamBuf, 0x8000A0C
.definelabel m4aSongNumStart, 0x8001DA4
.definelabel GmVblkIntr1_InGameUpdateWarioOAMAndSpritesTiles, 0x801BEA8
.definelabel GmWarioCreate, 0x801C5D8
.definelabel Sprite_Draw, 0x801D8C4
.definelabel Sprite_SpawnAsChild, 0x801E328
.definelabel ChangeWarioReact_Fire, 0x801EA3C
.definelabel ChangeWarioReact_Fat, 0x801EA64
.definelabel ChangeWarioReact_Frozen, 0x801EB54
.definelabel SpriteAI_Diamond, 0x802C848
.definelabel Diamond_Init, 0x802C434
.definelabel Diamond_Main, 0x802C4A8
.definelabel GmapSceneCreate, 0x806C794
.definelabel PanelPartWork_Broken_Main, 0x806EE98
.definelabel AutoSave_ExRead_Work, 0x8073A18
.definelabel SramBackup_Auto_Write, 0x8073880
.definelabel ItemGetFlgSet_LoadSavestateInfo2RAM, 0x8075B50
.definelabel GmStScoreCalc, 0x8076658
.definelabel Sprite_SpawnSecondary, 0x80766E8
.definelabel WarioCoinSet, 0x80768B8
.definelabel TKakeraComp_SE_Set, 0x8078D60
.definelabel TKakeraIconDsp_sub, 0x8078D98
.definelabel Select_Fade_Init, 0x807A428
.definelabel SelectVBlkIntr01, 0x807B758
.definelabel BossSave, 0x80813D0
.definelabel LoseSave, 0x80814DC
.definelabel EndingSave, 0x80814F0
.definelabel MmapHekigaChange, 0x808502C
.definelabel MmapBestScoreSet, 0x8085178
.definelabel WarioVoiceSet, 0x8088620
.definelabel MiniRandomCreate, 0x8089B80
.definelabel MojiCreate, 0x80904DC
.definelabel _divsi3, 0x8094E38
.definelabel _modsi3, 0x8094ED0

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
.definelabel DiamondAnm, 0x83B62AC
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
.definelabel GoldenTreasureTiles, 0x87471B0
.definelabel GoldenTreasurePalette, 0x8781750

.sym on
