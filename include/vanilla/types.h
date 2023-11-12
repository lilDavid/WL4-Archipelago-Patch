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
