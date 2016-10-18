#ifndef INCLUDE_COMMON
#define INCLUDE_COMMON

#define M_PI 3.1415926535897932384626433832795

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <stack>
#include <SDL_mixer.h>

#include "n2DLib.h"

#define UNUSED(var) (void)var

#define FPS 60

typedef int KeyEvent;

typedef int Fixed;

// KeyEvent
#define KDOWN(x) (x & 1)
#define KLEFT(x) (x & 2)
#define KRIGHT(x) (x & 4)
#define KUP(x) (x & 8)
#define KFIRE(x) (x & 16)
#define KPOLARITY(x) (x & 32)
#define KPOWER(x) (x & 64)
#define KQUIT(x) (x & 128)
#define KPAUSE(x) (x & 256)

extern KeyEvent getk();

// Bullets

#define LIGHT 0
#define SHADOW 1
#define SWITCHING0 2
#define SWITCHING1 4

#define MAX_BULLET 1000
#define MAX_HOMING 100
#define HOMING_TRAILING 15
#define MAX_FRAGMENT 12
#define MAX_LASER 12
#define FRAGMENT_TRAILING 7
// 12*10
#define MAX_POWER 120

#define LASER_SPEED 6
#define LASER_THICKNESS 25

#define MAX_ENEMY 500

#define MAX_DISPLAYABLE 2000

class Entity
{
public:
	Entity();
	~Entity();
	virtual Fixed getx();
	virtual Fixed gety();
	void setx(Fixed);
	void sety(Fixed);
	void addx(Fixed);
	void subx(Fixed);
	void addy(Fixed);
	void suby(Fixed);
	bool isActive();
	void activate();
	void deactivate();
	int getCamRel();
	Fixed angleToEntity(Entity*);
	Fixed angleToXY(Fixed x, Fixed y);
	Fixed distance2ToEntity(Entity*);
	Fixed distance2ToXY(Fixed x, Fixed y);
	bool isBoss;
	bool isEnemy;
	bool waitFrames(int frames);
protected:
	bool active;
	int camRel;
	Fixed x, y;
};

class Joint
{
public:
	Joint();
	~Joint();
	void activate(Entity *target, int targetX, int targetY, int jointX, int jointY, int jointCX, int jointCY, unsigned short *timg, unsigned short *jimg);
	void activate(Entity *target, int targetX, int targetY, int targetCX, int targetCY, int jointX, int jointY, int jointCX, int jointCY, unsigned short *timg, unsigned short *jimg);
	Fixed getx();
	Fixed gety();
	Entity *target;
	unsigned short *timg; // the target's image
	unsigned short *jimg; // the jointed image
	int targetX, targetY; // the connection point on the target
	int targetCX, targetCY; // the target's rotation center if relevant
	int jointX, jointY; // the connection point on the jointed image
	int jointCX, jointCY; // the jointed image's rotation center ; pass -1 to default to the center
};

typedef struct
{
	int absX, absY;
	int relX, relY;
} Camera;

typedef void (*cameraTravelling)(Camera*);

class DrawingCandidate
{
public:
	DrawingCandidate();
	~DrawingCandidate();
	void activate(unsigned short *img, Rect *pos, bool flash, int camRelation);
	void activate(unsigned short *img, Rect *pos, Rect *center, Fixed angle, bool flash, int camRelation);
	void deactivate();
	void draw();
private:
	Rect pos, center;
	bool rotates, centered, active, flash;
	// How the camera affects the object's display
	// See the enum lower in the file
	int camRel;
	Fixed angle;
	unsigned short *img;
};

class DrawingCandidates
{
public:
	DrawingCandidates();
	~DrawingCandidates();
	void add(unsigned short *img, Rect *pos, bool flash, int camRelation);
	void add(unsigned short *img, Rect *pos, Rect *center, Fixed angle, bool flash, int camRelation);
	void flush();
	void loadCameraPath(int id);
	Camera cam;
private:
	DrawingCandidate data[MAX_DISPLAYABLE];
	int candidatesCount;
	cameraTravelling cameraPath;
};

// Both enemies and player can fire them
class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();
	Rect* makeRect();
	void activate(Fixed x, Fixed y, Fixed a, Fixed r, int imageID, bool polarity, bool hurtsPlayer, int camRel);
	bool getPolarity();
	bool hurtsPlayer();
	bool handle();
	void draw();
	Fixed getx();
	Fixed gety();
	// speed
	Fixed dx, dy;
	unsigned short *img;
protected:
	bool polarity;
	bool hurtPlayer;
};

class Player;

