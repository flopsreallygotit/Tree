#ifndef TREE_UTILS_H
#define TREE_UTILS_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "universalUtils.h"
#include "treeConfig.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Shell for Stack Constructor Function.
#define treeConstructor \
    treeConstructorFunction(__FILE__, __LINE__)

/// @brief Constructs tree.
/// @param Tree Pointer to tree.
/// @param filename File where tree was created.
/// @param line Line where tree was created.
tree *treeConstructorFunction (const char *filename, int line);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Constructs node.
/// @param element Element.
/// @param parent Parent of this node / previous node.
node *nodeConstructor (elem_t element, node *parent);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Destructs tree.
/// @param tree Pointer to tree.
void treeDestructor (tree *Tree);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Returns pointer to tree root.
/// @param Tree Pointer to tree struct.
node *treeRoot    (tree *Tree);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Returns pointer to pointer to node parent.
/// @param Node Pointer to node.
node *nodeParent (node *Node);

/// @brief Returns pointer to pointer to left node.
/// @param Node Pointer to node.
node *leftNode   (node *Node);

/// @brief Returns pointer to pointer to right node.
/// @param Node Pointer to node.
node *rightNode  (node *Node);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Returns node data.
/// @param Node Pointer to node.
elem_t nodeData (node *Node);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes element in tree root.
/// @param Tree Pointer to tree.
/// @param element Element.
node *insertRoot (tree *Tree, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes element in left node of given subtree.
/// @param Node Pointer to node.
/// @param element Element.
node *insertLeftLeaf  (node *Node, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes element in left node of given subtree.
/// @param Node Pointer to node.
/// @param element Element.
node *insertRightLeaf (node *Node, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Inserts element in tree.
/// @param Tree Pointer to tree.
/// @param element Element.
node *treeInsert (tree *Tree, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes root to tree.
/// @param Tree Pointer to tree.
/// @param Root Pointer to root node.
ISERROR pushRootToTree (tree *Tree, node *Root);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes new leaf to subtree.
/// @param Node Pointer to current node.
/// @param Leaf lPointer to leaf node (Leaf of current node).
ISERROR pushLeafToNode (node *Node, node *Leaf);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Prints tree in pre-order.
/// @param Tree Poiter to tree.
/// @param file Pointer to file.
ISERROR preorderPrintTree (tree *Tree, FILE *file = stdout);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Dumps tree in htm file. (Shell for dumpFunction())
/// @param Tree Pointer to tree.
/// @param file Pointer to file.
#define treeDump(Tree, message, file) \
    treeDumpFunction(Tree, message, #Tree, __FILE__, __LINE__, __PRETTY_FUNCTION__, file)

ISERROR treeDumpFunction (const tree *Tree,     const char *message,
                          const char *treename, const char *filename, 
                          const int   line,     const char *function, 
                          FILE *file);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
