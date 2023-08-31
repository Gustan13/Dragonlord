#pragma once

#define MINIMOY 'a'
#define B_GOO   'b'
#define WIZARD  'c'
#define TINWKY  'd'
#define CHONK   'e'
#define MEGAMOY 'f'
#define DRAGON  'g'
#define KNIGHT  'h'
#define SPOTION 'i'
#define SWORD   'j'
#define SHIELD  'k'
#define USHIELD 'l'
#define ESWORD  'm'
#define KIWI    'n'
#define WALL    'o'

const char* level_1 = "a115237c70d16h44i24j66o33435334";
const char* level_2 = "h44";

typedef struct entity {
    short id;
    short x, y, steps, str;
    char type;
} ent;

typedef struct node {
    ent* e;
    struct node* next;
} node_t;

typedef struct queue {
    int size;
    struct node* first;
} queue_t;

queue_t* all = NULL;
queue_t* wizards = NULL;
queue_t* potions = NULL;
queue_t* walls = NULL;

int plr_x = 0;
int plr_y = 0;

int str = 1;

bool shd = false;
bool shd_x = false;

bool swd = false;
bool swd_x = false;

bool dead = false;

int turn = 0;