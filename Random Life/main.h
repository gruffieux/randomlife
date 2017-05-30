#ifndef MAIN_HEADER
#define MAIN_HEADER

#define TOTAL_ORC_RECORD 20
#define TOTAL_COMBAT_RECORD 6

#define TOTAL_ZONE_X 7
#define TOTAL_ZONE_Y 6
#define TOTAL_LIVING_TILE_ID 10
#define MAX_LIVING_TILE 80 * 80

#define BASIC_XP 25
#define BONUS_POINTS 10
#define MAGIC_POINTS 20
#define STAT_PRICE 10
#define SPELL_PRICE 10
#define CHEMICAL_SCALE 5
#define WORLD_TIME_SEC_PER_DAY 5 * 60
#define WORLD_TIME_SEC_BEFORE_CHAO 5 * 3600
#define POTION_GIFT_POWER 30
#define TOTAL_POTION_GIFT 5

#define TOTAL_MESSAGE_TYPE 8
#define TOTAL_BOOK_COMMENT 7

#define ACTION_MSG_COLOR RGB(0, 0, 0)
#define SKILL_RESULT_MSG_COLOR RGB(136, 136, 136)
#define CAST_SPELL_MSG_COLOR RGB(0, 255, 0)
#define DAMAGE_MSG_COLOR RGB(255, 0, 0)
#define MAGIC_EFFECT_MSG_COLOR RGB(255, 128, 0)
#define XP_MSG_COLOR RGB(226, 221, 5)
#define PROGRESS_MSG_COLOR RGB(0, 0, 255)
#define TRADING_MSG_COLOR RGB(0, 168, 0)
#define LEARN_SPELL_MSG_COLOR RGB(0, 232, 232)

#define TOTAL_CHAR_ANIMAL_SPIDER 5
#define TOTAL_CHAR_ANIMAL_FLATSNAKE 5
#define TOTAL_CHAR_ANIMAL_PINKSNAKE 5
#define TOTAL_CHAR_ANIMAL_BLACKBEAR 5
#define TOTAL_CHAR_ANIMAL_MELEFIC_DOG 2
#define TOTAL_CHAR_HUMANIZED_ROBERT 5
#define TOTAL_CHAR_HUMANIZED_REDFORD 5
#define TOTAL_CHAR_HUMANIZED_MINI_POUX 5
#define TOTAL_CHAR_HUMANIZED_POUX 5
#define TOTAL_CHAR_HUMANIZED_TROLL 5
#define TOTAL_CHAR_HUMANIZED_BAD_LOGGER 5
#define TOTAL_CHAR_HUMANIZED_MAD_LOGGER 5
#define TOTAL_CHAR_HUMANIZED_MARSHPLANT 5
#define TOTAL_CHAR_HUMANIZED_YUNG_ACOLYT 5
#define TOTAL_CHAR_HUMANIZED_OLD_ACOLYT 5
#define TOTAL_CHAR_HUMANIZED_LINK_ACOLYT 3
#define TOTAL_CHAR_PNJ 6
#define TOTAL_CHAR_PNJ_CITIZEN 5

#define HTH_MIN_DAMAGE 1
#define HTH_MAX_DAMAGE 2
#define CHAR_SPEED 2.9
#define TOTAL_COMBAT_PHASE 6
#define TOTAL_CHAR_SKIN 9
#define TOTAL_CHAR_ANI 11
#define TOTAL_CHAR_MAGIC_SPELL_EFFECT 7
#define TOTAL_CHAR_SKILL_RESULT 5
#define TOTAL_CHAR_SWATCH 4
#define TOTAL_CHAR_PNJ_QUEST 5
#define MAX_CHAR_STAT_SCORE 10
#define MAX_CHAR_SKILL_LEVEL 12
#define MAX_CHAR_MAGIC_LEVEL 14

