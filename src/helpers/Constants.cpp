// This file was not generated by a tool.
#include "helpers/Constants.hpp"

#include <cassert>
#include <cstdio>

#include <SDL2/SDL_mixer.h>

#include "gfx/titleScreen.h"
#include "gfx/gfx.h"
#include "gfx/bossgfx.h"
#include "gfx/bg.h"
#include "gfx/bossWarning.h"
#include "level/cameras.h"
#include "level/patterns.h"
#include "sfx/list.h"

uint16_t* LUTs::image_entries[static_cast<int>(BaseImageId::COUNT)];
uint16_t* LUTs::bossImage_entries[static_cast<int>(BossImageId::COUNT)];
uint16_t* LUTs::bgImage_entries[static_cast<int>(BgImageId::COUNT)];
Mix_Chunk* LUTs::sound_entries[static_cast<int>(SoundId::COUNT)];
Mix_Music* LUTs::music_entries[static_cast<int>(MusicId::COUNT)];

enemy_pattern LUTs::enemyPattern_entries[static_cast<int>(EnemyPatternId::COUNT)] = { cb_Pattern_null, cb_Pattern_box, cb_Pattern_prop, cb_Pattern_1_1, cb_Pattern_1_2, cb_Pattern_1_3, cb_Pattern_1_4, cb_Pattern_1_5, cb_Pattern_1_6,
	cb_Pattern_1_7, cb_Pattern_1_8, cb_Pattern_1_9, cb_Pattern_1_10, cb_Pattern_1_11, cb_Pattern_1_12, cb_Pattern_1_13, cb_Pattern_1_14, cb_Pattern_1_15, cb_Pattern_1_16,
	cb_Pattern_1_17, cb_Pattern_1_18, cb_Pattern_1_19, cb_Pattern_1_20, cb_Pattern_1_21, cb_Pattern_1_boss, cb_Pattern_1_bossGrenade,
	cb_Pattern_2_1, cb_Pattern_2_2, cb_Pattern_2_3, cb_Pattern_2_4, cb_Pattern_2_5, cb_Pattern_2_6, cb_Pattern_2_7, cb_Pattern_2_leftDoor, cb_Pattern_2_rightDoor,
	cb_Pattern_2_8, cb_Pattern_2_9, cb_Pattern_2_10, cb_Pattern_2_11, cb_Pattern_2_12, cb_Pattern_2_13, cb_Pattern_2_14, cb_Pattern_2_15, cb_Pattern_2_16, cb_Pattern_2_17,
	cb_Pattern_2_18, cb_Pattern_2_19, cb_Pattern_2_20, cb_Pattern_2_21, cb_Pattern_2_22, cb_Pattern_2_23, cb_Pattern_2_24, cb_Pattern_2_25, cb_Pattern_2_26, cb_Pattern_2_27,
	cb_Pattern_2_28, cb_Pattern_2_29, cb_Pattern_2_30, cb_Pattern_2_31, cb_Pattern_2_32, cb_Pattern_2_33, cb_Pattern_2_34, cb_Pattern_2_35, cb_Pattern_2_36, cb_Pattern_2_wall,
	cb_Pattern_2_bossWeakPoint, cb_Pattern_2_bossShield
};

camera_traveling LUTs::camTraveling_entries[static_cast<int>(CamTravelingId::COUNT)] = { cthIntro1, cthChap1, cthIntro2, cthChap2, cthChap2_2, cthChap2_boss };

background_traveling LUTs::bgTraveling_entries[static_cast<int>(BgTravelingId::COUNT)] = { cb_bgHandle_default, cb_bgHandle_2_2 };

bool LUTs::initialized = false;

