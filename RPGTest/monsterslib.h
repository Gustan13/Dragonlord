#pragma once
#ifndef _MONSTERS_
#define _MONSTERS_

#include <stdlib.h>
#include <stdbool.h>
#include "global.h"

void move_mstr(ent* monster, short x, short y)
{
	monster->x = x;
	monster->y = y;
}

bool check_col(queue_t* q, short x, short y)
{
	for (node_t* n = q->first; n != NULL; n = n->next)
		if (n->e->x == x && n->e->y == y)
			return true;
	return false;
}

void follow_player(ent* monster)
{
	short delta_x = plr_x - monster->x;
	short delta_y = plr_y - monster->y;

	short new_x = monster->x + delta_x / abs(delta_x);
	short new_y = monster->y + delta_y / abs(delta_y);

	if (abs(delta_x) > abs(delta_y))
	{
		if (check_col(all, new_x, monster->y))
			move_mstr(monster, monster->x, new_y);
		else if (check_col(all, monster->x, new_y))
			move_mstr(monster, new_x, monster->y);
	}
	else
	{
		if (check_col(all, monster->x, new_y))
			move_mstr(monster, new_x, monster->y);
		else if (check_col(all, monster->x, new_y))
			move_mstr(monster, monster->x, new_y);
	}
}

#endif // !_MONSTERS_