// Only enemies can fire them
// So hurtPlayer is always true
class Homing : public Bullet
{
public:
	Homing();
	~Homing();
	void activate(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity);
	bool handle();
	void draw();
	Player* target;
private:
	Fixed previousX[HOMING_TRAILING];
	Fixed previousY[HOMING_TRAILING];
	Fixed angle;
	int aimTimer;
};

class Enemy;
class BossEnemy;

// Both enemies and player can fire them
class PowerFragment : public Bullet
{
public:
	PowerFragment();
	~PowerFragment();
	void activate(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity, bool hurtsPlayer);
	bool handle();
	void draw();
	// Polar coordinates of speed
	Fixed speed;
	Fixed angle;
	Enemy *targetE;
	Player *targetP;
	BossEnemy *targetB;
private:
	Fixed previousX[FRAGMENT_TRAILING];
	Fixed previousY[FRAGMENT_TRAILING];
	// Determine if the initial angle has been reached once already
	bool hasReachedAngle;
	int skipPositionRecord;
};

// Only enemies can fire them
class Laser : public Bullet
{
public:
	Laser();
	~Laser();
	void activate(Enemy *origin, bool polarity, Fixed angOffset);
	void handle();
	void draw();
	Rect* getVector();
	void getSides(Rect *side1, Rect *side2);
	int getAmplitude();
	void setAmplitude(int amp);
	Fixed angle;
	Enemy *origin;
private:
	// Lasers are not immediate, they grow
	Fixed amplitude;
	Fixed angleOffset;
};

#define SCORE_HIT 20
#define SCORE_HIT_OP 40
#define SCORE_ABSORB 100

// BulletArray
class BulletArray
{
public:
	BulletArray();
	~BulletArray();
	void handle();
	void add(Fixed x, Fixed y, Fixed a, Fixed r, int imageID, bool polarity, bool hurtsPlayer, int camRel);
	void add_fragment(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity, bool hurtsPlayer);
	void add_homing(Fixed x, Fixed y, Fixed initialAngle, Player *target, bool polarity);
	void fire_laser(Enemy *origin, bool polarity, Fixed angleOffset);
	void stop_laser(Enemy *origin);
	void clear();
private:
	void deactivate(int offset, bool playSound);
	void deactivate_fragment(int offset);
	void deactivate_homing(int offset);
	Bullet data[MAX_BULLET];
	PowerFragment data_fragment[MAX_FRAGMENT];
	Homing data_homing[MAX_HOMING];
	Laser data_laser[MAX_LASER];
	// keep track of current bullet ...
	int bulletCount;
	// ... power fragment ...
	int fragmentCount;
	// ... homing ...
	int homingCount;
	// ... and laser
	int laserCount;
};

// Player
class Player : public Entity
{
public:
	Player();
	~Player();
	void reset();
	void handle(KeyEvent k);
	bool getPolarity();
	void switchPolarity();
	void hurt();
	int getLives();
	void setLives(int);
	bool isHurtable();
	// Ship images, light and shadow
	// First two are "normal" ship
	// Other four are polarity transition animation frames (frame 0 light/shadow, frame 1 light/shadow)
	// images have same dimensions
	unsigned short *img[6];
	int deathCounter;
private:
	int isSwitchingPolarity;
	// keep firing at a reasonable rate
	int fireDelay;
	// True is light, false is shadow
	bool polarity;
	// Key repeat handlers
	bool fireRepeat;
	bool polarityRepeat;
	int lives;
} ;

// Enemy
// For real this time

typedef void (*EnemyCallback)(Enemy*);

class Enemy : public Entity
{
	friend Joint;
public:
	Enemy();
	~Enemy();
	void handle();
	void activate(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, int type);
	bool damage(bool polarity, int amount);
	void joint(Entity *target, int targetX, int targetY, int jointX, int jointY, int jointCX, int jointCY, unsigned short *timg, unsigned short *jimg, bool diesWithJoint);
	void joint(Entity *target, int targetX, int targetY, int targetCX, int targetCY, int jointX, int jointY, int jointCX, int jointCY, unsigned short *timg, unsigned short *jimg, bool diesWithJoint);
	bool collide(Fixed x, Fixed y, Fixed cx = 0, Fixed cy = 0);
	Fixed getRotation();
	void setRotation(Fixed angle);
	bool getPolarity();
	int getWaveIndex();
	bool isGhost();
	bool isProp();
	bool isDamageable();
	Fixed rawx();
	Fixed rawy();
	Fixed getx();
	Fixed gety();
	// x, y on-screen
	// Enemy image
	bool visible;
	unsigned short *img;
	bool diedThisFrame;
	//
	// Used by patterns
	//
	Fixed rotationAngle;
	// What bullets does the enemy fire
	bool polarity;
	// The position of the enemy in the wave
	int waveIndex;
	// Internal data for free use by the behaviour code
	int internal[6];
	// Arrays to use with n2DLib's interpolatePathFloat
	float *ax, *ay;
	int *at;
	void setAX(int, ...);
	void setAY(int, ...);
	void setAT(int, ...);
	bool damageable;
	int HP, maxHP;
	// Box routine
	void beAbox();
	// Prop routine
	void beAprop();

