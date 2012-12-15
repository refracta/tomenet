/* $Id$ */
/* File: tables.c */

/* Purpose: Angband Tables shared by both client and server.
 * originally added for runecraft. - C. Blue */

#include "angband.h"



/* Spell failure rate calculations ----------------------------------------- */

/*
 * Stat Table (INT/WIS) -- Minimum failure rate (percentage)
 */
byte adj_mag_fail[] =
{
        99      /* 3 */,
        99      /* 4 */,
        99      /* 5 */,
        99      /* 6 */,
        99      /* 7 */,
        50      /* 8 */,
        30      /* 9 */,
        20      /* 10 */,
        15      /* 11 */,
        12      /* 12 */,
        11      /* 13 */,
        10      /* 14 */,
        9       /* 15 */,
        8       /* 16 */,
        7       /* 17 */,
        6       /* 18/00-18/09 */,
        6       /* 18/10-18/19 */,
        5       /* 18/20-18/29 */,
        5       /* 18/30-18/39 */,
        5       /* 18/40-18/49 */,
        4       /* 18/50-18/59 */,
        4       /* 18/60-18/69 */,
        4       /* 18/70-18/79 */,
        4       /* 18/80-18/89 */,
        3       /* 18/90-18/99 */,
        3       /* 18/100-18/109 */,
        2       /* 18/110-18/119 */,
        2       /* 18/120-18/129 */,
        2       /* 18/130-18/139 */,
        2       /* 18/140-18/149 */,
        1       /* 18/150-18/159 */,
        1       /* 18/160-18/169 */,
        1       /* 18/170-18/179 */,
        1       /* 18/180-18/189 */,
        1       /* 18/190-18/199 */,
        0       /* 18/200-18/209 */,
        0       /* 18/210-18/219 */,
        0       /* 18/220+ */
};

/*
 * Stat Table (INT/WIS) -- Various things
 */
byte adj_mag_stat[] =
{
        0       /* 3 */,
        0       /* 4 */,
        0       /* 5 */,
        0       /* 6 */,
        0       /* 7 */,
        1       /* 8 */,
        1       /* 9 */,
        1       /* 10 */,
        1       /* 11 */,
        1       /* 12 */,
        1       /* 13 */,
        1       /* 14 */,
        2       /* 15 */,
        2       /* 16 */,
        2       /* 17 */,
        3       /* 18/00-18/09 */,
        3       /* 18/10-18/19 */,
        3       /* 18/20-18/29 */,
        3       /* 18/30-18/39 */,
        3       /* 18/40-18/49 */,
        4       /* 18/50-18/59 */,
        4       /* 18/60-18/69 */,
        5       /* 18/70-18/79 */,
        6       /* 18/80-18/89 */,
        7       /* 18/90-18/99 */,
        8       /* 18/100-18/109 */,
        9       /* 18/110-18/119 */,
        10      /* 18/120-18/129 */,
        11      /* 18/130-18/139 */,
        12      /* 18/140-18/149 */,
        13      /* 18/150-18/159 */,
        14      /* 18/160-18/169 */,
        15      /* 18/170-18/179 */,
        16      /* 18/180-18/189 */,
        17      /* 18/190-18/199 */,
        18      /* 18/200-18/209 */,
        19      /* 18/210-18/219 */,
        20      /* 18/220+ */
};

/*
 * Stat Table (INT) for mimicry powers fail rate modifier (%)
 */
byte adj_int_pow[] =
{
        250     /* 3 */,
        210     /* 4 */,
        190     /* 5 */,
        160     /* 6 */,
        140     /* 7 */,
        120     /* 8 */,
        110     /* 9 */,
        100     /* 10 */,
        100     /* 11 */,
        95      /* 12 */,
        90      /* 13 */,
        85      /* 14 */,
        80      /* 15 */,
        75      /* 16 */,
        70      /* 17 */,
        65      /* 18/00-18/09 */,
        61      /* 18/10-18/19 */,
        57      /* 18/20-18/29 */,
        53      /* 18/30-18/39 */,
        49      /* 18/40-18/49 */,
        45      /* 18/50-18/59 */,
        41      /* 18/60-18/69 */,
        38      /* 18/70-18/79 */,
        35      /* 18/80-18/89 */,
        32      /* 18/90-18/99 */,
        29      /* 18/100-18/109 */,
        27      /* 18/110-18/119 */,
        25      /* 18/120-18/129 */,
        23      /* 18/130-18/139 */,
        21      /* 18/140-18/149 */,
        19      /* 18/150-18/159 */,
        17      /* 18/160-18/169 */,
        15      /* 18/170-18/179 */,
        14      /* 18/180-18/189 */,
        13      /* 18/190-18/199 */,
        12      /* 18/200-18/209 */,
        11      /* 18/210-18/219 */,
        10      /* 18/220+ */
};



