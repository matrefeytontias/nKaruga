#pragma once

#include <cstdint>

#include <n2DLib/n2DLib_math.h>
#include <SDL2/SDL_mixer.h>

#include "types.h"

struct Constants
{
	// General
	static constexpr int FPS = 60;
	static constexpr int SCREEN_WIDTH = 320;
	static constexpr int SCREEN_HEIGHT = 240;

	// General strings
	static constexpr char CONFIG_FILENAME[] = "nKaruga.cfg";
	static constexpr char TITLE_STRING[] = "Press any key";
	static constexpr char CONTINUE_TEXT[] = "Continue ? (Fire/Polarity : yes/no)";

	// Difficulty settings
	enum struct DifficultySetting
	{
		EASY = 0,
		NORMAL,
		HARD,
	};
	
	// Game phases
	enum struct GamePhase
	{
		PLAY,
		TRANSITION,
		BOSSCINEMATIC,
		BOSSFIGHT,
		BOSSEXPLODEINIT,
		BOSSEXPLODE,
		RESULTS,
	};

	// Enemy types
	enum struct EnemyType
	{
		ENEMY,
		PROP,
		BREAKABLE_PROP
	};

	// /!\ Fixed-point value
	static constexpr Fixed CAMERA_SPEED = itofix(1) / 3;
	// /!\ integer value
	// TODO : make this fixed
	static constexpr int INV_CAMERA_SPEED = 3;

	// TODO : make sure having both DEFAULT and RELATIVE makes sense
	// Camera relations
	enum struct CamRelation
	{
		DEFAULT,
		ABSOLUTE,
		RELATIVE
	};


	// TODO : get rid of both of those
	// also SHADOW -> DARK
	static constexpr int LIGHT = 0;
	static constexpr int SHADOW = 1;

	// Enemies
	static constexpr int MAX_ENEMY = 500;
	// Max amount of enemies that can be killed each frame.
	// It's very unlikely to ever get that high.
	static constexpr int MAX_KILLED_ENEMIES_PER_FRAME = 24;

	// Bullets
	static constexpr int MAX_BULLET = 1000;
	static constexpr int MAX_HOMING = 100;
	static constexpr int MAX_FRAGMENT = 12;
	static constexpr int MAX_LASER = 12;

	static constexpr int MAX_STORED_POWER = 120;

	static constexpr int LASER_SPEED = 6;
	static constexpr int LASER_THICKNESS = 25;

	static constexpr int SCORE_HIT = 20;
	static constexpr int SCORE_HIT_OP = 40;
	static constexpr int SCORE_ABSORB = 100;

	// Particles
	static constexpr int MAX_PARTICLE = 1024;
	static constexpr int PARTICLE_RADIUS = 4;

	// Menu items
	static constexpr const char* BOOLEAN_STRINGS[] = { "no", "yes" };
	static constexpr int TITLE_OPTIONS = 4;
	static constexpr const char* TITLE_MENU_OPTIONS[TITLE_OPTIONS] = {
		"Play",
		"Difficulty:",
		"Configure controls",
		"Quit"
	};

	static constexpr int KEYS_TO_BIND = 8;
	static constexpr const char* KEYBINDINGS_NAMES[KEYS_TO_BIND] = {
		"Down\n",
		"Left\n",
		"Right\n",
		"Up\n",
		"Fire\n",
		"Switch polarity\n",
		"Release power\n",
		"Pause the game\n"
	};

	static constexpr const char* DIFFICULTIES_NAMES[] = {
		"Easy",
		"Normal",
		"Hard"
	};

	static constexpr const char* RESULTS_TEXT[6] = {
		"BATTLE REPORT\n\n\n",
		"Boss destroy bonus\n\n",
		"Score (this chapter)\n\n",
		"Max ",
		" chains\n\n",
		"Grade\n\n"
	};

	// (start x, end x) coordinates of the horizontal lines to draw to fill a power slot, relative to the power slot itself
	// Bottom-up
	static constexpr int POWER_SLOT_FILL_COORDINATES[] = { 1, 2, 1, 5, 1, 8, 1, 10, 1, 10, 1, 10, 1, 10, 3, 10, 6, 10, 9, 10 };

};