#define TOTAL_PICKUP_MONEY_POORBAG 5
#define TOTAL_PICKUP_MONEY_MEDIUMBAG 10
#define TOTAL_PICKUP_MONEY_RICHBAG 10
#define TOTAL_PICKUP_FLOWER_WHITE 10
#define TOTAL_PICKUP_FLOWER_RED 10
#define TOTAL_PICKUP_FLOWER_BLUE 10
#define TOTAL_PICKUP_FRUIT_RED 6
#define TOTAL_PICKUP_FRUIT_PINK 6
#define TOTAL_PICKUP_FRUIT_BLUE 6
#define TOTAL_PICKUP_FRUIT_BLACK 6
#define TOTAL_PICKUP_FRUIT_ORANGE 6
#define TOTAL_PICKUP_DRINK_WATER 10
#define TOTAL_PICKUP_DRINK_WINE 20
#define TOTAL_PICKUP_POTION_LIFE 5
#define TOTAL_PICKUP_POTION_MANA 5
#define TOTAL_PICKUP_POTION_CHEAP 10
#define TOTAL_PICKUP_POTION_DF 3
#define TOTAL_PICKUP_WEAPON_WK_DAGGER 7
#define TOTAL_PICKUP_WEAPON_ST_DAGGER 3
#define TOTAL_PICKUP_WEAPON_MD_SWORD 8
#define TOTAL_PICKUP_WEAPON_ST_SWORD 2
#define TOTAL_PICKUP_WEAPON_PNJ_SWORD 3
#define TOTAL_PICKUP_WEAPON_WK_MACE 3
#define TOTAL_PICKUP_WEAPON_MD_MACE 4
#define TOTAL_PICKUP_WEAPON_ST_MACE 3
#define TOTAL_PICKUP_WEAPON_MD_AXE 8
#define TOTAL_PICKUP_WEAPON_ST_AXE 2
#define TOTAL_PICKUP_WEAPON_DAGGY 3
#define TOTAL_PICKUP_ARMOR_WK_LIGHT 5
#define TOTAL_PICKUP_ARMOR_MD_LIGHT 5
#define TOTAL_PICKUP_ARMOR_ST_LIGHT 5
#define TOTAL_PICKUP_ARMOR_PNJ1_LIGHT 5
#define TOTAL_PICKUP_ARMOR_MAGICIAN_DRESS 3
#define TOTAL_PICKUP_ARMOR_WK_HEAVY 5
#define TOTAL_PICKUP_ARMOR_MD_HEAVY 5
#define TOTAL_PICKUP_ARMOR_ST_HEAVY 5
#define TOTAL_PICKUP_ARMOR_PNJ_HEAVY 2

#define TOTAL_PICKUP_INGREDIENT_TYPE 12
#define TOTAL_PICKUP_GROUP 4
#define TOTAL_PICKUP_POTION_EFFECT 5
#define TOTAL_PICKUP_POTION_ELEMENT 3

#define TOTAL_MUSIC_CATEGORY 7

enum CONFIGURABLE_KEY
{
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
	KEY_ALWAYSWALK,
	KEY_ATTACK,
	KEY_CASTSPELL,
	KEY_MEDITATE,
	KEY_PICKUP,
	KEY_NEXTTURN,
	KEY_SHORTCUT,
	KEY_SHORT_INVENTORY,
	KEY_SHORT_CHARINFO,
	KEY_SHORT_SPELLLIST,
	KEY_SHORT_CHEMICALLAB,
	KEY_SHORT_LANDMAP,
	KEY_SHORT_QUESTBOOK,
	KEY_SHORT_EVENTS
};

enum SHORT_PANEL
{
	SHORT_INVENTORY,
	SHORT_CHARINFO,
	SHORT_SPELLLIST,
	SHORT_CHEMICALLAB,
	SHORT_LANDMAP,
	SHORT_QUESTBOOK,
	SHORT_EVENTS,
	SHORT_MENU
};

enum ENERGY_BAR_TYPE
{
	NONE_ENERGY_BAR_TYPE,
	LIFE,
	MANA,
	CHARGE,
	XP
};

enum MAP_LAYER
{
	GROUND,
	DECORATION,
	ITEMS,
	BRIDGES,
	PATHNODES
};