/* Mimicry powers ---------------------------------------------------------- */

/*
 * Mimic 'spells' -- they should be integrated to the realm spells
 * (slevel, smana, sfail, sexp)
 */
magic_type innate_powers[96] =
{
/* 0, mana, fail, 0 */
// RF4_SHRIEK                   0x00000001      /* Shriek for help */
  {0, 2, 0, 0},
// RF4_UNMAGIC                  0x00000002      /* (?) */
  {0, 0, 0, 0},
// (S_ANIMAL) RF4_XXX3                  0x00000004      /* (?) */
  {0, 0, 0, 0},
// RF4_ROCKET                   0x00000008      /* (?) */
  {0, 60, 70, 0},
// RF4_ARROW_1                  0x00000010      /* Fire an arrow (light) */
  {0, 2, 5, 0},
// RF4_ARROW_2                  0x00000020      /* Fire an shot (heavy) */
  {0, 2, 6, 0},
// XXX (RF4_ARROW_3)                    0x00000040      /* Fire bolt (heavy) */
  {0, 2, 7, 0},
// XXX (RF4_ARROW_4)                    0x00000080      /* Fire missiles (heavy) */
  {0, 3, 9, 0},
// RF4_BR_ACID                  0x00000100      /* Breathe Acid */
  {0, 10, 20, 0},
// RF4_BR_ELEC                  0x00000200      /* Breathe Elec */
  {0, 10, 20, 0},
// RF4_BR_FIRE                  0x00000400      /* Breathe Fire */
  {0, 10, 20, 0},
// RF4_BR_COLD                  0x00000800      /* Breathe Cold */
  {0, 10, 20, 0},
// RF4_BR_POIS                  0x00001000      /* Breathe Poison */
  {0, 13, 25, 0},
// RF4_BR_NETH                  0x00002000      /* Breathe Nether */
  {0, 22, 35, 0},
// RF4_BR_LITE                  0x00004000      /* Breathe Lite */
  {0, 10, 20, 0},
// RF4_BR_DARK                  0x00008000      /* Breathe Dark */
  {0, 10, 20, 0},
// RF4_BR_CONF                  0x00010000      /* Breathe Confusion */
  {0, 10, 25, 0},
// RF4_BR_SOUN                  0x00020000      /* Breathe Sound */
  {0, 14, 30, 0},
// RF4_BR_CHAO                  0x00040000      /* Breathe Chaos */
  {0, 25, 40, 0},
// RF4_BR_DISE                  0x00080000      /* Breathe Disenchant */
  {0, 28, 45, 0},
// RF4_BR_NEXU                  0x00100000      /* Breathe Nexus */
  {0, 20, 45, 0},
// RF4_BR_TIME                  0x00200000      /* Breathe Time */
  {0, 30, 50, 0},
// RF4_BR_INER                  0x00400000      /* Breathe Inertia */
  {0, 25, 35, 0},
// RF4_BR_GRAV                  0x00800000      /* Breathe Gravity */
  {0, 25, 35, 0},
// RF4_BR_SHAR                  0x01000000      /* Breathe Shards */
  {0, 15, 25, 0},
// RF4_BR_PLAS                  0x02000000      /* Breathe Plasma */
  {0, 25, 30, 0},
// RF4_BR_WALL                  0x04000000      /* Breathe Force */
  {0, 30, 40, 0},
// RF4_BR_MANA                  0x08000000      /* Breathe Mana */
  {0, 35, 45, 0},
// RF4_BR_DISI                  0x10000000
  {0, 50, 70, 0},
// RF4_BR_NUKE                  0x20000000
  {0, 27, 40, 0},
// 0x40000000
  {0, 0, 0, 0},
// RF4_BOULDER
  {0, 2, 15, 0},

/*
 * New monster race bit flags
 */
// RF5_BA_ACID                  0x00000001      /* Acid Ball */
  {0, 8, 10, 0},
// RF5_BA_ELEC                  0x00000002      /* Elec Ball */
  {0, 8, 10, 0},
// RF5_BA_FIRE                  0x00000004      /* Fire Ball */
  {0, 8, 10, 0},
// RF5_BA_COLD                  0x00000008      /* Cold Ball */
  {0, 8, 10, 0},
// RF5_BA_POIS                  0x00000010      /* Poison Ball */
  {0, 11, 20, 0},
// RF5_BA_NETH                  0x00000020      /* Nether Ball */
  {0, 25, 40, 0},
// RF5_BA_WATE                  0x00000040      /* Water Ball */
  {0, 17, 30, 0},
// RF5_BA_MANA                  0x00000080      /* Mana Storm */
  {0, 45, 50, 0},
// RF5_BA_DARK                  0x00000100      /* Darkness Storm */
  {0, 20, 0, 0},
// RF5_DRAIN_MANA               0x00000200      /* Drain Mana */
  {0, 0, 0, 0},
// RF5_MIND_BLAST               0x00000400      /* Blast Mind */
  {0, 15, 13, 0},
// RF5_BRAIN_SMASH              0x00000800      /* Smash Brain */
  {0, 25, 15, 0},
// RF5_CAUSE_1                  0x00001000      /* Cause Light Wound */
  {0, 3, 15, 0},
// RF5_CAUSE_2                  0x00002000      /* Cause Serious Wound */
  {0, 6, 20, 0},
// RF5_BA_NUKE                  0x00004000      /* Toxic Ball */
  {0, 30, 30, 0},
// RF5_BA_CHAO                  0x00008000      /* Chaos Ball */
  {0, 45, 40, 0},
// RF5_BO_ACID                  0x00010000      /* Acid Bolt */
  {0, 4, 13, 0},
// RF5_BO_ELEC                  0x00020000      /* Elec Bolt (unused) */
  {0, 3, 13, 0},
// RF5_BO_FIRE                  0x00040000      /* Fire Bolt */
  {0, 4, 13, 0},
// RF5_BO_COLD                  0x00080000      /* Cold Bolt */
  {0, 3, 13, 0},
// RF5_BO_POIS                  0x00100000      /* Poison Bolt (unused) */
  {0, 5, 16, 0},
// RF5_BO_NETH                  0x00200000      /* Nether Bolt */
  {0, 15, 20, 0},
// RF5_BO_WATE                  0x00400000      /* Water Bolt */
  {0, 13, 18, 0},
// RF5_BO_MANA                  0x00800000      /* Mana Bolt */
  {0, 20, 25, 0},
// RF5_BO_PLAS                  0x01000000      /* Plasma Bolt */
  {0, 15, 18, 0},
// RF5_BO_ICEE                  0x02000000      /* Ice Bolt */
  {0, 10, 15, 0},
// RF5_MISSILE                  0x04000000      /* Magic Missile */
  {0, 2, 5, 0},
// RF5_SCARE                    0x08000000      /* Frighten Player */
  {0, 3, 8, 0},
// RF5_BLIND                    0x10000000      /* Blind Player */
  {0, 5, 8, 0},
// RF5_CONF                     0x20000000      /* Confuse Player */
  {0, 5, 8, 0},
// RF5_SLOW                     0x40000000      /* Slow Player */
  {0, 7, 10, 0},
// RF5_HOLD                     0x80000000      /* Paralyze Player (translates into forced monsleep) */
  {0, 5, 10, 0},

/*
 * New monster race bit flags
 */
// RF6_HASTE                    0x00000001      /* Speed self */
  {0, 10, 20, 0},
// RF6_HAND_DOOM                0x00000002      /* Speed a lot (?) */
  {0, 100, 80, 0},
// RF6_HEAL                     0x00000004      /* Heal self */
  {0, 10, 20, 0},
// RF6_XXX2                     0x00000008      /* Heal a lot (?) */
  {0, 0, 0, 0},
// RF6_BLINK                    0x00000010      /* Teleport Short */
  {0, 5, 15, 0},
// RF6_TPORT                    0x00000020      /* Teleport Long */
  {0, 20, 30, 0},
// RF6_XXX3                     0x00000040      /* Move to Player (?) */
  {0, 0, 0, 0},
// RF6_XXX4                     0x00000080      /* Move to Monster (?) */
  {0, 0, 0, 0},
// RF6_TELE_TO                  0x00000100      /* Move player to monster */
  {0, 20, 30, 0},
// RF6_TELE_AWAY                0x00000200      /* Move player far away */
  {0, 25, 30, 0},
// RF6_TELE_LEVEL               0x00000400      /* Move player vertically */
  {0, 30, 60, 0},
// RF6_XXX5                     0x00000800      /* Move player (?) */
  {0, 0, 0, 0},
// RF6_DARKNESS         0x00001000      /* Create Darkness */
  {0, 6, 8, 0},
// RF6_TRAPS                    0x00002000      /* Create Traps */
  {0, 15, 25, 0},
// RF6_FORGET                   0x00004000      /* Cause amnesia */
  {0, 25, 35, 0},

};