struct LUTs
{
	// Base images LUT
	enum struct BaseImageId
	{
		DEFAULT = 0,
		PLAYER_SHIP_LIGHT,
		PLAYER_SHIP_SHADOW,
		PLAYER_SHIP_POLARITYSWITCH_0_LIGHT,
		PLAYER_SHIP_POLARITYSWITCH_0_SHADOW,
		PLAYER_SHIP_POLARITYSWITCH_1_LIGHT,
		PLAYER_SHIP_POLARITYSWITCH_1_SHADOW,
		PLAYER_SHIP_INVINCIBLE_LIGHT,
		PLAYER_SHIP_INVINCIBLE_SHADOW,
		PLAYER_BULLET_LIGHT,
		PLAYER_BULLET_SHADOW,
		PLAYER_HOMING_BULLET_LIGHT_0,
		PLAYER_HOMING_BULLET_LIGHT_1,
		PLAYER_HOMING_BULLET_LIGHT_2,
		PLAYER_HOMING_BULLET_SHADOW_0,
		PLAYER_HOMING_BULLET_SHADOW_1,
		PLAYER_HOMING_BULLET_SHADOW_2,
		PLAYER_EXPLOSION_0,
		PLAYER_EXPLOSION_1,
		PLAYER_EXPLOSION_2,
		PLAYER_EXPLOSION_3,
		PLAYER_EXPLOSION_4,
		PLAYER_EXPLOSION_5,
		PLAYER_EXPLOSION_6,
		PLAYER_EXPLOSION_7,
		PLAYER_EXPLOSION_8,
		PLAYER_EXPLOSION_9,
		PLAYER_EXPLOSION_10,
		PLAYER_EXPLOSION_11,
		ENEMY_BULLET_0_LIGHT,
		ENEMY_BULLET_0_SHADOW,
		ENEMY_BULLET_1_LIGHT,
		ENEMY_BULLET_1_SHADOW,
		ENEMY_BULLET_2_LIGHT,
		ENEMY_BULLET_2_SHADOW,
		ENEMY_HOMING_BULLET_LIGHT,
		ENEMY_HOMING_BULLET_SHADOW,
		ENEMY_LASER_LIGHT,
		ENEMY_LASER_SHADOW,
		ENEMY_GENERATOR_LIGHT,
		ENEMY_GENERATOR_SHADOW,
		ENEMY_WALL_LIGHT,
		ENEMY_WALL_SHADOW,
		ENEMY_SHIP_0_LIGHT,
		ENEMY_SHIP_0_SHADOW,
		ENEMY_SHIP_1_LIGHT,
		ENEMY_SHIP_1_SHADOW,
		ENEMY_SHIP_2_LIGHT,
		ENEMY_SHIP_2_SHADOW,
		ENEMY_SHIP_3_LIGHT,
		ENEMY_SHIP_3_SHADOW,
		ENEMY_SHIP_4_LIGHT,
		ENEMY_SHIP_4_SHADOW,
		ENEMY_SHIP_4_LIGHTBALL,
		ENEMY_SHIP_4_SHADOWBALL,
		ENEMY_SHIP_5_LIGHT,
		ENEMY_SHIP_5_SHADOW,
		ENEMY_SHIP_6_LIGHT,
		ENEMY_SHIP_6_SHADOW,
		DOOR_LEFT,
		DOOR_RIGHT,
		BOX_LIGHT_1,
		BOX_LIGHT_2,
		BOX_LIGHT_3,
		BOX_LIGHT_4,
		BOX_LIGHT_5,
		BOX_LIGHT_6,
		BOX_SHADOW_1,
		BOX_SHADOW_2,
		BOX_SHADOW_3,
		BOX_SHADOW_4,
		BOX_SHADOW_5,
		BOX_SHADOW_6,
		BOX_SOLID_1,
		BOX_SOLID_2,
		BOX_SOLID_3,
		BOX_SOLID_4,
		BOX_SOLID_5,
		BOX_SOLID_6,
		PROP_WALL_LEFT,
		PROP_WALL_RIGHT,
		CHAIN_HIT_LIGHT,
		CHAIN_HIT_SHADOW,
		EXPLOSION_LIGHT_0,
		EXPLOSION_LIGHT_1,
		EXPLOSION_LIGHT_2,
		EXPLOSION_LIGHT_3,
		EXPLOSION_LIGHT_4,
		EXPLOSION_LIGHT_5,
		EXPLOSION_SHADOW_0,
		EXPLOSION_SHADOW_1,
		EXPLOSION_SHADOW_2,
		EXPLOSION_SHADOW_3,
		EXPLOSION_SHADOW_4,
		EXPLOSION_SHADOW_5,
		POWERSLOT,
		LIVES,
		TITLESCREEN,
		BOSS_WARNING,
		BOSS1_ENEMY_SHIP_LIGHT,
		BOSS1_ENEMY_SHIP_SHADOW,
		BOSS1_GRENADE_LIGHT,
		BOSS1_GRENADE_SHADOW,
		BOSS2_LEFTSHIELD,
		BOSS2_RIGHTSHIELD,
		BOSS2_LEFTUPPERARM,
		BOSS2_RIGHTUPPERARM,
		COUNT
	};

