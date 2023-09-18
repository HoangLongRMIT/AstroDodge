#include "uart.h"
#include "function.h"
#include "cli_function.h"
#include "mbox.h"
#include "framebf.h"
#include "game.h"
// #include "spaceship.h"
#include "display_image.h"
#include "object.h"
#include "printf.h"
#include "game_universe_background_1.h"
// #include "asteroid.h"

int wait_time_shoot = 100;
void init_game(Game *world)
{
    world->game_over = 0;
    world->game_start = 0;
    world->main_menu.on_game_menu = 1;
    world->main_menu.game_start_menu = 1;
    world->game_win = 0;
    restartGame = 0;
    pauseGame = 0;
    quitGame = 0;

    init_map(&world->world);
    framebf_init();
}
// Create the stage
void init_map(World *world)
{
    init_player(&world->player);
    init_enemies(world);
    init_playerScore(&world->playerScore);
    init_life(&world->life);
    world->enemies_alive = NUM_ENEMIES;
    world->game_menu.game_menu_option = 0;
    world->game_menu.on_gameMenu_menu = 0;
    world->game_over = 0;
}
void restart_game(Game *world)
{   clearscreen(0, 0);
    init_map(&world->world);
    world->game_over = 0;
    world->game_start = 0;
    world->main_menu.on_game_menu = 1;
    world->main_menu.game_start_menu = 1;
    world->game_win = 0;
    restartGame = 0;
    displayGameUniverseBackground(0, 0);
    pauseGame = 0;
    quitGame = 0;
}
// Setting the value for player
void init_player(Entity *player)
{
    player->dimension.height = 90;
    player->dimension.width = 72;
    player->position.x = (MAP_WIDTH / 2) - (player->dimension.width / 2);
    player->position.y = MAP_HEIGHT - 50;
    for (int i = 0; i < MAX_BULLETS; i++)
        player->projectile[i].active = 0;
    player->health.current_health = 3;
    player->type = PLAYER;
    player->needs_update = 1;
    player->needs_render = 0;
    player->needs_clear = 0;
    player->combat_update = 0;
    player->enabled = 1;
}

// Setting the value for enemies
void init_enemies(World *world)
{
    for (int i = 0, j = 0; i < NUM_ENEMIES; i++)
    {
        world->enemies[i].needs_render = 0;
        world->enemies[i].needs_update = 1;
        world->enemies[i].enabled = 1;
        world->enemies[i].combat_update = 0;
        for (int j = 0; j < MAX_BULLETS; j++){
            world->enemies[i].projectile[j].active = 0;
            world->enemies[i].projectile[j].position.y=1000;
        }
            
            
    }

    for (int i = 0; i < MAX_SHOOTERS; i++)
    {
        world->shooters[i] = i;
    }
}

// Move player
void move_player(World *world)
{
    uart_puts("Press A to move left: \n");
    uart_puts("Press D to move right: \n");
    uart_puts("Press W to move up: \n");
    uart_puts("Press S to move down: \n");
    uart_puts("Press SPACE to move down: \n");
    uart_puts("Press P to stop: \n");
    while (!quitGame && !restartGame)
    {
        while (!pauseGame)
        {
            char character = uart_getc_game();
            if (character != '\n' && character != '\b')
            {
            }
            if (character == 'a')
            {
                move_entity(&world->player, LEFT);
                world->player.velocity.y = 0;
            }
            // if character = s, scroll down -> screen down
            else if (character == 'd')
            {
                move_entity(&world->player, RIGHT);
                world->player.velocity.y = 0;
            }
            else if (character == 'w')
            {
                world->player.velocity.y = -VERTICAL_SPEED;
                world->player.velocity.x = 0;
                world->player.needs_update = 1;
            }
            else if (character == 's')
            {
                world->player.velocity.y = VERTICAL_SPEED;
                world->player.velocity.x = 0;
                world->player.needs_update = 1;
            }
            else if (character == ' ')
            {
                entity_shoot(&world->player, UP);
            }
            else if (character == 'p')
            {
                show_game_menu(world);
            }
            update_AI_system(world);
            update_collision_system(world);
            update_combat_system(world);
            update_player_position(world);
            render(world);
            if (wait_time_shoot % 10 == 0 && wait_time_shoot != 100)
            {
                enemy_shoot(world);
            }
            if (wait_time_shoot == 100)
            {
                enemy_shoot(world);
                wait_time_shoot = 0;
            }
            {
                wait_time_shoot++;
            }
        }
    }
}