/* Runecraft --------------------------------------------------------------- */

r_element r_elements[RCRAFT_MAX_ELEMENTS] =
{
{ R_LITE, "Light", 	SKILL_R_LITE },
{ R_DARK, "Darkness",	SKILL_R_DARK },
{ R_NEXU, "Nexus", 	SKILL_R_NEXU },
{ R_NETH, "Nether", 	SKILL_R_NETH },
{ R_CHAO, "Chaos", 	SKILL_R_CHAO },
{ R_MANA, "Mana", 	SKILL_R_MANA },
};

r_imperative r_imperatives[RCRAFT_MAX_IMPERATIVES+1] =
{
{ I_MINI, "minimized",	 0,  6, -10,  6, -1,  8, 10 },
{ I_LENG, "lengthened",	 2,  8,  +5, 10,  0, 15, 10 },
{ I_COMP, "compressed",	 3, 13,  -5, 13, -2,  8, 10 },
{ I_MODE, "moderate",	 5, 10,   0, 10,  0, 10, 10 },
{ I_EXPA, "expanded",	 7, 13,  +5, 10, +2, 13, 10 },
{ I_BRIE, "brief",	 8,  8,  -5,  8,  0,  6,  5 },
{ I_MAXI, "maximized",	10, 15, +10, 15, +1, 13, 10 },
{ I_ENHA, "enhanced",	10, 15,   0, 10,  0, 10, 10 },
};

