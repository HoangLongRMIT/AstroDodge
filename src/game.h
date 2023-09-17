//#include <stdint.h>

#include "mbox.h"
#include "uart.h"



#define NUM_ENEMIES (12)

#define NUM_BUNKERS 3
#define BUNKER_HEALTH 10

#define alien_initial_y 0
#define alien_initial_x 200
#define HORIZONTAL_OFFSET 61
#define VERTICAL_OFFSET 61

#define ENEMIES_VERTICAL_MAX (1080 - 262 - VERTICAL_OFFSET)

#define PLAYER_SPEED 15
#define VERTICAL_SPEED 30
#define HORIZONTAL_SPEED 1

#define LEFT_MAX 50
#define RIGHT_MAX (MAP_WIDTH - 50) 
#define TOP_MAX 162
#define BOTTOM_MAX 917
#define MAX_BULLETS 4
#define BULLET_VELOCITY 15

#define MAP_WIDTH 1024
#define MAP_HEIGHT 768

#define MAX_SHOOTERS 10

#define SCORE_ORIGINX 850
#define SCORE_ORIGINY 5
#define SHIFT 35

#define BAR_ORIGINX 1150
#define BAR_ORIGINY 180

static int travel_right = 0;

static int quitGame;
static int restartGame;
static int pauseGame;

typedef union {
    int current_health;
    int player_health;
} Health;

typedef struct {
    float x, y;
} Position;

typedef struct {
    float width, height;
} Dimension;

typedef struct {
    float x, y;
} Velocity;

typedef struct {
    int score;
    int needsUpdate;
    int needsRender;
} Score;

typedef enum { PLAYER = 1, ENEMIES=2} Type;

typedef struct {
    Position position;
    Position previous_pos;
    Velocity velocity;
    Dimension dimension;
    int needs_update;
    int needs_render;
    int needs_clear;
    int active;
} Missile;
/* ship, shooter */
typedef struct {
    Velocity velocity;
    Dimension dimension;
    Position position;
    Position previous_pos;
    Missile projectile[MAX_BULLETS];
    Type type;
    Health health;
    int needs_update;
    int needs_render;
    int needs_clear;
    int combat_update;
    int enabled;
} Entity;
typedef struct {
    int game_menu_option;
    int on_gameMenu_menu;
} GameMenu;

typedef struct {
    int game_start_menu;
    int on_game_menu;
} MainMenu;

typedef struct map {
    Entity player;
    Entity enemies[NUM_ENEMIES];
    int shooters[MAX_SHOOTERS];
    int left_most_enemies[6];
    int right_most_enemies[6];
    int enemies_alive;
    int game_over;
    Score playerScore;
    Entity life;
    GameMenu game_menu;

} World;

typedef struct {
    World world;
    int game_win;
    int game_over;
    int game_start;
    MainMenu main_menu;
} Game;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    STOP,
    RESET_VERTICAL,
    RESET_HORIZONTAL
} Direction;
static Dimension meteors= {100,100};
void init_game(Game *world);
void init_map(World *world);

void init_player(Entity *player);
void init_enemies(World *world);
void init_life(Entity *life);
void render(World *world);  // framebf.c
// void move_player(World *world);
void move_player(World *world);

void update_player_position(World *world);
void move_entity(Entity *entity, Direction direction);
void drawEntity(Entity entity);

void entity_shoot(Entity *entity, Direction direction);
Missile *create_bullet(Entity owner);
void move_bullet(Missile *projectile, Direction direction);
void *memcpy(void *dest, const void *src, unsigned long n);
void draw_projectile(Type type, Position position, Dimension dimension);
void clear_projectile(Position position, Dimension dimension);
void render_health(World *world);

void render_score(int num,int x, int y);

void update_combat_system(World *world);
void update_collision_system(World *world);
void clear(Entity entity);
void update_score(World *world);
void enemy_shoot(World *world);
int rand(void);

void update_AI_system(World *world);
int enemies_at_bottom(World *world);
void update_shooters(World *world, int index);
void drawGameMenu(World *game);
void drawMainMenu(Game *game);
void show_game_menu(World *world);
void show_main_menu(Game *game) ;
void drawScore(World *world,char *type);
void init_playerScore(Score *playerScore);
void endScreen(int won, World *world) ;
void restart_game(Game *world);
void drawSpaceShip(Entity entity, World *world);

// void displaySpaceShipImage(int x, int y);
