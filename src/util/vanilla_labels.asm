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
.definelabel WarioHeart, 0x3001910
.definelabel WHit, 0x3001918
.definelabel WarioLift, 0x3001940
.definelabel usWarStopFlg, 0x30019F6
.definelabel ucSeldemoSeq, 0x3003C39
.definelabel usMojiCount, 0x3004A30  ; ???

; I/O Registers
.definelabel REG_BG3CNT, 0x400000E
.definelabel REG_BG0VOFS, 0x4000012
.definelabel REG_DMA3SAD, 0x40000D4

; PalRAM
.definelabel ObjectPalette4, 0x5000280
.definelabel ObjectPalette9, 0x5000320
.definelabel ObjectPaletteF, 0x50003E0

; Functions
.thumb
.definelabel MainGameLoop, 0x80001CC
.definelabel ClearOamBuf, 0x8000A0C
.definelabel m4aSongNumStart, 0x8001DA4
.definelabel WarioHitMain, 0x801009c
.definelabel GmWarioCreate, 0x801C5D8
.definelabel WarChng_Flying, 0x8012CC8
.definelabel EnemyDisplayMain, 0x801D8C4
.definelabel EnemyChildSet, 0x801E328
.definelabel ChangeWarioReact_Fire, 0x801EA3C
.definelabel ChangeWarioReact_Fat, 0x801EA64
.definelabel ChangeWarioReact_Puffy, 0x801EADC
.definelabel ChangeWarioReact_Spring, 0x801EB04
.definelabel ChangeWarioReact_Frozen, 0x801EB54
.definelabel KillChildWhenParentDies, 0x80268DC
.definelabel EntityAI_Q_K5_Hip_COM_takarabako, 0x80292BC
.definelabel EntityAI_INITIAL_takara_kakera, 0x802932C
.definelabel GmapSceneCreate, 0x806C794
.definelabel PanelPartWork_Broken_Main, 0x806EE98
.definelabel AutoSave_EXRead_Work, 0x8073A18
.definelabel TOptObjSet, 0x80766E8
.definelabel WarioCoinSet, 0x80768B8
.definelabel TKakeraComp_SE_Set, 0x8078D60
.definelabel TKakeraIconDsp_Sub, 0x8078D98
.definelabel Select_Fade_Init, 0x807A428
.definelabel MmapHekigaChange, 0x808502C
.definelabel MmapBestScoreSet, 0x8085178
.definelabel WarioVoiceSet, 0x8088620
.definelabel MiniRandomCreate, 0x8089B80
.definelabel MojiCreate, 0x80904DC
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