void show_main_menu(Game *game)
{

    uart_puts("Press s to move down: \n");
    uart_puts("Press w to move up: \n");
    uart_puts("Press space to choose: \n");
    // drawLogo();
    drawMainMenu(game);
    while (game->main_menu.on_game_menu)
    {
        char character = uart_getc_game();
        if (character == 'w')
        {
            game->main_menu.game_start_menu = 1; // select start
            drawMainMenu(game);
        }
        else if (character == 's')
        {
            game->main_menu.game_start_menu = 0; // select quit
            drawMainMenu(game);
        }

        else if (character == ' ')
        {
            game->main_menu.on_game_menu = 0;
            if (game->main_menu.game_start_menu)
                game->game_start = 1;
            else
                game->game_start = 0;
            displayGameUniverseBackground(0, 0);
        }
    }
}
void show_game_menu(World *world)
{
    world->game_menu.game_menu_option = 1;
    world->game_menu.on_gameMenu_menu = 1;
    pauseGame = 1;
    while (world->game_menu.on_gameMenu_menu)
    {
        //    while (clock() < menu_timer)
        //         ;
        //     menu_timer = clock() + CLOCKS_PER_SEC / 4;

        drawGameMenu(world);
        char character = uart_getc_game();
        printf("\n%d", world->game_menu.game_menu_option);
        if (character == 'w') // up
        {
            if (world->game_menu.game_menu_option < 2)
            {
                world->game_menu.game_menu_option++;
            }
        }
        else if (character == 's') // down
        {
            if (world->game_menu.game_menu_option > 0)
            {
                world->game_menu.game_menu_option--;
            }
        }
        else if (character == ' ') // B
        {
            if (world->game_menu.game_menu_option == 2)
            {
                world->game_menu.on_gameMenu_menu = 0;
                displayGameUniverseBackground(0,0);
                printf("\nSELECT: Pause");
                world->life.needs_render = 1;
                world->playerScore.needsRender = 1;
                world->player.needs_render = 1;
                pauseGame = 0;
            }
            else if (world->game_menu.game_menu_option == 1)
            {
                displayGameUniverseBackground(0,0);
                printf("\nSELECT: Restart");
                restartGame = 1;
                return;
            }
            else if (world->game_menu.game_menu_option == 0)
            {
                displayGameUniverseBackground(0,0);
                printf("\nSELECT: Quit");
                quitGame = 1;
                return;
            }
        }
    }
    return;
}