	// Used to constraint an enemy to another
	Joint *jointObj;
	bool isJointed;
private:
	int shipImgID;
	// Ghost enemies have no interaction with anything
	bool ghost;
	// Props follow the absolute camera
	bool prop;
	// Does the enemy use rotation (achieved in the pattern)
	bool hasRotation;
	// The enemy's behaviour
	//int callback;
	EnemyCallback callback;

	//bool isJointed;
	bool diesWithJoint;
	//Entity *jointedTo;
	//Fixed jointX;
	//Fixed jointY;
	// How many bullets does the enemy fireback when dying
	int fireback;
	// Used for complex spawning schemes
	// Is 0 when the enemy can be deactivated by leaving the screen
	int spawned;
	// If the enemy has just been hit and its sprite needs to flash
	bool flash;
};

/* Bosses' data format
 * - Number of patterns
 * - For each pattern :
 *   - Amount of HP
 *   - Collision callback ID (called to test collision between the boss and bullets, it can change depending on the boss's state)
 * - Array of every needed images
 * 
 * The callback is supposed to handle all patterns by testing the amount of HP.
 * Exceptionally, drawing should be taken care of in the callback using drawing candidates since bosses have complex graphical behaviors.
 */

// Initialization callback, handling pre-battle cinematics and such
// Inputs :
// - boss enemy to initialize
// Output : none
typedef void (*boss_icb)(BossEnemy*);
// Behaviour callback, handling the boss's patterns during battle
// Inputs :
// - boss enemy to handle
// - player object for aiming
// - bullet array for firing bullets
// Output : none
typedef void (*boss_cb)(BossEnemy*);
// Collision callback, handling whether or not the boss is being hit by a bullet
// Inputs :
// - boss enemy to test
// - bullet object
// - amount of damage to deal if the boss is hit
// Output : amount of damage dealt, 0 for no collision, negative for collision with no damage
typedef int (*boss_ccb)(BossEnemy*, Bullet*, int);
// Player collision callback, handling whether or not the player destroys (him|her)self on the boss
// Inputs :
// - boss enemy to test
// - player object
// Output : boolean
typedef bool (*boss_pccb)(BossEnemy*);
// Distance callback, handling distance calcualtion for when the boss is being aimed by power fragments
// Inputs :
// - boss enemy to test
// - power fragment object
// Output : distance to boss
typedef Fixed (*boss_dcb)(BossEnemy*, PowerFragment*);
// Angle callback, handling angle calculation for when the boss is being aimed by power fragments
// Inputs :
// - boss enemy to test
// - power fragment object
// Output : angle to boss
typedef Fixed (*boss_acb)(BossEnemy*, PowerFragment*);

typedef struct
{
	int id;
	int HPperBar;
	boss_cb callback;
	// Every following member contains this amount of data, because it's pattern-specific
	int patternsNb;
	// Self-explanatory
	int *HPperPattern;
	int *timeoutPerPattern;
	boss_icb *initCallbacks;
	boss_ccb *collisionCallbacks;
	boss_dcb *distanceCallbacks;
	boss_acb *angleCallbacks;
	boss_pccb *playerCollisionCallbacks;
} BossData;

// The really big bad one
class BossEnemy : public Entity
{
public:
	BossEnemy();
	~BossEnemy();
	void activate(BossData *d); // drawing is taken care of by the callback code
	int handle();
	void damage(int amount);
	bool isHurtable();
	void makeHurtable();
	void setInternal(int offset, int value);
	void incInternal(int offset);
	void decInternal(int offset);
	int getInternal(int offset);
	int getTimeout();
	Fixed getDistance(PowerFragment*);
	Fixed getAngle(PowerFragment*);
	int id;
	int body; // image LUT offset to body image
	Fixed angle;
	bool flash;
	int HP;
	int maxHP;
	int HPperBar;
	int currentPattern;
	int lastPattern;
	int *HPperPattern;
	int *timeoutPerPattern;
	int patternsNb;
	bool readyToGo;
	bool initCallbackCalled;
	boss_icb *initCallbacks;
	boss_cb callback;
	boss_ccb *collisionCallbacks;
	boss_dcb *distanceCallbacks;
	boss_acb *angleCallbacks;
	boss_pccb *playerCollisionCallbacks;
	// Used to save attached enemies
	std::vector<Enemy*> attachedEnemies;
private:
	int remainingTime;
	bool *hurtable;
	// LOTS of internal registers
	int internal[32];
};

