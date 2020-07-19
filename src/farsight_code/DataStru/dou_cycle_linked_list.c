/*
	double_cycle_linked_list
*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


typedef struct double_list {
	int num;
	struct double_list *prev, *next;
} DOU_LIST;

static void add_node_to_list(DOU_LIST **p_entry, int num)
{
	DOU_LIST *node = malloc(sizeof(DOU_LIST));
	node->num = num;
	if (!(*p_entry))  {
		*p_entry = node;
		node->prev = node->next = *p_entry;
	} else {
		(*p_entry)->prev->next = node;
		node->prev= (*p_entry)->prev;
		(*p_entry)->prev = node;
		node->next = (*p_entry);
	}
	return;
}

static int delete_node_from_list(DOU_LIST **p_node)
{
	DOU_LIST *tmpnode;

	if (!(*p_node))//NULL
		return -1;
	
	if (*p_node == (*p_node)->next) {//rest only one
		free(*p_node);
		*p_node = NULL;
		return 0;
	}

	(*p_node)->prev->next = (*p_node)->next;
	(*p_node)->next->prev = (*p_node)->prev;
	tmpnode = (*p_node)->next;
	free(*p_node);
	*p_node = tmpnode;//current node point next node
	return 0;
}

int main(void)
{
	DOU_LIST *entry = NULL;
	int i;

	for (i = 1; i <= 13; i++) 
		add_node_to_list(&entry, i);

	while (entry != entry->next) {
		entry = entry->next->next;//shift rigth twice
		delete_node_from_list(&entry);
	}

	printf("last one number is %d\n", entry->num);
	
	delete_node_from_list(&entry);
	return 0;
}