void move_entity(Entity *entity, Direction direction)
{
    switch (direction)
    {
    case LEFT:
        entity->velocity.x =
            (entity->type == PLAYER) ? -PLAYER_SPEED : -HORIZONTAL_SPEED;
        // entity->velocity.y = 0;
        entity->needs_update = 1;
        break;
    case RIGHT:
        entity->velocity.x =
            (entity->type == PLAYER) ? PLAYER_SPEED : HORIZONTAL_SPEED;

        entity->needs_update = 1;
        break;
    case UP:
        // entity->velocity.x = 0;
        entity->velocity.y = -VERTICAL_SPEED;
        entity->needs_update = 1;
        break;
    case DOWN:
        // entity->velocity.x = 0;
        entity->velocity.y = VERTICAL_SPEED;
        entity->needs_update = 1;
        break;
    case RESET_VERTICAL:
        entity->velocity.y = 0;
        entity->needs_update = 1;
        break;
    case RESET_HORIZONTAL:
        entity->velocity.x = 0;
        entity->needs_update = 1;
        break;
    default:
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->needs_update = 1;
    }
}
// void update_movement_system(World *world) {
void update_player_position(World *world)
{
    if (world->player.needs_update)
    {
        world->player.previous_pos = world->player.position;
        world->player.position.x += world->player.velocity.x;
        world->player.position.y += world->player.velocity.y;
        // Stop moving at edge
        if (world->player.position.x < 0)
        {
            world->player.position.x = 0;
        }
        else if (world->player.position.x >
                 MAP_WIDTH - world->player.dimension.width)
        {
            world->player.position.x =
                MAP_WIDTH - world->player.dimension.width;
        }
        if (world->player.position.y < 0)
        {
            world->player.position.y = 0;
        }
        else if (world->player.position.y >
                 MAP_HEIGHT - world->player.dimension.height)
        {
            world->player.position.y =
                MAP_HEIGHT - world->player.dimension.height;
        }
        world->player.needs_render = 1;
        world->player.needs_update = 0;
    }

    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        if (world->enemies[i].needs_update)
        {
            world->enemies[i].previous_pos = world->enemies[i].position;
            world->enemies[i].position.x += world->enemies[i].velocity.x;
            world->enemies[i].position.y += world->enemies[i].velocity.y;
            world->enemies[i].needs_render = 1;
            world->enemies[i].needs_update = 0;
        }
        // Check if enemy hit player
        if (world->enemies[i].position.x < world->player.position.x +
                                               world->player.dimension.width &&
            world->enemies[i].position.x + world->enemies[i].dimension.width >
                world->player.position.x &&
            world->enemies[i].position.y < world->player.position.y +
                                               world->player.dimension.height &&
            world->enemies[i].position.y + world->enemies[i].dimension.height >
                world->player.position.y)
        {
            endScreen(0, world);
        }
    }
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (world->player.projectile[i].needs_update)
        {
            if (world->player.projectile[i].position.y > TOP_MAX)
            {
                world->player.projectile[i].previous_pos =
                    world->player.projectile[i].position;
                world->player.projectile[i].position.x +=
                    world->player.projectile[i].velocity.x;
                world->player.projectile[i].position.y +=
                    world->player.projectile[i].velocity.y;
                world->player.projectile[i].needs_render = 1;
            }
            else
            {
                world->player.projectile[i].needs_render = 0;
                world->player.projectile[i].active = 0;
                world->player.projectile[i].needs_clear = 1;
            }
        }
    }
    for (int index = 0; index < 10; index++)
    {
        int i = world->shooters[index];
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (world->enemies[i].projectile[j].needs_update)
            {
                if (world->enemies[i].projectile[j].position.y < BOTTOM_MAX)
                {
                    world->enemies[i].projectile[j].previous_pos =
                        world->enemies[i].projectile[j].position;
                    world->enemies[i].projectile[j].position.x +=
                        world->enemies[i].projectile[j].velocity.x;
                    world->enemies[i].projectile[j].position.y +=
                        world->enemies[i].projectile[j].velocity.y;
                    world->enemies[i].projectile[j].needs_render = 1;
                }
                else
                {
                    world->enemies[i].projectile[j].needs_render = 0;
                    world->enemies[i].projectile[j].active = 0;
                    world->enemies[i].projectile[j].needs_clear = 1;
                }
            }
        }
    }
}
void enemy_shoot(World *world)
{
    // if (clock() < before) return;
    // before = clock() + CLOCKS_PER_SEC / 2;

    int random = (rand() % 100) % 10;
    // printf("\n%d", random);
    entity_shoot(&world->enemies[world->shooters[random]], DOWN);
}
unsigned long int next = 1;
int rand(void)
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
// Function to generate a random number between 100 and 900
int randAsteroidPosition()
{
    // Generate a random number between 0 and 800 (inclusive)
    int randomNumber = rand() % 801;

    // Add 100 to the generated number to get a range from 100 to 900
    return randomNumber + 100;
}
void entity_shoot(Entity *entity, Direction direction)
{
    // if (clock() < entity->timer) return;

    // entity->timer = clock() + CLOCKS_PER_SEC / 2;
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!entity->projectile[i].active)
        {
            // Initial a bullet from player
            if (entity->type == PLAYER)
            {
                entity->projectile[i].position.x =
                    entity->position.x + (entity->dimension.width / 2);
                entity->projectile[i].position.y =
                    entity->position.y - entity->dimension.height;
                entity->projectile[i].dimension.height = red_laser.height;
                entity->projectile[i].dimension.width = red_laser.width;
            }
            else
            {
                entity->projectile[i].position.x =
                    randAsteroidPosition();
                entity->projectile[i].position.y =
                    50;
                entity->projectile[i].dimension.height = asteroid_image.height;
                entity->projectile[i].dimension.width = asteroid_image.width;
            }
            entity->projectile[i].needs_update = 1;
            entity->projectile[i].needs_render = 1;
            entity->projectile[i].active = 1;
            move_bullet(&entity->projectile[i], direction);
            return;
        }
    }
}
void update_AI_system(World *world)
{
    /* vertical reset */
    for (int i = 0; i < NUM_ENEMIES; i++)
        move_entity(&world->enemies[i], RESET_VERTICAL);

    /* check wall collisions */
    for (int i = 0; i < 6; i++)
    {
        if ((world->enemies[9].position.x +
             world->enemies[9].dimension.width) >= (RIGHT_MAX))
        {
            travel_right = 0;
            if (!enemies_at_bottom(world))
            {
                for (int j = 0; j < NUM_ENEMIES; j++)
                {
                    move_entity(&world->enemies[j], DOWN);
                }
            }
        }
        else if ((world->enemies[0].position.x) <= (LEFT_MAX))
        {
            travel_right = 1;
            if (!enemies_at_bottom(world))
            {
                for (int j = 0; j < NUM_ENEMIES; j++)
                {
                    move_entity(&world->enemies[j], DOWN);
                }
            }
        }
    }

    /* move enemies right or left */
    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        if (travel_right)
        {
            move_entity(&world->enemies[i], RIGHT);
        }
        else
        {
            move_entity(&world->enemies[i], LEFT);
        }
    }
}
void move_bullet(Missile *projectile, Direction direction)
{
    switch (direction)
    {
    case UP:
        projectile->velocity.y = -BULLET_VELOCITY;
        break;
    case DOWN:
        projectile->velocity.y = BULLET_VELOCITY;
        break;
    default:
        projectile->velocity.y = 0;
    }
}

