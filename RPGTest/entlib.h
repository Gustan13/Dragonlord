#pragma once
#ifndef _ENT_
#define _ENT_

#include <stdlib.h>
#include "global.h"

void destroy_entities(queue_t* q)
{
	if (!q)
		return;

	for (node_t* n = q->first; n != NULL; n = n->next)
		free(n->e);
}

void destroy_queue(queue_t* q)
{
	if (!q)
		return;

	node_t* aux = NULL;
	for (node_t* n = q->first; n != NULL; n = aux)
	{
		aux = n->next;
		free(n);
	}
}

void remove_object(queue_t* q, node_t* node)
{
	short id = node->e->id;

	node_t* aux = NULL;

	if (q->first->e->id == id)
	{
		aux = q->first->next;
		free(q->first->e);
		free(q->first);
		q->first = aux;
		return;
	}

	aux = q->first;

	for (node_t* n = q->first->next; n != NULL; n = n->next)
	{
		id = node->e->id;

		if (n->e->id == id)
		{
			aux->next = n->next;
			free(n->e);
			free(n);
			break;
		}

		aux = n;
	}

	return;
}

void add_object(queue_t* q, node_t* node)
{
	if (q == NULL)
		return;
	
	if (q->first == NULL)
	{
		q->first = node;
		q->size = 1;
		return;
	}

	for (node_t* n = q->first; n != NULL; n = n->next)
		if (n->next == NULL)
		{
			n->next = node;
			return;
		}
}

short pick_str(char c)
{
	switch (c)
	{
	case MINIMOY:
		return 3;
	case B_GOO:
		return 3;
	case WIZARD:
		return 5;
	case TINWKY:
		return 0;
	case CHONK:
		return 8;
	case MEGAMOY:
		return 10;
	case DRAGON:
		return 12;
	default:
		return 0;
	}
}

void fill_level(const char* level)
{
	char c, type;
	int i = 0;

	c = level[i];
	type = c;

	while (c != '\0')
	{
		if (c > 96)
		{
			if (c == KNIGHT)
			{
				i++;
				c = level[i];
				plr_x = c - 48;
				i++;
				c = level[i];
				plr_y = c - 48;
			}
			else
				type = c;
			i++;
			c = level[i];
			continue;
		}

		ent* entity = malloc(sizeof(ent));

		if (entity)
		{
			entity->id = (short)i;
			entity->steps = 0;
			entity->str = pick_str(type);
			entity->type = type;

			entity->x = (short)(c - 48);
			i++;
			c = level[i];
			entity->y = (short)(c - 48);
			i++;
		}

		if (type == SPOTION)
		{
			node_t* node = malloc(sizeof(node_t));
			if (node)
			{
				node->e = entity;
				node->next = NULL;
				add_object(potions, node);
			}
		}
		else if (type == WIZARD)
		{
			node_t* node = malloc(sizeof(node_t));
			if (node)
			{
				node->e = entity;
				node->next = NULL;
				add_object(wizards, node);
			}
		}
		else if (type == WALL)
		{
			node_t* node = malloc(sizeof(node_t));
			if (node) 
			{
				node->e = entity;
				node->next = NULL;
				add_object(walls, node);
			}
		}

		node_t* node = malloc(sizeof(node_t));
		if (node)
		{
			node->e = entity;
			node->next = NULL;
			add_object(all, node);
		}

		c = level[i];
	}
}

#endif // !_ENT_