void LUTs::buildGameLUTs()
{
	assert(!initialized);

	/* GFX */
	// Base images
	image_entries[static_cast<int>(LUTs::BaseImageId::DEFAULT)] = image_null;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_LIGHT)] = image_player_ship_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_SHADOW)] = image_player_ship_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_0_LIGHT)] = image_player_ship_polarityswitch_0_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_0_SHADOW)] = image_player_ship_polarityswitch_0_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_1_LIGHT)] = image_player_ship_polarityswitch_1_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_POLARITYSWITCH_1_SHADOW)] = image_player_ship_polarityswitch_1_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_INVINCIBLE_LIGHT)] = image_player_ship_invincible_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_SHIP_INVINCIBLE_SHADOW)] = image_player_ship_invincible_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_HOMING_BULLET_LIGHT_0)] = image_player_homing_bullet_light_0;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_HOMING_BULLET_LIGHT_1)] = image_player_homing_bullet_light_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_HOMING_BULLET_LIGHT_2)] = image_player_homing_bullet_light_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_HOMING_BULLET_SHADOW_0)] = image_player_homing_bullet_shadow_0;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_HOMING_BULLET_SHADOW_1)] = image_player_homing_bullet_shadow_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_HOMING_BULLET_SHADOW_2)] = image_player_homing_bullet_shadow_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_BULLET_LIGHT)] = image_player_bullet_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_BULLET_SHADOW)] = image_player_bullet_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_0)] = image_player_explosion_0;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_1)] = image_player_explosion_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_2)] = image_player_explosion_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_3)] = image_player_explosion_3;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_4)] = image_player_explosion_4;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_5)] = image_player_explosion_5;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_6)] = image_player_explosion_6;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_7)] = image_player_explosion_7;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_8)] = image_player_explosion_8;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_9)] = image_player_explosion_9;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_10)] = image_player_explosion_10;
	image_entries[static_cast<int>(LUTs::BaseImageId::PLAYER_EXPLOSION_11)] = image_player_explosion_11;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_LIGHT)] = image_enemy_bullet_0_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_0_SHADOW)] = image_enemy_bullet_0_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_LIGHT)] = image_enemy_bullet_1_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_1_SHADOW)] = image_enemy_bullet_1_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_2_LIGHT)] = image_enemy_bullet_2_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_BULLET_2_SHADOW)] = image_enemy_bullet_2_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_HOMING_BULLET_LIGHT)] = image_enemy_homing_bullet_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_HOMING_BULLET_SHADOW)] = image_enemy_homing_bullet_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_LASER_LIGHT)] = image_enemy_laser_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_LASER_SHADOW)] = image_enemy_laser_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_GENERATOR_LIGHT)] = image_enemy_generator_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_GENERATOR_SHADOW)] = image_enemy_generator_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_WALL_LIGHT)] = image_enemy_wall_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_WALL_SHADOW)] = image_enemy_wall_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_0_LIGHT)] = image_enemy_ship_0_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_0_SHADOW)] = image_enemy_ship_0_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_1_LIGHT)] = image_enemy_ship_1_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_1_SHADOW)] = image_enemy_ship_1_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_2_LIGHT)] = image_enemy_ship_2_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_2_SHADOW)] = image_enemy_ship_2_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_3_LIGHT)] = image_enemy_ship_3_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_3_SHADOW)] = image_enemy_ship_3_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_4_LIGHT)] = image_enemy_ship_4_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_4_SHADOW)] = image_enemy_ship_4_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_4_LIGHTBALL)] = image_enemy_ship_4_lightball;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_4_SHADOWBALL)] = image_enemy_ship_4_shadowball;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_5_LIGHT)] = image_enemy_ship_5_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_5_SHADOW)] = image_enemy_ship_5_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_6_LIGHT)] = image_enemy_ship_6_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::ENEMY_SHIP_6_SHADOW)] = image_enemy_ship_6_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::DOOR_LEFT)] = image_door_left;
	image_entries[static_cast<int>(LUTs::BaseImageId::DOOR_RIGHT)] = image_door_right;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_LIGHT_1)] = image_box_light_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_LIGHT_2)] = image_box_light_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_LIGHT_3)] = image_box_light_3;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_LIGHT_4)] = image_box_light_4;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_LIGHT_5)] = image_box_light_5;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_LIGHT_6)] = image_box_light_6;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SHADOW_1)] = image_box_shadow_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SHADOW_2)] = image_box_shadow_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SHADOW_3)] = image_box_shadow_3;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SHADOW_4)] = image_box_shadow_4;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SHADOW_5)] = image_box_shadow_5;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SHADOW_6)] = image_box_shadow_6;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SOLID_1)] = image_box_solid_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SOLID_2)] = image_box_solid_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SOLID_3)] = image_box_solid_3;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SOLID_4)] = image_box_solid_4;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SOLID_5)] = image_box_solid_5;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOX_SOLID_6)] = image_box_solid_6;
	image_entries[static_cast<int>(LUTs::BaseImageId::PROP_WALL_LEFT)] = image_prop_wall_left;
	image_entries[static_cast<int>(LUTs::BaseImageId::PROP_WALL_RIGHT)] = image_prop_wall_right;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS1_ENEMY_SHIP_LIGHT)] = image_boss1_enemy_ship_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS1_ENEMY_SHIP_SHADOW)] = image_boss1_enemy_ship_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS1_GRENADE_LIGHT)] = image_boss1_grenade_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS1_GRENADE_SHADOW)] = image_boss1_grenade_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS2_LEFTSHIELD)] = image_boss2_leftShield;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS2_RIGHTSHIELD)] = image_boss2_rightShield;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS2_LEFTUPPERARM)] = image_boss2_leftUpperArm;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS2_RIGHTUPPERARM)] = image_boss2_rightUpperArm;
	image_entries[static_cast<int>(LUTs::BaseImageId::CHAIN_HIT_LIGHT)] = image_chain_hit_light;
	image_entries[static_cast<int>(LUTs::BaseImageId::CHAIN_HIT_SHADOW)] = image_chain_hit_shadow;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_LIGHT_0)] = image_explosion_light_0;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_LIGHT_1)] = image_explosion_light_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_LIGHT_2)] = image_explosion_light_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_LIGHT_3)] = image_explosion_light_3;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_LIGHT_4)] = image_explosion_light_4;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_LIGHT_5)] = image_explosion_light_5;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_SHADOW_0)] = image_explosion_shadow_0;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_SHADOW_1)] = image_explosion_shadow_1;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_SHADOW_2)] = image_explosion_shadow_2;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_SHADOW_3)] = image_explosion_shadow_3;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_SHADOW_4)] = image_explosion_shadow_4;
	image_entries[static_cast<int>(LUTs::BaseImageId::EXPLOSION_SHADOW_5)] = image_explosion_shadow_5;
	image_entries[static_cast<int>(LUTs::BaseImageId::POWERSLOT)] = image_powerslot;
	image_entries[static_cast<int>(LUTs::BaseImageId::LIVES)] = image_lives;
	image_entries[static_cast<int>(LUTs::BaseImageId::BOSS_WARNING)] = image_bossWarning;
	image_entries[static_cast<int>(LUTs::BaseImageId::TITLESCREEN)] = image_titleScreen;
	
	// Boss images
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS1_BODY)] = bossImage_1_body;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS1_LEFTARM_ARMED)] = bossImage_1_leftarm_armed;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS1_RIGHTARM_ARMED1)] = bossImage_1_rightarm_armed1;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS1_RIGHTARM_ARMED2)] = bossImage_1_rightarm_armed2;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS1_LEFTARM_NONARMED)] = bossImage_1_leftarm_nonarmed;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS1_RIGHTARM_NONARMED)] = bossImage_1_rightarm_nonarmed;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_BODY)] = bossImage_2_body;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTSHIELD)] = bossImage_2_leftShield;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTSHIELD)] = bossImage_2_rightShield;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTARM)] = bossImage_2_leftArm;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTARM)] = bossImage_2_rightArm;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTUPPERARM)] = bossImage_2_leftUpperArm;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTUPPERARM)] = bossImage_2_rightUpperArm;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTWING)] = bossImage_2_leftWing;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTWINGOPENING_1)] = bossImage_2_leftWingOpening_1;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTWINGOPENING_2)] = bossImage_2_leftWingOpening_2;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTWINGOPENING_3)] = bossImage_2_leftWingOpening_3;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_LEFTWINGOPENING_4)] = bossImage_2_leftWingOpening_4;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTWING)] = bossImage_2_rightWing;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTWINGOPENING_1)] = bossImage_2_rightWingOpening_1;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTWINGOPENING_2)] = bossImage_2_rightWingOpening_2;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTWINGOPENING_3)] = bossImage_2_rightWingOpening_3;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RIGHTWINGOPENING_4)] = bossImage_2_rightWingOpening_4;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_RAILS)] = bossImage_2_rails;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_LEFT_1)] = bossImage_2_hitpoint_left_1;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_LEFT_2)] = bossImage_2_hitpoint_left_2;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_LEFT_3)] = bossImage_2_hitpoint_left_3;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_LEFT_4)] = bossImage_2_hitpoint_left_4;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_LEFT_5)] = bossImage_2_hitpoint_left_5;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_RIGHT_1)] = bossImage_2_hitpoint_right_1;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_RIGHT_2)] = bossImage_2_hitpoint_right_2;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_RIGHT_3)] = bossImage_2_hitpoint_right_3;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_RIGHT_4)] = bossImage_2_hitpoint_right_4;
	bossImage_entries[static_cast<int>(LUTs::BossImageId::BOSS2_HITPOINT_RIGHT_5)] = bossImage_2_hitpoint_right_5;

	// Background images
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE1_0)] = image_bg1_0;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE1_1)] = image_bg1_1;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE1_2)] = image_bg1_2;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE1_3)] = image_bg1_3;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE1_4)] = image_bg1_4;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_0)] = image_bg2_0;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_1)] = image_bg2_1;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_2)] = image_bg2_2;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_3)] = image_bg2_3;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_4)] = image_bg2_4;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_5)] = image_bg2_5;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_6)] = image_bg2_6;
	bgImage_entries[static_cast<int>(LUTs::BgImageId::STAGE2_7)] = image_bg2_7;
	
	/* SFX */
	// Sounds
	for (int i = 0; i < static_cast<int>(SoundId::COUNT); i++)
	{
		printf("Loading sound '%s' ... ", sfxList[i]);
		sound_entries[i] = Mix_LoadWAV(sfxList[i]);
		printf("done\n");
	}
	// Background musics
	for (int i = 0; i < static_cast<int>(MusicId::COUNT); i++)
	{
		printf("Loading music '%s' ... ", musicList[i]);
		music_entries[i] = Mix_LoadMUS(musicList[i]);
		printf("done\n");
	}

	initialized = true;
}