void update_left_most(World *world, int index)
{
    for (int i = 0; i < 6; i++)
    {
        if (world->left_most_enemies[i] == index)
        {
            world->left_most_enemies[i] += 1;
        }
    }
}

void update_right_most(World *world, int index)
{
    for (int i = 0; i < 6; i++)
    {
        if (world->right_most_enemies[i] == index)
        {
            world->right_most_enemies[i] -= 1;
        }
    }
}

int intersectAABB(Missile *projectile, Entity *entity)
{
    return projectile->position.x <
               (entity->position.x + entity->dimension.width) &&
           (projectile->position.x + projectile->dimension.width) >
               entity->position.x &&
           projectile->position.y <
               (entity->position.y + entity->dimension.height) &&
           (projectile->position.y + projectile->dimension.height) >
               entity->position.y;
}

int intersection(Missile *projectile, Missile *entity)
{
    return projectile->position.x <
               (entity->position.x + entity->dimension.width) &&
           (projectile->position.x + projectile->dimension.width) >
               entity->position.x &&
           projectile->position.y <
               (entity->position.y + entity->dimension.height) &&
           (projectile->position.y + projectile->dimension.height) >
               entity->position.y;
}

void resolve_collisions(Missile *projectile, Entity *entity)
{
    int isEnabled = entity->enabled;
    int intersects = intersectAABB(projectile, entity);
    if (isEnabled && intersects)
    {
        projectile->active = 0;
        projectile->needs_update = 0;
        projectile->needs_render = 0;
        projectile->needs_clear = 1;
        entity->combat_update = 1;
    }
}

void resolve_collisions2(Missile *projectile, Missile *projectile2, World *world)
{
    int intersects = intersection(projectile, projectile2);
    if (intersects)
    {
        drawExplosion2(projectile2);
        wait_msec(1000);
        projectile->active = 0;
        projectile->needs_update = 0;
        projectile->needs_render = 0;
        projectile->needs_clear = 1;
        projectile2->active = 0;
        projectile2->needs_update = 0;
        projectile2->needs_render = 0;
        projectile2->needs_clear = 1;
        update_score(world);
        world->playerScore.needsRender = 1;
        uart_puts("hit \n");
    }
}