	// Player animation indices
	struct PlayerAnimId
	{
		// Scoped enum + implicit conversions
		enum
		{
			LIGHT = 0,
			SHADOW = 1,
			SWITCHING0 = 2,
			SWITCHING1 = 4,
		};
	};

	// Boss images LUT
	enum struct BossImageId
	{
		BOSS1_BODY = 0,
		BOSS1_LEFTARM_ARMED,
		BOSS1_RIGHTARM_ARMED1,
		BOSS1_RIGHTARM_ARMED2,
		BOSS1_LEFTARM_NONARMED,
		BOSS1_RIGHTARM_NONARMED,
		BOSS2_BODY,
		BOSS2_LEFTSHIELD,
		BOSS2_RIGHTSHIELD,
		BOSS2_LEFTARM,
		BOSS2_RIGHTARM,
		BOSS2_LEFTUPPERARM,
		BOSS2_RIGHTUPPERARM,
		BOSS2_LEFTWING,
		BOSS2_LEFTWINGOPENING_1,
		BOSS2_LEFTWINGOPENING_2,
		BOSS2_LEFTWINGOPENING_3,
		BOSS2_LEFTWINGOPENING_4,
		BOSS2_RIGHTWING,
		BOSS2_RIGHTWINGOPENING_1,
		BOSS2_RIGHTWINGOPENING_2,
		BOSS2_RIGHTWINGOPENING_3,
		BOSS2_RIGHTWINGOPENING_4,
		BOSS2_RAILS,
		BOSS2_HITPOINT_LEFT_1,
		BOSS2_HITPOINT_LEFT_2,
		BOSS2_HITPOINT_LEFT_3,
		BOSS2_HITPOINT_LEFT_4,
		BOSS2_HITPOINT_LEFT_5,
		BOSS2_HITPOINT_RIGHT_1,
		BOSS2_HITPOINT_RIGHT_2,
		BOSS2_HITPOINT_RIGHT_3,
		BOSS2_HITPOINT_RIGHT_4,
		BOSS2_HITPOINT_RIGHT_5,
		COUNT
	};

	// Enemy patterns
	// Naming format for generic enemies is Cx_Py = chapter x, pattern y.
	enum struct EnemyPatternId
	{
		DEFAULT = 0,
		BOX,
		PROP,
		// Intro 1
		C1_P1,
		C1_P2,
		C1_P3,
		C1_P4,
		C1_P5,
		C1_P6,
		// Chapter 1
		C1_P7,
		C1_P8,
		C1_P9,
		C1_P10,
		C1_P11,
		C1_P12,
		C1_P13,
		C1_P14,
		C1_P15,
		C1_P16,
		C1_P17,
		C1_P18,
		C1_P19,
		C1_P20,
		C1_P21,
		C1_BOSS,
		C1_BOSS_GRENADE,
		// Intro 2
		C2_P1,
		C2_P2,
		C2_P3,
		C2_P4,
		C2_P5,
		C2_P6,
		C2_P7,
		C2_LEFT_DOOR,
		C2_RIGHT_DOOR,
		// Chapter 2
		C2_P8,
		C2_P9,
		C2_P10,
		C2_P11,
		C2_P12,
		C2_P13,
		C2_P14,
		C2_P15,
		C2_P16,
		C2_P17,
		C2_P18,
		C2_P19,
		C2_P20,
		C2_P21,
		C2_P22,
		C2_P23,
		C2_P24,
		C2_P25,
		C2_P26,
		C2_P27,
		C2_P28,
		C2_P29,
		C2_P30,
		C2_P31,
		C2_P32,
		C2_P33,
		C2_P34,
		C2_P35,
		C2_P36,
		C2_WALL,
		C2_BOSS_WEAK_POINT,
		C2_BOSS_SHIELD,
		COUNT
	};