// Used to hold information on killed enemies in order to get the position for ChainNotifs
class DestroyedEnemies
{
public:
	DestroyedEnemies();
	~DestroyedEnemies();
	void activate(Enemy *origin, int offset);
	void clear();
	int x[MAX_ENEMY];
	int y[MAX_ENEMY];
	bool relevant[MAX_ENEMY];
};

// Explosion animations
class ExplosionAnim : public Entity
{
public:
	ExplosionAnim();
	~ExplosionAnim();
	void activate(int, int, bool);
	void handle();
private:
	int counter;
	bool polarity;
};

class EnemiesArray
{
public:
	EnemiesArray();
	~EnemiesArray();
	Enemy* add(int x, int y, int HP, int shipImgID, int callbackID, int waveIndex, bool polarity, bool hasRotation, int firebackAmount, bool ghost, int type);
	void handle();
	void handleExplosions();
	void resetEnemyCounter();
	void destroyAllEnemies();
	bool enemiesKilled();
	Enemy data[MAX_ENEMY];
	DestroyedEnemies deadEnemies;
private:
	ExplosionAnim explosionsAnims[MAX_ENEMY];
	int currentExplosion;
	int currentEnemy;
};

// Particles
// Because they are pretty
#define MAX_PARTICLE 1024
#define PARTICLE_RADIUS 4
class Particles
{
public:
	Particles();
	~Particles();
	void add(Fixed x, Fixed y, Fixed angle, Fixed r, bool polarity, int lifetime);
	void pulse(Fixed x, Fixed y, bool polarity);
	void handle();
private:
	Fixed x[MAX_PARTICLE];
	Fixed y[MAX_PARTICLE];
	Fixed dx[MAX_PARTICLE];
	Fixed dy[MAX_PARTICLE];
	int time[MAX_PARTICLE];
	int dt[MAX_PARTICLE];
	bool polarity[MAX_PARTICLE];
	int counter;
};

class BackgroundScroller;

// Update callback
typedef void (*backgroundHandle)(BackgroundScroller*);

// Background scrolling handler
class BackgroundScroller
{
public:
	BackgroundScroller(unsigned short *bg, Fixed _x, Fixed _y, Fixed sscale, Fixed dscale, int bgHandleID);
	~BackgroundScroller();
	void draw();
	void update();
	Fixed x;
	Fixed y;
	Fixed dx;
	Fixed dy;
	unsigned short *img;
	Fixed w;
	Fixed h;
	unsigned short colorKey;
	Fixed scrollScale;
	Fixed displayScale;
	backgroundHandle handle;
};

// Text notifications of score-chaining
class ChainNotif : public Entity
{
public:
	ChainNotif();
	~ChainNotif();
	void activate(int x, int y, int value);
	void handle();
private:
	Fixed backupX;
	// Number to display
	int value;
	bool maxChain;
	// Stay visible for a certain amount of frames
	int untilDeath;
	int counter;
};

/* UTILS */

// General purpose menu handler
typedef struct
{
	char *name;
	void *value;
	char **labels;
	int labelsNb;
} MenuItem;

class Menu
{
public:
	Menu(int num, ...);
	~Menu();
	// Returns the index of the chosen option, or -1 if the menu was exitted
	int run();
private:
	std::vector<char*> options;
	std::vector<void*> values;
	std::vector< std::vector<char*> > labels;
	int num;
	int maxOptionWidth;
	int width;
	int height;
	// Returns 0 if the choice was a value modifier, else 1
	int apply(int choice);
};

// SFX
// Sounds
enum
{
	/* TODO */
	SD_BULLET_FIRE_ENEMY_0,
	SD_BULLET_FIRE_ENEMY_1,
	SD_BULLET_FIRE_ENEMY_2,
	SD_BULLET_FIRE_ENEMY_HOMING,
	SD_BULLET_FIRE_PLAYER_BULLET,
	SD_BULLET_FIRE_FRAGMENT,
	SD_BULLET_IMPACT,
	SD_ENEMY_EXP_BIG,
	SD_ENEMY_EXP_SMALL,
	SD_ENEMY_LASER_CHARGE,
	SD_ENEMY_LASER_RELEASE,
	SD_PLAYER_CHAIN,
	SD_PLAYER_DEATH,
	SD_PLAYER_SWITCH,
	/* END TODO */
	SD_MENU_ACCEPT,
	SD_MENU_BACK,
	SD_MENU_CURSOR,
	SD_MENU_SELECT,
	SD_MENU_START,
	SD_BOSS_ALERT,
	NB_SOUNDS
};
// Musics
enum
{
	BGM_CHAPTER1_MAIN,
	BGM_CHAPTER1_LOOP,
	BGM_CHAPTER1_BOSS,
	BGM_CHAPTER2_MAIN,
	BGM_CHAPTER2_LOOP,
	NB_MUSICS
};

