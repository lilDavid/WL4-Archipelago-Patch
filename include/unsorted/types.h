#pragma once

typedef enum {
    PASSAGE_ENTRY = 0,
    PASSAGE_EMERALD = 1,
    PASSAGE_RUBY = 2,
    PASSAGE_TOPAZ = 3,
    PASSAGE_SAPPHIRE = 4,
    PASSAGE_GOLDEN = 5,
    PASSAGE_MAX = 6
} Passage;

typedef enum {
    LEVEL_1 = 0,
    LEVEL_2 = 1,
    LEVEL_3 = 2,
    LEVEL_4 = 3,
    LEVEL_MAXLEVEL = 4,  // Number of "proper" levels per passage
    LEVEL_BOSS = 4,
    LEVEL_MINIGAME_SHOP = 5,
    LEVEL_MAX = 6,  // Number of "levels" where Wario could be in a passage
} InPassageLevel;

typedef enum {
    SE_CONFIRM = 0x125,
    SE_GEM_GET = 0x13B,
    SE_CD_GET = 0x13C,
    SE_HEART_GET = 0x140,
    SE_FULL_HEALTH_ITEM_GET = 0x143,
    SE_RESULTS_FOUND_CD = 0x1D0,
    SE_MINIGAME_COIN_GET = 0x223,
    SE_HIGH_SCORE = 0x252,
} m4aSongNum;

typedef enum {
    WV_TREASURE = 1,
    WV_HURT = 4,
} WarioVoiceID;

typedef enum {
    REACT_NORMAL,
    REACT_WATER,
    REACT_FIRE,
    REACT_FAT,
    REACT_FROZEN,
    REACT_ZOMBIE,
    REACT_SNOWMAN,
    REACT_BOUNCY,
    REACT_PUFFY,
    REACT_BAT,
    REACT_FLAT,
    REACT_MASK,
    REACT_MAX
} WarioReact;