enum ACTOR_LIVING_TILE_ID
{
	NONE_LIVING_TILE_ID,
	GRASS,
	WATER,
	MONTAIN,
	HIGH_MONTAIN,
	VILLAGE,
	MUD,
	FOREST,
	SNOW,
	RUINS
};

enum CHAR_RACE
{
	NONE_CHAR_RACE,
	ANIMAL,
	HUMANIZED
};

enum CHAR_CLASS
{
	NONE_CHAR_CLASS,
	WEAK,
	MEDIUM,
	STRONG,
	PNJ1,
	PNJ2
};

enum CHAR_ANI
{
	STOP,
	MOVE,
	HITTED,
	SLEEP,
	DEAD,
	ATTACK_HTH,
	ATTACK_DAGGER,
	ATTACK_MACE,
	ATTACK_SWORD,
	ATTACK_AXE,
	CONCENTRATE
};

enum CHAR_SKIN
{
	NONE_CHAR_SKIN,
	CLOTHES,
	LEATHER_CLOTHES,
	MESH_COAT,
	WHITE_DRESS,
	BLACK_DRESS,
	BLUE_DRESS,
	PADS_ARMOR,
	IRON_ARMOR
};

enum CHAR_ACTION
{
	NONE_CHAR_ACTION,
	MOVEMENT,
	ATTACK,
	CAST_SPELL,
	MEDITATE,
	SPY,
	STEAL,
	CRAFT_POTION
};

enum CHAR_STAT
{
	NONE_CHAR_STAT,
	AGILITY,
	DEXTERITY,
	HEALTH,
	INTELLIGENCE,
	PERCEPTION,
	STRENGTH,
	WILLPOWER
};

enum CHAR_STAT2
{
	NONE_CHAR_STAT2,
	HIT_POINTS,
	MANA_POINTS,
	CHARGE_POINTS,
	MAGICAL_AFFINITY,
	COMBAT_SPEED
};

enum CHAR_SKILL
{
	NONE_CHAR_SKILL,
	MEDITATION,
	LIGHT_ARMOR,
	HEAVY_ARMOR,
	SPYING,
	STEALING,
	DISCRETION,
	CHEMISTRY
};

enum CHAR_COMBAT_SKILL
{
	NONE_CHAR_COMBAT_SKILL,
	HAND_TO_HAND,
	DAGGERS,
	MACES,
	SWORDS,
	AXES
};

enum CHAR_MAGIC_SCHOOL
{
	NONE_CHAR_MAGIC_SCHOOL,
	CURE,
	DESTRUCTION,
	PROTECTION
};

enum CHAR_MAGIC_SPELL_EFFECT
{
	NONE_CHAR_MAGIC_SPELL_EFFECT,
	SELF_HEAL,		//Gu�rison
	LIFE_DAMAGE,	//Destruction
	CHOC_WAVE,		//Destruction
	ANTIMAGIC,		//Destruction
	ABSORBING,		//Protection
	REFLECTING		//Protection
};

enum CHAR_SWATCH
{
	BASIC,
	SPELL_CASTING,
	POTION_EFFECT,
	SEARCH
};

enum CHAR_SKILL_RESULT
{
	NONE_CHAR_SKILL_RESULT,
	CRITICAL_SUCCESS,
	SUCCESS,
	FAILURE,
	CRITICAL_FAILURE
};

enum CHAR_ANIMAL_TYPE
{
	SPIDER,
	FLAT_SNAKE,
	PINK_SNAKE,
	BLACK_BEAR,
	MALEFIC_DOG
};

enum CHAR_BEHAVIOR
{
	PEACEFUL,
	AGGRO,
	THIEF
};

enum CHAR_PNJ_JOB
{
	NONE_CHAR_PNJ_JOB,
	TRADER,
	MAGIC_LEARNER,
	QUEST_GIVER
};

enum CHAR_PNJ_QUEST
{
	NONE_CHAR_PNJ_QUEST,
	WARRIOR_QUEST,
	PRIEST_QUEST,
	GUARD_QUEST,
	FINAL_QUEST
};