// Sound handler
class SoundHandle
{
public:
	SoundHandle();
	~SoundHandle();
	void update();
	int quickPlaySFX(Mix_Chunk *sfx);
	int playBgMusic(Mix_Music *mainM, Mix_Music *loopM);
	void setPausedBgMusic(bool paused);
	int fadeOutMusic(int ms, void(*callback)());
	int stopBgMusic();
	bool musicPlaying();
private:
	static void hook_branch();
	Mix_Music *loop;
	static bool launchLoop;
};

class Level
{
public:
	static void init(int n);
	static void reinit(int n);
	static void deinit();
	static void advanceLevel();
	static void (*executeIntro)(); // re-bindable
	static void updateBg();
	static int phase;
	static bool fightingBoss;
	static bool gameEnded;
	static int counter, timer, waveIndex, waveTimer; // level counter, pause timer (for wait commands), wave index, wave timer
	// Important global entities
	static Player *p;
	static BulletArray *bArray;
	static EnemiesArray *enemiesArray;
	static BossEnemy *be;
	static SoundHandle *soundSystem;
private:
	static int chapterNum;
	static Enemy *currentWaveEnemies[MAX_ENEMY]; // current wave's set of enemies (for 'killed' commands)
	static int skipCommand; // skip offset (for skip commands)
	static bool continueParsing;
	static std::vector<BackgroundScroller> bgStack; // needs an iterator
	static std::stack<unsigned int> commandStack; // command backtracking (for repeat commands)
	static int currentW, currentH;
	static void intro1();
	static void intro2();
	static void intro3();
	static void intro4();
	static void intro5();
};

// Level streams
// x, y, HP, image ID, callback ID, polarity, has rotation ?, fireback amount, type ?
#define enemy(x, y, HP, iID, cbID, p, hR, f, type) x, y, HP, iID, cbID, p, hR, f, type

// IMPORTANT
// PROPS ARE NOT CONSIDERED PARTS OF A WAVE, THUS THEIR waveIndex VARIABLE MUST NOT BE USED

// Game phases
enum
{
	PHASE_GAME,
	PHASE_TRANSITION,
	PHASE_BOSSCINEMATIC,
	PHASE_BOSSFIGHT,
	PHASE_BOSSEXPLODEINIT,
	PHASE_BOSSEXPLODE,
	PHASE_RESULTS,
};

// Special values
#define LVLSTR_CHAPTEREND -3
#define LVLSTR_END -2
#define LVLSTR_CMD -1

// Commands
enum
{
	LVLSTR_NEWWAVE,
	LVLSTR_NEWCAMERA,
	LVLSTR_WAIT,
	LVLSTR_WAITABS,
	LVLSTR_WAITCAMERA,
	LVLSTR_SKIP,
	LVLSTR_SKIPABS,
	LVLSTR_REPEATABS,
	LVLSTR_KILLED,
	LVLSTR_BACKGROUND,
	LVLSTR_RESETBG,
	LVLSTR_MUSIC,
	LVLSTR_REINIT,
	LVLSTR_CHAPTER,
	LVLSTR_JOINT,
	LVLSTR_BOSS,
	LVLSTR_BKPT
};