	// Camera travelling handlers
	enum struct CamTravelingId
	{
		INTRO1 = 0,
		CHAPTER1,
		INTRO2,
		CHAPTER2,
		CHAPTER2_2,
		CHAPTER2_BOSS,
		COUNT
	};

	// Background handlers
	enum struct BgTravelingId
	{
		DEFAULT = 0,
		CHAPTER2_2,
		COUNT
	};

	// Background images LUT
	enum struct BgImageId
	{
		STAGE1_0 = 0,
		STAGE1_1,
		STAGE1_2,
		STAGE1_3,
		STAGE1_4,
		STAGE2_0,
		STAGE2_1,
		STAGE2_2,
		STAGE2_3,
		STAGE2_4,
		STAGE2_5,
		STAGE2_6,
		STAGE2_7,
		COUNT
	};

	// Sounds
	enum struct SoundId
	{
		/* TODO */
		BULLET_FIRE_ENEMY_0 = 0,
		BULLET_FIRE_ENEMY_1,
		BULLET_FIRE_ENEMY_2,
		BULLET_FIRE_ENEMY_HOMING,
		BULLET_FIRE_PLAYER_BULLET,
		BULLET_FIRE_FRAGMENT,
		BULLET_IMPACT,
		ENEMY_EXP_BIG,
		ENEMY_EXP_SMALL,
		ENEMY_LASER_CHARGE,
		ENEMY_LASER_RELEASE,
		PLAYER_CHAIN,
		PLAYER_DEATH,
		PLAYER_SWITCH,
		/* END TODO */
		MENU_ACCEPT,
		MENU_BACK,
		MENU_CURSOR,
		MENU_SELECT,
		MENU_START,
		BOSS_ALERT,
		COUNT
	};

	// Musics
	enum struct MusicId
	{
		CHAPTER1_MAIN = 0,
		CHAPTER1_LOOP,
		CHAPTER1_BOSS,
		CHAPTER2_MAIN,
		CHAPTER2_LOOP,
		COUNT
	};

	static void buildGameLUTs();
	static void freeGameLUTs();

	static const uint16_t* baseImage(BaseImageId entry);
	static const uint16_t* bossImage(BossImageId entry);
	static const uint16_t* backgroundImage(BgImageId entry);
	static Mix_Chunk* sound(SoundId entry);
	static Mix_Music* music(MusicId entry);
	static enemy_pattern enemyPattern(EnemyPatternId entry);
	static camera_traveling camTraveling(CamTravelingId entry);
	static background_traveling bgTraveling(BgTravelingId entry);
	// Index offsetting functions to ease animation and programmatic
	// selection of images. TODO : find a better system maybe ?
	static BaseImageId offset(BaseImageId id, unsigned int add);
	static BossImageId offset(BossImageId id, unsigned int add);
	static BgImageId offset(BgImageId id, unsigned int add);
	static SoundId offset(SoundId id, unsigned int add);
	static MusicId offset(MusicId id, unsigned int add);
	// Shortform functions that amount to lookup(offset(x, a))
	static const uint16_t* baseImage(BaseImageId entry, unsigned int add);
	static const uint16_t* bossImage(BossImageId entry, unsigned int add);
	static const uint16_t* backgroundImage(BgImageId entry, unsigned int add);
	static Mix_Chunk* sound(SoundId entry, unsigned int add);
	static Mix_Music* music(MusicId entry, unsigned int add);

private:
	static const uint16_t* image_entries[static_cast<int>(BaseImageId::COUNT)];
	static const uint16_t* bossImage_entries[static_cast<int>(BossImageId::COUNT)];
	static const uint16_t* bgImage_entries[static_cast<int>(BgImageId::COUNT)];
	static enemy_pattern enemyPattern_entries[static_cast<int>(EnemyPatternId::COUNT)];
	static camera_traveling camTraveling_entries[static_cast<int>(CamTravelingId::COUNT)];
	static background_traveling bgTraveling_entries[static_cast<int>(BgTravelingId::COUNT)];
	static Mix_Chunk* sound_entries[static_cast<int>(SoundId::COUNT)];
	static Mix_Music* music_entries[static_cast<int>(MusicId::COUNT)];
	static bool initialized;
};