r_type r_types[RCRAFT_MAX_TYPES+1] =
{
{ T_BOLT, "bolt",	 1,  1, 15, 3,  4, 53, 27,  0,   0, 0, 0,  0,  0 },
{ T_BEAM, "beam",	 5,  2, 25, 3,  9, 53, 32,  9, 169, 0, 0,  6, 14 },
{ T_CLOU, "cloud",	10,  3, 30, 0,  0,  0,  0,  9, 169, 1, 5,  6, 14 },
{ T_BALL, "ball",	15,  5, 30, 0,  0, 28, 28, 25, 784, 2, 7,  0,  0 },
{ T_WAVE, "wave",	20,  4, 40, 0,  0,  0,  0, 25, 441, 0, 0,  6, 10 },
{ T_STOR, "storm",	25, 10, 40, 5, 10, 25, 25, 16, 256, 1, 3, 10, 35 },
{ T_RUNE, "rune",	30, 20, 20, 0,  0,  0,  0, 16, 625, 1, 2,  3,  7 },
{ T_ENCH, "augment",	35, 50, 50, 0,  0,  0,  0,  0,   0, 0, 0,  0,  0 },
};

r_projection r_projections[RCRAFT_MAX_PROJECTIONS] =
{
{ R_LITE,		GF_LITE,	400,	"light" },
{ R_DARK,		GF_DARK,	900,	"darkness" },
{ R_NEXU,		GF_NEXUS,	350,	"lightning" },
{ R_NETH,		GF_NETHER,	550,	"nether" },
{ R_CHAO,		GF_CHAOS,	600,	"chaos" },
{ R_MANA,		GF_MANA,	550,	"mana" },

{ R_LITE | R_DARK,	GF_CONFUSION,	400,	"confusion" },
{ R_LITE | R_NEXU,	GF_INERTIA,	350,	"inertia" },
{ R_LITE | R_NETH,	GF_ELEC,	1200,	"lightning" },
{ R_LITE | R_CHAO,	GF_FIRE,	1200,	"fire" },
{ R_LITE | R_MANA,	GF_WAVE,	250,	"water" },

{ R_DARK | R_NEXU,	GF_GRAVITY,	300,	"gravity" },
{ R_DARK | R_NETH,	GF_COLD,	1200,	"frost" },
{ R_DARK | R_CHAO,	GF_ACID,	600,	"acid" },
{ R_DARK | R_MANA,	GF_POIS,	800,	"poison" },

{ R_NEXU | R_NETH,	GF_TIME,	150,	"time" },
{ R_NEXU | R_CHAO,	GF_SOUND,	400,	"sound" },
{ R_NEXU | R_MANA,	GF_SHARDS,	400,	"shards" },

{ R_NETH | R_CHAO,	GF_DISENCHANT,	500,	"disenchantment" },
{ R_NETH | R_MANA,	GF_FORCE,	400,	"force"},

{ R_CHAO | R_MANA,	GF_PLASMA,	700,	"plasma" },
};