// Starts a new wave
#define cmd_newWave LVLSTR_CMD, 1, LVLSTR_NEWWAVE
// Loads a new camera path
#define cmd_newCameraPath(n) LVLSTR_CMD, 2, LVLSTR_NEWCAMERA, n
// Waits a given amount of frames
#define cmd_wait(x) LVLSTR_CMD, 2, LVLSTR_WAIT, x
// Waits until G_gpTimer has a certain value
#define cmd_waitAbs(x) LVLSTR_CMD, 2, LVLSTR_WAITABS, x
// Waits until the ABSOLUTE camera reaches this point in the level
#define cmd_waitCamera(x, y) LVLSTR_CMD, 3, LVLSTR_WAITCAMERA, x, y
// Skips the nb next commands if G_waveTimer is greater than time
#define cmd_skip(time, nb) LVLSTR_CMD, 3, LVLSTR_SKIP, time, nb
// Skips the nb next commands if G_gpTimer is greater than time
#define cmd_skipAbs(time, nb) LVLSTR_CMD, 3, LVLSTR_SKIPABS, time, nb
// Go back nb commands before the current position (not including this one) if G_gpTimer is lower than time
#define cmd_repeatAbs(time, nb) LVLSTR_CMD, 3, LVLSTR_REPEATABS, time, nb
// Waits for all enemies to be killed (includes props)
#define cmd_killed LVLSTR_CMD, 1, LVLSTR_KILLED
// Pushes the given background to the background rendering stack
#define cmd_useBackground(id, x, y, scrollScale, displayScale, handleID) LVLSTR_CMD, 7, LVLSTR_BACKGROUND, id, x, y, scrollScale, displayScale, handleID
// Empties the background rendering stack
#define cmd_resetBackgrounds LVLSTR_CMD, 1, LVLSTR_RESETBG
// Plays the chapter's music
#define cmd_playMusic(mainM, loopM) LVLSTR_CMD, 3, LVLSTR_MUSIC, mainM, loopM
// Loads a new chapter
#define cmd_newChapter(n) LVLSTR_CMD, 2, LVLSTR_REINIT, n
// Starts a new chapter (call it after its introduction is done)
#define cmd_startChapter() LVLSTR_CMD, 1, LVLSTR_CHAPTER
// Joints an enemy to another enemy (given their waveIndex numbers) at the point (x, y)
// If the enemy is joint-dependant, it dies when the anchor enemy dies.
#define cmd_joint(which, to, x, y, jointDependant) LVLSTR_CMD, 6, LVLSTR_JOINT, which, to, x, y, jointDependant
// Breakpoint
#define cmd_bkpt LVLSTR_CMD, 1, LVLSTR_BKPT
// Fight the boss of chapter n with music playing
#define cmd_fightBoss(n) LVLSTR_CMD, 2, LVLSTR_BOSS, n

// LUT-related

// Base images LUT
enum
{
	image_LUT_null,
	image_LUT_player_ship_light,
	image_LUT_player_ship_shadow,
	image_LUT_player_ship_polarityswitch_0_light,
	image_LUT_player_ship_polarityswitch_0_shadow,
	image_LUT_player_ship_polarityswitch_1_light,
	image_LUT_player_ship_polarityswitch_1_shadow,
	image_LUT_player_ship_invincible_light,
	image_LUT_player_ship_invincible_shadow,
	image_LUT_player_bullet_light,
	image_LUT_player_bullet_shadow,
	image_LUT_player_homing_bullet_light_0,
	image_LUT_player_homing_bullet_light_1,
	image_LUT_player_homing_bullet_light_2,
	image_LUT_player_homing_bullet_shadow_0,
	image_LUT_player_homing_bullet_shadow_1,
	image_LUT_player_homing_bullet_shadow_2,
	image_LUT_player_explosion_0,
	image_LUT_player_explosion_1,
	image_LUT_player_explosion_2,
	image_LUT_player_explosion_3,
	image_LUT_player_explosion_4,
	image_LUT_player_explosion_5,
	image_LUT_player_explosion_6,
	image_LUT_player_explosion_7,
	image_LUT_player_explosion_8,
	image_LUT_player_explosion_9,
	image_LUT_player_explosion_10,
	image_LUT_player_explosion_11,
	image_LUT_enemy_bullet_0_light,
	image_LUT_enemy_bullet_0_shadow,
	image_LUT_enemy_bullet_1_light,
	image_LUT_enemy_bullet_1_shadow,
	image_LUT_enemy_bullet_2_light,
	image_LUT_enemy_bullet_2_shadow,
	image_LUT_enemy_homing_bullet_light,
	image_LUT_enemy_homing_bullet_shadow,
	image_LUT_enemy_laser_light,
	image_LUT_enemy_laser_shadow,
	image_LUT_enemy_generator_light,
	image_LUT_enemy_generator_shadow,
	image_LUT_enemy_wall_light,
	image_LUT_enemy_wall_shadow,
	image_LUT_enemy_ship_0_light,
	image_LUT_enemy_ship_0_shadow,
	image_LUT_enemy_ship_1_light,
	image_LUT_enemy_ship_1_shadow,
	image_LUT_enemy_ship_2_light,
	image_LUT_enemy_ship_2_shadow,
	image_LUT_enemy_ship_3_light,
	image_LUT_enemy_ship_3_shadow,
	image_LUT_enemy_ship_4_light,
	image_LUT_enemy_ship_4_shadow,
	image_LUT_enemy_ship_4_lightball,
	image_LUT_enemy_ship_4_shadowball,
	image_LUT_enemy_ship_5_light,
	image_LUT_enemy_ship_5_shadow,
	image_LUT_enemy_ship_6_light,
	image_LUT_enemy_ship_6_shadow,
	image_LUT_door_left,
	image_LUT_door_right,
	image_LUT_box_light_1,
	image_LUT_box_light_2,
	image_LUT_box_light_3,
	image_LUT_box_light_4,
	image_LUT_box_light_5,
	image_LUT_box_light_6,
	image_LUT_box_shadow_1,
	image_LUT_box_shadow_2,
	image_LUT_box_shadow_3,
	image_LUT_box_shadow_4,
	image_LUT_box_shadow_5,
	image_LUT_box_shadow_6,
	image_LUT_box_solid_1,
	image_LUT_box_solid_2,
	image_LUT_box_solid_3,
	image_LUT_box_solid_4,
	image_LUT_box_solid_5,
	image_LUT_box_solid_6,
	image_LUT_prop_wall_left,
	image_LUT_prop_wall_right,
	image_LUT_chain_hit_light,
	image_LUT_chain_hit_shadow,
	image_LUT_explosion_light_0,
	image_LUT_explosion_light_1,
	image_LUT_explosion_light_2,
	image_LUT_explosion_light_3,
	image_LUT_explosion_light_4,
	image_LUT_explosion_light_5,
	image_LUT_explosion_shadow_0,
	image_LUT_explosion_shadow_1,
	image_LUT_explosion_shadow_2,
	image_LUT_explosion_shadow_3,
	image_LUT_explosion_shadow_4,
	image_LUT_explosion_shadow_5,
	image_LUT_powerslot,
	image_LUT_lives,
	image_LUT_titleScreen,
	image_LUT_bossWarning,
	image_LUT_boss1_enemy_ship_light,
	image_LUT_boss1_enemy_ship_shadow,
	image_LUT_boss1_grenade_light,
	image_LUT_boss1_grenade_shadow,
	image_LUT_boss2_leftShield,
	image_LUT_boss2_rightShield,
	image_LUT_boss2_leftUpperArm,
	image_LUT_boss2_rightUpperArm,
	NB_IMAGES
};

