#pragma once
#ifndef _MONSTERS_
#define _MONSTERS_

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "global.h"
#include "entlib.h"

void move_mstr(ent* monster, short x, short y)
{
	monster->x = x;
	monster->y = y;
}

bool check_col(queue_t* q, short x, short y)
{
	for (node_t* n = q->first; n != NULL; n = n->next)
		if (n->e->x == x && n->e->y == y && (n->e->type < SPOTION || n->e->type > ESWORD))
			return true;
	return false;
}

void move_towards(ent* monster, short x, short y)
{
	short delta_x = x - monster->x;
	short delta_y = y - monster->y;

	short new_x = monster->x, new_y = monster->y;

	if (delta_x != 0)
		new_x = monster->x + delta_x / abs(delta_x);

	if (delta_y != 0)
		new_y = monster->y + delta_y / abs(delta_y);

	if (abs(delta_x) > abs(delta_y))
	{
		if (!check_col(all, new_x, monster->y))
			move_mstr(monster, new_x, monster->y);
		else
			if (!check_col(all, monster->x, new_y))
				move_mstr(monster, monster->x, new_y);
	}
	else
	{
		if (!check_col(all, monster->x, new_y))
			move_mstr(monster, monster->x, new_y);
		else
			if (!check_col(all, new_x, monster->y))
				move_mstr(monster, new_x, monster->y);
	}
}

void follow_player(ent* monster)
{
	move_towards(monster, plr_x, plr_y);
}

void move_twinky(ent* twinky)
{
	double closest_distance = 99;

	node_t* closest_potion = NULL;

	for (node_t* n = potions->first; n != NULL; n = n->next)
	{
		double delta_x = abs(n->e->x - twinky->x);
		double delta_y = abs(n->e->y - twinky->y);

		double distance = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
		if (distance < closest_distance)
		{
			closest_distance = distance;
			closest_potion = n;
		}
	}

	if (closest_potion && closest_distance > 0) {
		move_towards(twinky, closest_potion->e->x, closest_potion->e->y);
	}

	if (closest_distance <= 1) {
		
		ent* entity = closest_potion->e;

		remove_only_node(all, closest_potion->e);
		remove_only_node(potions, closest_potion->e);
		free(entity);
		twinky->type = CHONK;
		twinky->str = 8;
	}
}

void move_monsters()
{
	for (node_t* n = all->first; n != NULL; n = n->next)
		switch (n->e->type)
		{
		case MINIMOY:
			follow_player(n->e);
			break;
		case CHONK:
			follow_player(n->e);
			break;
		case TINWKY:
			move_twinky(n->e);
			break;
		default:
			break;
		}
}

#endif // !_MONSTERS_