void LUTs::freeGameLUTs()
{
	assert(initialized);

	// Sounds
	for (int i = 0; i < static_cast<int>(SoundId::COUNT); i++)
		Mix_FreeChunk(sound_entries[i]);
	// Background musics
	for (int i = 0; i < static_cast<int>(MusicId::COUNT); i++)
		Mix_FreeMusic(music_entries[i]);

	initialized = false;
}

// LUT lookup functions

uint16_t* LUTs::baseImage(BaseImageId entry)
{
	assert(initialized);
	return image_entries[static_cast<int>(entry)];
}

uint16_t* LUTs::bossImage(BossImageId entry)
{
	assert(initialized);
	return bossImage_entries[static_cast<int>(entry)];
}

uint16_t* LUTs::backgroundImage(BgImageId entry)
{
	assert(initialized);
	return bgImage_entries[static_cast<int>(entry)];
}

Mix_Chunk* LUTs::sound(SoundId entry)
{
	assert(initialized);
	return sound_entries[static_cast<int>(entry)];
}

Mix_Music* LUTs::music(MusicId entry)
{
	assert(initialized);
	return music_entries[static_cast<int>(entry)];
}

enemy_pattern LUTs::enemyPattern(EnemyPatternId entry)
{
	assert(initialized);
	return enemyPattern_entries[static_cast<int>(entry)];
}

