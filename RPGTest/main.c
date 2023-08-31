#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>

#include "tools.h"
#include "entlib.h"
#include "global.h"
#include "monsterslib.h"

#define KEY_SEEN     1
#define KEY_RELEASED 2

#define BUFFER_W 240
#define BUFFER_H 192

#define DISP_SCALE 3
#define DISP_W (BUFFER_W * DISP_SCALE)
#define DISP_H (BUFFER_H * DISP_SCALE)

#define TILE_SIZE 24

ALLEGRO_BITMAP* buffer;

void init_queues()
{
    all = malloc(sizeof(queue_t));
    if (all)
    {
        all->size = 0;
        all->first = NULL;
    }

    walls = malloc(sizeof(queue_t));
    if (walls)
    {
        walls->size = 0;
        walls->first = NULL;
    }

    potions = malloc(sizeof(queue_t));
    if (potions)
    {
        potions->size = 0;
        potions->first = NULL;
    }
    
    wizards = malloc(sizeof(queue_t));
    if (wizards)
    {
        wizards->size = 0;
        wizards->first = NULL;
    }
}

void restart_level(const char *level)
{
    destroy_entities(all);

    destroy_queue(all);
    destroy_queue(walls);
    destroy_queue(potions);
    destroy_queue(wizards);

    init_queues();
    fill_level(level);

    swd = false;
    swd_x = false;

    shd = false;
    shd_x = false;

    str = 1;

    dead = false;
}

bool plr_wall(short x, short y)
{
    for (node_t* n = walls->first; n != NULL; n = n->next)
        if (n->e->x == x && n->e->y == y)
            return false;
    return true;
}

void plr_entity()
{
    for (node_t* n = all->first; n != NULL; n = n->next)
        if (n->e->x == plr_x && n->e->y == plr_y)
        {
            switch (n->e->type)
            {
            case SWORD:
                str += 3;
                swd = true;
                remove_full_node(all, n);
                return;
            case ESWORD:
                str += 5;
                swd_x = true;
                remove_full_node(all, n);
                return;
            case SHIELD:
                shd = true;
                remove_full_node(all, n);
                return;
            case USHIELD:
                shd_x = true;
                remove_full_node(all, n);
                return;
            case SPOTION:
                str += 1;

                ent* entity = n->e;

                remove_only_node(all, n);
                remove_only_node(potions, n);

                free(n->e);
                return;

            default:
                if (n->e->str >= str)
                    dead = true;
                else
                    remove_full_node(all, n);
                return;
            }
        }
}

void move_player(unsigned char key[], bool *pressed)
{
    if (*pressed == false)
    {
        *pressed = true;
        turn = 1;
        if (key[ALLEGRO_KEY_UP] && plr_y > 0 && plr_wall(plr_x, plr_y - 1))
            plr_y -= 1;
        else if (key[ALLEGRO_KEY_DOWN] && plr_y < 7 && plr_wall(plr_x, plr_y + 1))
            plr_y += 1;
        else if (key[ALLEGRO_KEY_LEFT] && plr_x > 0 && plr_wall(plr_x - 1, plr_y))
            plr_x -= 1;
        else if (key[ALLEGRO_KEY_RIGHT] && plr_x < 7 && plr_wall(plr_x + 1, plr_y))
            plr_x += 1;
        else
        {
            *pressed = false;
            turn = 0;
        }
        
        plr_entity();
    }
}

void draw_map()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            al_draw_rectangle(
                j * TILE_SIZE,
                i * TILE_SIZE,
                j * TILE_SIZE + TILE_SIZE,
                i * TILE_SIZE + TILE_SIZE,
                al_map_rgb(100, 0, 0),
                1
            );
}

void draw_player()
{
    al_draw_filled_rectangle(
        plr_x * TILE_SIZE,
        plr_y * TILE_SIZE,
        plr_x * TILE_SIZE + TILE_SIZE,
        plr_y * TILE_SIZE + TILE_SIZE,
        al_map_rgb(255, 255, 255)
    );
}

void draw_objects(ALLEGRO_FONT* font)
{
    for (node_t* n = all->first; n != NULL; n = n->next)
    {
        al_draw_filled_rectangle(
            n->e->x * TILE_SIZE,
            n->e->y * TILE_SIZE,
            n->e->x * TILE_SIZE + TILE_SIZE,
            n->e->y * TILE_SIZE + TILE_SIZE,
            n->e->type <= DRAGON ? (al_map_rgb_f(1,0,0)) : (al_map_rgb_f(0,0,1))
        );
        al_draw_text(
            font,
            al_map_rgb_f(1, 1, 1),
            n->e->x * TILE_SIZE + TILE_SIZE / 2,
            n->e->y * TILE_SIZE + TILE_SIZE / 2,
            1,
            &n->e->type
            );
    }
}

void draw_hud(ALLEGRO_FONT* font)
{
    if (swd)
        al_draw_filled_circle(
            TILE_SIZE * 8 + TILE_SIZE,
            TILE_SIZE * 5,
            TILE_SIZE / 4,
            al_map_rgb_f(1, 1, 0)
        );
    if (swd_x)
        al_draw_filled_circle(
            TILE_SIZE * 8 + TILE_SIZE,
            TILE_SIZE * 4,
            TILE_SIZE / 4,
            al_map_rgb_f(1, 0, 0)
        );
    if (shd)
        al_draw_filled_circle(
            TILE_SIZE * 8 + TILE_SIZE,
            TILE_SIZE * 3,
            TILE_SIZE / 4,
            al_map_rgb_f(0, 1, 0)
        );
    if (shd_x)
        al_draw_filled_circle(
            TILE_SIZE * 8 + TILE_SIZE,
            TILE_SIZE * 2,
            TILE_SIZE / 4,
            al_map_rgb_f(0, 0, 1)
        );
    
    al_draw_textf(
        font,
        al_map_rgb_f(1, 1, 1),
        TILE_SIZE * 8,
        TILE_SIZE / 4,
        0,
        "POW:%d",
        str
    );
}

int main()
{
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(DISP_W, DISP_H);
    must_init(disp, "display");

    buffer = al_create_bitmap(BUFFER_W, BUFFER_H);
    must_init(buffer, "buffer");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    bool pressed = false;

    ALLEGRO_EVENT event;

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);

    init_queues();
    fill_level(level_1);

    while (1)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            if (key[ALLEGRO_KEY_ESCAPE])
                done = true;

            if (turn == 0)
                move_player(key, &pressed);

            for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                key[i] &= KEY_SEEN;

            redraw = true;
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
            break;

        case ALLEGRO_EVENT_KEY_UP:
            key[event.keyboard.keycode] &= KEY_RELEASED;
            pressed = false;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;
        }

        if (done)
            break;

        if (dead)
            restart_level(level_1);

        if (turn == 1)
        {
            move_monsters();
            turn = 0;
        }

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_set_target_bitmap(buffer);

            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_player();
            draw_map();
            draw_objects(font);

            draw_hud(font);

            al_set_target_backbuffer(disp);
            al_draw_scaled_bitmap(buffer, 0, 0, BUFFER_W, BUFFER_H, 0, 0, DISP_W, DISP_H, 0);

            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    destroy_entities(all);

    destroy_queue(all);
    destroy_queue(walls);
    destroy_queue(potions);
    destroy_queue(wizards);

    return 0;
}