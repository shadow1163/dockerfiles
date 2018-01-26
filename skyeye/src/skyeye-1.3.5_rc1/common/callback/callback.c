/*
        callback.c - implementation of callback 
        Copyright (C) 2003-2007 Skyeye Develop Group
        for help please send mail to <skyeye-developer@lists.sf.linuxforum.net>

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program; if not, write to the Free Software
        Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
/*
 * 06/22/2009   Michael.Kang  <blackfin.kang@gmail.com>
 */
#include <stdio.h>
#include "skyeye_mm.h"
#include "skyeye_callback.h"

/**
* @brief the queue of call back
*/
typedef struct callback_queue{
	callback_kind_t kind;
	callback_func_t* func;
	callback_func_t* next;
}callback_queue_t;

/**
* @brief the array of call back queue
*/
callback_queue_t callbacks[Max_callback];

/**
* @brief initilization of the given callback
*
* @param kind
*/
static void init_callback_queue(callback_kind_t kind){
	if(kind >= Max_callback){
		/* some error happens */
		return;
	}
	callback_queue_t* queue = &callbacks[kind];
	queue->kind = kind;
	queue->func = NULL;
	queue->next = NULL;
}

/**
* @brief add a callback function to a callback queue
*
* @param func the callback function
* @param kind the callback type
*
* @return 
*/
static exception_t add_callback_func(callback_func_t func, callback_kind_t kind){
	callback_queue_t* queue = &callbacks[kind];	
	/*
	 * if function of head node is NULL, we just put func into the head node.
	 */
	if(queue->func == NULL){
		queue->func = func;
		return No_exp;
	}
	callback_queue_t* node = skyeye_mm(sizeof(callback_queue_t));
	if(node == NULL){
		return Malloc_exp;
	}
	node->kind = kind;
	node->func = func;
	/*
	 * we put the new node into the queue->next.
	 */
	node->next = queue->next;
	queue->next = node;

	return No_exp;
}

/* remove a callback function from queue */
static void remove_callback(){
}

/**
* @brief initializtion of callback module
*/
void init_callback(){
	int i = 0;
	while(i < Max_callback){
		init_callback_queue(i++);
		//memset(&callbacks[i], 0, sizeof(callback_queue_t));
	}
}

/**
* @brief Add a callback handler to a given callback queue
*
* @param func
* @param kind
*/
void register_callback(callback_func_t func, callback_kind_t kind){
	int i;
	for(i = 0; i < Max_callback; i++)
		if(callbacks[i].kind == kind)
			break;	
	/*
	 * we have not found any match callback queue
	 */
	if((i == Max_callback) && (i != kind)){
		return;
	}
	/* put func to the corresponding queue */	
	add_callback_func(func, kind);
}

/**
* @brief scan the callback queue and execute the function in the queue.
*
* @param kind the callback type
* @param arch_instance
*
* @return 
*/
int exec_callback(callback_kind_t kind, generic_arch_t* arch_instance){
	callback_queue_t* queue = &callbacks[kind];
	while(queue){
		callback_func_t func = queue->func;
		if(func != NULL)
			func(arch_instance);
		queue = queue->next;
	}

	return No_exp;
}