enum PICKUP_TYPE
{
	NONE_PICKUP_TYPE,
	MONEY,
	MESSAGE,
	JEWEL,
	FLOWER,
	FRUIT,
	DRINK,
	POTION,
	WEAPON,
	ARMOR
};

enum PICKUP_INGREDIENT_TYPE
{
	NONE_PICKUP_INGREDIENT_TYPE,
	FLOWER_WHITE,
	FLOWER_RED,
	FLOWER_BLUE,
	FRUIT_RED,
	FRUIT_PINK,
	FRUIT_BLUE,
	FRUIT_BLACK,
	FRUIT_ORANGE,
	DRINK_WATER,
	DRINK_WINE,
	DRINK_TROLLFLUID
};

enum PICKUP_UNIQUE_ID
{
	NONE_PICKUP_UNIQUE_ID,
	DEFAULTBAG,
	AUTHORIZATION,
	MINDPROTECTOR,
	MAGICCIRCLE,
	DISCRETIONFLOW,
	NECROMISERY,
	AXEL,
	PRIESTDAGGY,
	WIZARDDAGGY,
	GMAGODAGGY,
	WIZARDSTAFF,
	SYLVANEL,
	PRIESTDRESS,
	WIZARDDRESS,
	GMAGODRESS
};

enum PICKUP_GROUP
{
	OTHERS,
	WEAPONS,
	ARMORS,
	INGREDIENTS
};

enum PICKUP_ENCHANT
{
	NONE_PICKUP_ENCHANT,
	CURE_SCHOOL_BONUS,
	DESTRUCTION_SCHOOL_BONUS,
	PROTECTION_SCHOOL_BONUS,
	SUMMON_NECROMISERY,
	MAGIC_ABSORBER,
	MAGIC_TRANSPERCOR,
	LIFE_MANA_GENERATOR
};

enum PICKUP_POTION_EFFECT
{
	NONE_PICKUP_POTION_EFFECT,
	LIFE_REGEN,
	MANA_REGEN,
	MANA_TRANS_LIFE,
	INVISIBILITY
};

enum EVENT_TYPE
{
	NONE_EVENT_TYPE,
	SKILL_UP,
	COMBAT_SKILL_UP,
	MAGIC_SKILL_UP,
	ACTION_SUCCESS,
	ACTION_FAILED,
	PICKUP_STEALED,
	PICKUP_SUMMONED,
	POTIONGIFT_ADDED
};

enum MESSAGE_TYPE
{
	DEBUG_ACTION,
	CURRENT_ACTION,
	ACTION_DENIED,
	BUY_PICKUP_DENIED,
	LEARN_SPELL_DENIED,
	CAST_SPELL_DENIED,
	XP_GAIN_DENIED,
	SAVE_DATA
};

enum BOOK_COMMENT
{
	WARRIOR_STEP1,
	WARRIOR_STEP2,
	PRIEST_STEP1,
	PRIEST_STEP2,
	PRIEST_STEP3,
	FINAL_STEP1,
	FINAL_STEP2
};

enum MUSIC_CATEGORY
{
	NONE_MUSIC_CATEGORY,
	MC_OUTLAND,
	MC_VILLAGE,
	MC_MONTAIN,
	MC_FOREST,
	MC_SNOW,
	MC_WIZARD_FORTRESS
};

enum SOUND_EFFECT
{
	POTION_DRINK,
	DOG_GO,
	BEAR_GO,
	ANIMAL_BITE,
	ACOLYT_GO,
	CRAFT_SUCCESS,
	TRADING,
	POTION_BREAK,
	SPELL_LEARNT,
	DOG_ATTACK,
	SPELL_EFFECT_HEAL,
	SPELL_EFFECT_PROTECTION,
	SPELL_EFFECT_WAVE,
	CRAFT_SPECIAL,
	SPELL_EFFECT_ANTIMAGIC,
	POUX_GO,
	TROLL_GO,
	CHAO,
	HUMAN_SCREAM,
	LEVEL_UP,
	HUMAN_HIT_MACE,
	HUMAN_HIT_HTH,
	LOGGER_GO,
	HUMAN_ATTACK,
	SPELL_MEMORIZED,
	HUMAN_LAUGH,
	SNAKE_GO,
	SNAKE_ATTACK,
	HUMAN_SLEEP,
	HUMAN_HIT_AXE,
	SPELL_EFFECT_DAMAGE,
	ACTION_FORBIDDEN,
	CRAFT_FAILURE
};