camera_traveling LUTs::camTraveling(CamTravelingId entry)
{
	assert(initialized);
	return camTraveling_entries[static_cast<int>(entry)];
}

background_traveling LUTs::bgTraveling(BgTravelingId entry)
{
	assert(initialized);
	return bgTraveling_entries[static_cast<int>(entry)];
}

// LUT lookup with offset

uint16_t* LUTs::baseImage(BaseImageId entry, unsigned int add)
{
	assert(initialized);
	assert(static_cast<int>(entry) + add < static_cast<int>(BaseImageId::COUNT));
	return image_entries[static_cast<int>(entry) + add];
}

uint16_t* LUTs::bossImage(BossImageId entry, unsigned int add)
{
	assert(initialized);
	assert(static_cast<int>(entry) + add < static_cast<int>(BossImageId::COUNT));
	return bossImage_entries[static_cast<int>(entry) + add];
}

uint16_t* LUTs::backgroundImage(BgImageId entry, unsigned int add)
{
	assert(initialized);
	assert(static_cast<int>(entry) + add < static_cast<int>(BgImageId::COUNT));
	return bgImage_entries[static_cast<int>(entry) + add];
}

Mix_Chunk* LUTs::sound(SoundId entry, unsigned int add)
{
	assert(initialized);
	assert(static_cast<int>(entry) + add < static_cast<int>(SoundId::COUNT));
	return sound_entries[static_cast<int>(entry) + add];
}

Mix_Music* LUTs::music(MusicId entry, unsigned int add)
{
	assert(initialized);
	assert(static_cast<int>(entry) + add < static_cast<int>(MusicId::COUNT));
	return music_entries[static_cast<int>(entry) + add];
}