// Boss images LUT
enum
{
	bossImage_LUT_1_body,
	bossImage_LUT_1_leftarm_armed,
	bossImage_LUT_1_rightarm_armed1,
	bossImage_LUT_1_rightarm_armed2,
	bossImage_LUT_1_leftarm_nonarmed,
	bossImage_LUT_1_rightarm_nonarmed,
	bossImage_LUT_2_body,
	bossImage_LUT_2_leftShield,
	bossImage_LUT_2_rightShield,
	bossImage_LUT_2_leftArm,
	bossImage_LUT_2_rightArm,
	bossImage_LUT_2_leftUpperArm,
	bossImage_LUT_2_rightUpperArm,
	bossImage_LUT_2_leftWing,
	bossImage_LUT_2_leftWingOpening_1,
	bossImage_LUT_2_leftWingOpening_2,
	bossImage_LUT_2_leftWingOpening_3,
	bossImage_LUT_2_leftWingOpening_4,
	bossImage_LUT_2_rightWing,
	bossImage_LUT_2_rightWingOpening_1,
	bossImage_LUT_2_rightWingOpening_2,
	bossImage_LUT_2_rightWingOpening_3,
	bossImage_LUT_2_rightWingOpening_4,
	bossImage_LUT_2_rails,
	bossImage_LUT_2_hitpoint_left_1,
	bossImage_LUT_2_hitpoint_left_2,
	bossImage_LUT_2_hitpoint_left_3,
	bossImage_LUT_2_hitpoint_left_4,
	bossImage_LUT_2_hitpoint_left_5,
	bossImage_LUT_2_hitpoint_right_1,
	bossImage_LUT_2_hitpoint_right_2,
	bossImage_LUT_2_hitpoint_right_3,
	bossImage_LUT_2_hitpoint_right_4,
	bossImage_LUT_2_hitpoint_right_5,
	NB_BOSS_IMAGES
};

// Background images LUT
enum
{
	bgImage_LUT_1_0,
	bgImage_LUT_1_1,
	bgImage_LUT_1_2,
	bgImage_LUT_1_3,
	bgImage_LUT_1_4,
	bgImage_LUT_2_0,
	bgImage_LUT_2_1,
	bgImage_LUT_2_2,
	bgImage_LUT_2_3,
	bgImage_LUT_2_4,
	bgImage_LUT_2_5,
	bgImage_LUT_2_6,
	bgImage_LUT_2_7,
	NB_BACKGROUND_IMAGES
};