void update_collision_system(World *world)
{
    Entity *player = &world->player;
    Entity *enemy = world->enemies;

    for (int i = 0; i < 10; i++)
    {
        int index = world->shooters[i];
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            if (enemy[index].projectile[j].active)
            {
                resolve_collisions(&enemy[index].projectile[j], player);
                for (int a = 0; a < MAX_BULLETS; a++)
                {
                    if (player->projectile[a].active)
                    {
                        resolve_collisions2(&player->projectile[a], &enemy[index].projectile[j], world);
                    }
                }
            }
        }
    }
}
void update_shooters(World *world, int index)
{
    for (int i = 0; i < MAX_SHOOTERS; i++)
    {
        if (world->shooters[i] == index)
        {
            world->shooters[i] += 10;
        }
    }
}
void update_combat_system(World *world)
{

    if (world->player.combat_update)
    {
        drawExplosion(world->player);
        wait_msec(500);

        drawSpaceShip(world->player, world);
        world->life.needs_render = 1;
        world->player.health.current_health -= 1;
        if (world->player.health.current_health <= 0)
        {
            world->player.enabled = 0;
            world->player.needs_clear = 1;
        }
        world->player.combat_update = 0;
    if (world->playerScore.score >= 300)
    {
        endScreen(1, world);
    }
        if (world->player.health.current_health == 0)
        {
            clearPlayerLife(170, 20);
            displayScore0(170, 10);
            endScreen(0, world);
        }
    }
}
int enemies_at_bottom(World *world)
{
    int bottom_most = 0;
    for (int i = 0; i < 10; i++)
    {
        if (world->enemies[bottom_most].position.y <
            world->enemies[i].position.y)
        {
            bottom_most = world->shooters[i];
        }
    }
    return (world->enemies[bottom_most].position.y +
            world->enemies[bottom_most].dimension.height) >
           ENEMIES_VERTICAL_MAX;
}
// Draw the enity using the data has set
void render(World *world)
{ if (quitGame)
    {
        return;
    }
    wait_msec(30000);

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        Type type = world->player.type;

        if (world->player.projectile[i].needs_render)
        {
            clear_projectile(world->player.projectile[i].previous_pos,
                             world->player.projectile[i].dimension);
            draw_projectile(type, world->player.projectile[i].position,
                            world->player.projectile[i].dimension);
        }
        else if (world->player.projectile[i].needs_clear)
        {
            clear_projectile(world->player.projectile[i].position,
                             meteors);
            world->player.projectile[i].needs_clear = 0;
        }
    }

    for (int i = 0; i < NUM_ENEMIES; i++)
    {
        if (world->enemies[i].needs_render && world->enemies[i].enabled)
        {
            clear(world->enemies[i]);
            drawEntity(world->enemies[i]);

            world->enemies[i].needs_render = 1; // 0 default
        }
        else if (world->enemies[i].needs_clear)
        {
            clear(world->enemies[i]);
            // fix bug bullet not clear
            for (int j = 0; j < MAX_BULLETS; j++)
            {
                clear_projectile(world->enemies[i].projectile[j].position,
                                 world->enemies[i].projectile[j].dimension);
            }

            drawSpaceShip(world->player, world);
            world->enemies[i].needs_clear = 0;
        }
    }

    for (int i = 0; i < MAX_SHOOTERS; i++)
    {
        for (int j = 0; j < MAX_BULLETS; j++)
        {
            int index = world->shooters[i];
            Type type = world->enemies[index].type;
            if (world->enemies[index].projectile[j].needs_render)
            {

                clear_projectile(
                    world->enemies[index].projectile[j].previous_pos,
                    world->enemies[index].projectile[j].dimension);
                draw_projectile(type,
                                world->enemies[index].projectile[j].position,
                                world->enemies[index].projectile[j].dimension);
            }
            else if (world->enemies[index].projectile[j].needs_clear)
            {
                clear_projectile(world->enemies[index].projectile[j].position,
                                 world->enemies[index].projectile[j].dimension);
                world->enemies[index].projectile[j].needs_clear = 0;
            }
        }
    }

    if (world->player.needs_render && world->player.enabled)
    {
        clear(world->player);
        drawSpaceShip(world->player, world);
        world->player.needs_render = 0;
    }
    else if (world->player.needs_clear)
    {
        drawExplosion2(world->player);
        wait_msec(500);
        world->player.needs_clear = 0;
    }

    if (world->life.needs_render)
    {
        render_health(world);
    }
    if (world->playerScore.needsRender)
    {
        char *type = "";
        drawScore(world, type);
        world->playerScore.needsRender = 0;
    }
}

