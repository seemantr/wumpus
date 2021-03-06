/**
 * Copyright (C) 2007 Stefan Buettcher. All rights reserved.
 * This is free software with ABSOLUTELY NO WARRANTY.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA
 **/

/**
 * Definition of the class AVLTree. Adapted from an earlier non-object-oriented
 * implementation. This tree is supposed to be memory-efficient. Therefore, we
 * do not allow it to have more than 32000 nodes, so that we can use 16-bit
 * integers as pointers.
 *
 * author: Stefan Buettcher
 * created: 2004-09-18
 * changed: 2004-10-20
 **/


#ifndef __MISC__AVLTREE_H
#define __MISC__AVLTREE_H


#include <sys/types.h>
#include "stringbuffer.h"
#include "../index/lexicon.h"
#include "../misc/all.h"


class Lexicon;


/**
 * AVLTreeNode instances are used for representing the nodes of the
 * AVLTree. They have two 32-bit integers (key and value).
 **/
typedef struct {

	/** This is all we can carry: a 32-bit integer. **/
	int32_t value;

	/** The height of this node (used for rebalancing). **/
	int16_t height;

	/** Left and right child nodes (-1 for "non-existent"). **/
	uint16_t leftChild, rightChild;

	/** Parent of the node (0 for the root node). **/
	uint16_t parent;

} AVLTreeNode;


class AVLTree {

public:

	/**
	 * Maximum number of nodes we can manage. This number has to be stricly less
	 * than 65535 (since we are using 16-bit integers).
	 **/
	static const int MAX_NODE_COUNT = 16382;

	/**
	 * If a node's parent value is equal to this value, it means that the node
	 * does not exist and can be used by "createNode".
	 **/
	static const uint16_t EMPTY_NODE = MAX_NODE_COUNT;

private:

	Lexicon *owner;

	/** A simple array of nodes. **/
	AVLTreeNode *nodes;

	/** Number of nodes in the tree. **/
	int nodeCount;

	/** Node index of the root node. **/
	int root;

	/** A list of free nodes in the array. **/
	uint16_t *freeNodes;

	/** Number of free nodes. **/
	int freeNodeCount;

public:

	/**
	 * Creates a new AVLTree that can store terms managed by the Lexicon instance
	 * given by "lexicon".
	 **/
	AVLTree(Lexicon *lexicon);

	/** Deletes the object. **/
	~AVLTree();

	/**
	 * If there is a node within the tree whose "key" member points to the string
	 * given by "key", the method returns a reference to the node. NULL otherwise.
	 **/
	AVLTreeNode *findNode(char *key);

	/** Returns the number of nodes in the tree. **/
	int getNodeCount();

	/**
	 * Returns the number of the node whose "key" member corresponds to the value
	 * of parameter "key". -1 if there is no such node.
	 **/
	int getNodeNumber(char *key);

	/**
	 * Returns a pointer to a sorted list of the elements (data fields) stored
	 * in the tree. The memory has to be freed by the caller.
	 **/
	int32_t *createSortedList();

	/**
	 * Returns a pointer to a sorted list of the elements (data fields) stored in
	 * the subtree rooted at "node". The memory has to be freed by the caller.
	 **/
	int32_t *createSortedList(int node);

	/**
	 * Inserts a new node into the tree, with node value as given by the
	 * parameter "value". Returns 0 on success, -1 on error.
	 **/
	int insertNode(int32_t value);

	/**
	 * Removes the node with key "key" from the tree. Returns 0 if successful,
	 * -1 otherwise.
	 **/
	int deleteNode(char *key);

	/**
	 * Removes the node with index "node" from the tree. Returns 0 if successuful,
	 * -1 otherwise.
	 **/
	int deleteNode(int node);

private:

	/** Returns the size of the subtree rooted at node "node". **/
	int getSizeOfSubtree(int node);

	/** Internal helper function for "createSortedList". **/
	int32_t *storeSortedList(int node, int32_t *array);

	/** Returns the height of the node given by "node". **/
	int getNodeHeight(int node);

	void rotate(int parent, int child);

	void rebalanceHereOrAbove(int node);

	int createNode(int32_t value);

}; // end of class AVLTree


#endif