// Enemies' patterns
enum
{
	Pattern_null,
	Pattern_box,
	Pattern_prop,
	// Intro 1
	Pattern_1_1,
	Pattern_1_2,
	Pattern_1_3,
	Pattern_1_4,
	Pattern_1_5,
	Pattern_1_6,
	// Chapter 1
	Pattern_1_7,
	Pattern_1_8,
	Pattern_1_9,
	Pattern_1_10,
	Pattern_1_11,
	Pattern_1_12,
	Pattern_1_13,
	Pattern_1_14,
	Pattern_1_15,
	Pattern_1_16,
	Pattern_1_17,
	Pattern_1_18,
	Pattern_1_19,
	Pattern_1_20,
	Pattern_1_21,
	Pattern_1_boss,
	Pattern_1_bossGrenade,
	// Intro 2
	Pattern_2_1,
	Pattern_2_2,
	Pattern_2_3,
	Pattern_2_4,
	Pattern_2_5,
	Pattern_2_6,
	Pattern_2_7,
	Pattern_2_leftDoor,
	Pattern_2_rightDoor,
	// Chapter 2
	Pattern_2_8,
	Pattern_2_9,
	Pattern_2_10,
	Pattern_2_11,
	Pattern_2_12,
	Pattern_2_13,
	Pattern_2_14,
	Pattern_2_15,
	Pattern_2_16,
	Pattern_2_17,
	Pattern_2_18,
	Pattern_2_19,
	Pattern_2_20,
	Pattern_2_21,
	Pattern_2_22,
	Pattern_2_23,
	Pattern_2_24,
	Pattern_2_25,
	Pattern_2_26,
	Pattern_2_27,
	Pattern_2_28,
	Pattern_2_29,
	Pattern_2_30,
	Pattern_2_31,
	Pattern_2_32,
	Pattern_2_33,
	Pattern_2_34,
	Pattern_2_35,
	Pattern_2_36,
	Pattern_2_wall,
	Pattern_2_bossWeakPoint,
	Pattern_2_bossShield,
	NB_CALLBACKS
};

// Enemy types
enum
{
	TYPE_ENEMY,
	TYPE_PROP,
	TYPE_BREAKABLE_PROP
};

// /!\ Fixed-point value
#define FCAMERA_SPEED (itofix(1) / 3)
// /!\ integer value
#define INV_CAMERA_SPEED 3

// Camera travelling handlers
enum
{
	CameraPath_i1,
	CameraPath_c1,
	CameraPath_i2,
	CameraPath_c2,
	CameraPath_c2_2,
	CameraPath_c2_boss
};

// Camera relations
enum
{
	CAMREL_NONE,
	CAMREL_ABSOLUTE,
	CAMREL_RELATIVE
};

// Background handlers
enum
{
	bgHandle_default,
	bgHandle_2_2
};

extern unsigned short *image_entries[NB_IMAGES];
extern unsigned short *bossImage_entries[NB_BOSS_IMAGES];
extern unsigned short *bgImage_entries[NB_BACKGROUND_IMAGES];
extern Mix_Chunk *sound_entries[NB_SOUNDS];
extern Mix_Music *music_entries[NB_MUSICS];

extern void buildGameLUTs();
extern void freeGameLUTs();

// Global vars
extern DrawingCandidates *DC;
extern Particles *G_particles;
extern int G_minX, G_maxX; // screen boundaries, changed on special occasions
extern int G_gpTimer;
extern int G_skipFrame;
extern int G_killedThisFrame[MAX_ENEMY], G_frameChainOffset, G_chainStatus, G_inChainCount, G_maxChain;
extern int G_score, G_power;
extern bool G_usingArrows;
extern bool G_fireback, G_hardMode;
extern bool G_hasFiredOnce;
extern int G_bossIntroChannel;

// Custom keys
extern t_key G_downKey, G_leftKey, G_rightKey, G_upKey, G_fireKey, G_polarityKey, G_fragmentKey, G_pauseKey;

// Utils
extern Enemy* findNearestEnemy(Fixed x, Fixed y);
extern bool collidePointRect(Fixed, Fixed, Fixed, Fixed, int, int);
extern int distance(int x1, int y1, int x2, int y2);
extern int iToScreenX(int x, int camRel);
extern int iToScreenY(int y, int camRel);
extern Fixed fToScreenX(Fixed x, int camRel);
extern Fixed fToScreenY(Fixed y, int camRel);
extern BossData createBossData(int bossID);
extern int isKeyPressed(t_key);

#endif