struct ORC
{
	int SRS[TOTAL_ORC_RECORD], min[TOTAL_ORC_RECORD], max[TOTAL_ORC_RECORD];
	ORC()
	{
		int i;

		for (i = 0; i < TOTAL_ORC_RECORD; i++)
			SRS[i] = min [i] = max[i] = 0;
	}
};

struct BonusPoints
{
	int level, total, gain;
	BonusPoints() {level = total = gain = 0;}
};

struct FlagedData
{
	int data;
	bool flag;
	FlagedData()
	{
		data = 0; 
		flag = false;
	}
};

struct CharCombat
{
	int MinCS, MaxCS, MaxDistance, action[TOTAL_COMBAT_PHASE];
	CharCombat()
	{
		int i;

		MinCS = MaxCS = MaxDistance = 0;
		for (i = 0; i < TOTAL_COMBAT_PHASE; i++)
			action[i] = 0;
	}
};

struct CharStatus
{
	bool access, look, spy, fight, turn, attacking, attack, concentrate, hit, hitted, stunned, WpStunned, absorbing, reflecting, ResistSpell, invisible, sleep, trading, cursed, dead, playingSound, PlayingRepeatSound;
	CHAR_ACTION Action;
	CharStatus()
	{
		access = look = spy = fight = attacking = attack = concentrate = hit = hitted = stunned = WpStunned = absorbing = reflecting = ResistSpell = invisible = sleep = trading = cursed = dead = playingSound = PlayingRepeatSound = false;
		turn = true;
		Action = NONE_CHAR_ACTION;
	}
};

struct CharZoneStatus
{
	short dead, spyed;
	CharZoneStatus() {dead = spyed = 0;}
};

struct EventData1
{
	int score, RealScore;
	EventData1() {score = RealScore = 0;}
};

struct EventData2
{
	float score, RealScore;
	EventData2() {score = RealScore = 0;}
};

struct CharBehavior
{
	bool progress, aggro, thief, CurseInfluenced, move, dispatch, GetPickup;
	CHAR_SKILL FavoriteSkill1, FavoriteSkill2;
	CHAR_COMBAT_SKILL FavoriteCombatSkill;
	PICKUP_GROUP ThiefType;
	CharBehavior()
	{
		progress = aggro = thief = move = dispatch = GetPickup = false;
		CurseInfluenced = true;
		FavoriteSkill1 = FavoriteSkill2 = NONE_CHAR_SKILL;
		FavoriteCombatSkill = NONE_CHAR_COMBAT_SKILL;
		ThiefType = OTHERS;
	}
};

struct PickupMalus
{
	int AGModifier, STModifier, WPModifier;
	PickupMalus() {AGModifier = STModifier = WPModifier = 0;}
	PickupMalus(int PM_AGModifier, int PM_STModifier, int PM_WPModifier)
	{
		AGModifier = PM_AGModifier;
		STModifier = PM_STModifier;
		WPModifier = PM_WPModifier;
	}
};

struct PickupStatus
{
	bool picked, equiped;
	PickupStatus() {picked = equiped = false;}
};

struct Zone
{
	int CoordX, CoordY, MinCreature, MaxCreature, AnimalCreatureRate, HumanizedCreatureRate, WeakCreatureRate, MediumCreatureRate, StrongCreatureRate, category;
	bool GroundLayer, DecoLayer, ItemsLayer, BridgesLayer, PathNodesLayer;
	short cursed;
	char *GroundFileName, *DecoFileName, *ItemsFileName, *BridgesFileName;
};

#endif