void drawScore(World *world, char *type)
{
    int x = 0;
    int y = 0;
    if (type == "d")
    {
        x = 350;
        y = 500;
        displayScore(770 - x, 10 + y);
    }
    if (type == "")
    {
        displayScore(770, 10);
    }

    int ones = (world->playerScore.score % 10);
    int tens = (world->playerScore.score % 100) / 10;
    int hundreds = (world->playerScore.score % 1000) / 100;
    int thousands = (world->playerScore.score % 10000) / 1000;

    int score = world->playerScore.score;
    if (score >= 0 && score < 10)
    {
        clearScore(ones, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        render_score(ones, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
    }
    else if (score >= 10 && score < 100)
    {
        clearScore(tens, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        clearScore(ones, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
        render_score(tens, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        render_score(ones, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
    }
    else if (score >= 100 && score < 1000)
    {
        clearScore(hundreds, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        clearScore(tens, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
        clearScore(ones, SCORE_ORIGINX - x + SHIFT + SHIFT, SCORE_ORIGINY + y);
        render_score(hundreds, SCORE_ORIGINX - x, SCORE_ORIGINY + y);
        render_score(tens, SCORE_ORIGINX - x + SHIFT, SCORE_ORIGINY + y);
        render_score(ones, SCORE_ORIGINX - x + SHIFT + SHIFT, SCORE_ORIGINY + y);
    }
    else if (score >= 1000)
    {
        clearScore(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
        clearScore(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
        clearScore(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
        clearScore(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT,
                   SCORE_ORIGINY);

        render_score(thousands, SCORE_ORIGINX, SCORE_ORIGINY);
        render_score(hundreds, SCORE_ORIGINX + SHIFT, SCORE_ORIGINY);
        render_score(tens, SCORE_ORIGINX + SHIFT + SHIFT, SCORE_ORIGINY);
        render_score(ones, SCORE_ORIGINX + SHIFT + SHIFT + SHIFT,
                     SCORE_ORIGINY);
    }
}

void render_health(World *world)
{
    int clife = (world->player.health.current_health);
    printf("health: %d\n", clife);
    displayWordPlayerLife(100, 10);

    if (clife == 0)
    {
        clearPlayerLife(170, 30);
        displayScore0(170, 10);
    }

    if (clife == 1)
    {
        clearPlayerLife(210, 20);
        displayPlayerLife(170, 20);
    }
    if (clife == 2)
    {
        clearPlayerLife(250, 20);
        displayPlayerLife(170, 20);
        displayPlayerLife(210, 20);
    }
    if (clife == 3)
    {
        displayPlayerLife(170, 20);
        displayPlayerLife(210, 20);
        displayPlayerLife(250, 20);
    }

    world->life.needs_render = 0;
}

void clear_health(int x, int y)
{
    for (int h = 0; h < x; h++)
    {
        for (int w = 0; w < y; w++)
        {
            drawPixelARGB32(w, h, 0);
        }
    }
}

void render_score(int num, int x, int y)
{

    if (num == 1)
        displayScore1(x, y);
    else if (num == 2)
        displayScore2(x, y);
    else if (num == 3)
        displayScore3(x, y);
    else if (num == 4)
        displayScore4(x, y);
    else if (num == 5)
        displayScore5(x, y);
    else if (num == 6)
        displayScore6(x, y);
    else if (num == 7)
        displayScore7(x, y);
    else if (num == 8)
        displayScore8(x, y);
    else if (num == 9)
        displayScore9(x, y);
    else if (num == 0)
        displayScore0(x, y);
}

void init_life(Entity *life)
{
    life->health.player_health = 3;
    life->needs_update = 0;
    life->needs_render = 1;
}

void init_playerScore(Score *playerScore)
{
    playerScore->score = 0;
    playerScore->needsUpdate = 0;
    playerScore->needsRender = 1;
}

void update_score(World *world)
{

    world->playerScore.score += 30;
}

void *memcpy(void *dest, const void *src, unsigned long n)
{
    for (unsigned long i = 0; i < n; i++)
    {
        ((char *)dest)[i] = ((char *)src)[i];
    }
}

void drawGameMenu(World *game)
{
    int *colorptrMenu;
    int widthMenu = game_menu_pause.width;
    int heightMenu = game_menu_pause.height;
    if (game->game_menu.game_menu_option == 2)
        colorptrMenu = (int *)game_menu_pause.image_pixels;
    else if (game->game_menu.game_menu_option == 1)
        colorptrMenu = (int *)game_menu_restart.image_pixels;
    else if (game->game_menu.game_menu_option == 0)
        colorptrMenu = (int *)game_menu_quit.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
    int yMenu = TOP_MAX;

    for (int i = 0; i < (widthMenu * heightMenu); i++)
    {
        xMenu++;
        if (i % widthMenu == 0)
        {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
        }
        if (colorptrMenu[i] != 0x00000000)
        {
            // uart_hex(colorptrMenu[i]);
            drawPixelARGB32(xMenu, yMenu, colorptrMenu[i]);
        }
        else {
            drawPixelARGB32(xMenu, yMenu, background_universe_image_1[yMenu * universe_background_width_1 + xMenu]);
        }
        // drawPixel(xMenu, yMenu, colorptrMenu[i]);
    }
}
void drawMainMenu(Game *game)
{
    int *colorptrMenu;
    int widthMenu = start_image.width;
    int heightMenu = start_image.height;

    if ((game->main_menu.game_start_menu))
        colorptrMenu = (int *)start_image.image_pixels;
    else
        colorptrMenu = (int *)quit_image.image_pixels;

    int xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
    int yMenu = TOP_MAX + 455;

    for (int i = 0; i < (widthMenu * heightMenu); i++)
    {
        xMenu++;
        if (i % widthMenu == 0)
        {
            yMenu++;
            xMenu = (int)((MAP_WIDTH / 2) - (widthMenu / 2));
        }
        if (colorptrMenu[i] != 0xFF000000)
        {
            drawPixelARGB32(xMenu, yMenu, colorptrMenu[i]);
        }
        else {
            drawPixelARGB32(xMenu, yMenu, background_universe_image_1[yMenu * universe_background_width_1 + xMenu]);
        }
    }
}

void endScreen(int won, World *world)
{

    pauseGame = 1;
    uart_puts("\n\n");
    uart_puts("Press o to out: \n");
    uart_puts("Press r to restart: \n");
    char *type = "d";
    displayGameUniverseBackground(0, 0);

    clearscreen(0, 0);

    if (won)
    {
        drawScore(world, type);
        displayGameWinImage(300, 100);
    }
    else
    {
        drawScore(world, type);
        displayGameOverImage(300, 100);
    }
    while (!restartGame)
    {
        char character = uart_getc();

        if (character == 'o')
        {
            quitGame = 1;
            uart_puts("\n\nSuccessfully out!\n");
            return;
        }
        if (character == 'r')
        {
            restartGame = 1;
        }
    }
    
    return;
}

void drawExplosion(Entity entity)
{
    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;
    if (entity.type == PLAYER)
    {
        uart_puts("Dead \n");
        displayExplosion(x, y);
    }
}
void drawExplosion2(Missile entity)
{
    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;

    displayExplosion2(x, y);
}

void drawSpaceShip(Entity entity, World *world)
{
    int score = world->playerScore.score;
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.position.x;
    int oldX = x;
    int y = entity.position.y;
    printf("Score: %d\n", score);
    if (entity.type == PLAYER)
    {

        displaySpaceShipImage(x, y);
    }
}

void clear(Entity entity)
{
    int width = entity.dimension.width;
    int height = entity.dimension.height;

    int x = entity.previous_pos.x;
    int oldX = x;
    int y = entity.previous_pos.y;

    for (int i = 0; i < (width * height); i++)
    {
        x++;
        if (i % width == 0)
        {
            y++;
            x = oldX;
        }
        drawPixelARGB32(x, y, background_universe_image_1[y * universe_background_width_1 + x]);
    }
}
