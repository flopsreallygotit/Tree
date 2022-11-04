#ifndef TREE_UTILS_H
#define TREE_UTILS_H

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "universalUtils.h"
#include "treeConfig.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

enum ISERROR
{
    /// @brief Shows that function fulfilled without errors.
    NOTERROR        = 0,

    /// @brief Pointer is NULL.
    NULLPOINTER     = 1,

    /// @brief Value of variable is NAN, inf or overflow max limit of it type.
    ISNOTFINITE     = 2,

    /// @brief Left struct canary died.
    LEFTCANARY      = 3,

    /// @brief Right struct canary died.
    RIGHTCANARY     = 4,

    /// @brief Tree is wrong.
    WRONGTREE       = 5,

    /// @brief Tree size is wrong or capacity is bigger than maxSize.
    WRONGSIZE       = 6,

    /// @brief General error.
    ERROR           = 7,

    /// @brief Error in memory allocation.
    ALLOCATIONERROR = 8,
};

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

/// @brief Destructs tree.
/// @param tree Pointer to tree.
void treeDestructor (tree *Tree);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Returns pointer to tree root.
/// @param Tree Pointer to tree struct.
node *treeRoot (tree *Tree);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Inserts element in tree.
/// @param Tree Pointer to tree.
/// @param element Element.
node *treeInsert (tree *Tree, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Pushes new leaf to subtree.
/// @param Node Pointer to node (subtree).
/// @param element Element.
node *pushLeafToNode (node *Node, elem_t element);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// @brief Dumps tree. (Shell for simpleDumpFunction())
/// @param Tree Pointer to tree.
/// @param file Pointer to file. If you haven't enter it uses stdout.
#define simpleTreeDump(Tree, file) \
    simpleTreeDumpFunction(Tree, #Tree, __FILE__, __LINE__, __PRETTY_FUNCTION__, file)

ISERROR simpleTreeDumpFunction (const tree *Tree, const char *treename, const char *filename, 
                                const int   line, const char *function, FILE *file = stdout);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif