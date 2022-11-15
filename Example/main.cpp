#include <malloc.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include "treeUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! Warning: FIRST SWITCH elem_t to int in treeConfig.h !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main ()
{
    // Constructs tree.

    tree *Tree1 = treeConstructor;

    // Inserts element in tree.

    //   Root
    //  /   \
    // 12   NULL

    node *twelvePointer = treeInsert(Tree1, 12);

    // File info dumps in.
    // Better use "a" permission to complete 2 or more dumps.

    // Dumps info about tree with pointers.

    // Better use macro for dumps for correct information display.

    FILE *output = fopen("output.htm", "a");
    treeDump(Tree1, "Dump after twelve insert.", output);

    // Inserts element in subtree with root 12.

    //  ...
    //  / \
    // 12 13

    node *thirteenPointer = treeInsert(Tree1, 13);

    //  ...
    //   13
    //  /  \
    // 14  NULL

    node *fourteenPointer = treeInsert(Tree1, 14);

    //  ...
    //   12
    //  /  \
    // 1   NULL

    node *onePointer = treeInsert(Tree1,  1);

    // You can also use insert without saving it return value.

    // Twelve already hasn't leafs and element pushes to left/right leaf.
    // Zero is greater than minus one and it pushes to node with one.

    //  ...
    //   1
    //  /  \
    // 0   NULL

    treeInsert(Tree1, 0);

    // Root: rootPoison (default: 0) changes to 23.
    
    insertRoot(Tree1, 23);

    // One more dump.

    treeDump(Tree1, "Dump of tree № 1", output);

    //  ...
    //   13
    //  /  \
    // 14  100

    treeInsert(Tree1, 100);

    treeDump(Tree1, "I love C <3", output);

    // Don't forget to close output :-)

    fclose(output);

    // Destructs tree.
    
    treeDestructor(Tree1);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
