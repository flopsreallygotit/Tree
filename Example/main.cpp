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
    // 12  NULL

    node *twelvePointer = treeInsert(Tree1, 12);

    // File info dumps in.
    // Better use "a" permission to complete 2 or more dumps.

    // Dumps info about tree with pointers.

    // Better use macro for dumps for correct information display.

    FILE *output = fopen("output.htm", "a");
    treeDump(Tree1, "Dump after twelve insert.", output);

    // Inserts element in subtree with root 12.

    // ...
    //  12
    // /  \
    //-1  NULL

    node *minusPointer = pushLeafToNode(Tree1, twelvePointer, -1);

    // ...
    //  12
    // /  \
    //-1  1

    node *onePointer   = pushLeafToNode(Tree1, twelvePointer,  1);

    // You can also use insert without saving it return value.

    // Twelve already hasn't leafs and element pushes to left/right leaf.
    // Zero is greater than minus one and it pushes to node with one.

    // ...
    //  12
    // /  \
    //-1  1
    //    |
    //    0

    pushLeafToNode(Tree1, twelvePointer, 0);

    // Root: rootPoison (default: 0) changes to 23.
    
    treeRootPush(Tree1, 23);

    // One more dump.

    treeDump(Tree1, "Dump after twelve subtree insert.", output);

    // Don't forget to close output :-)

    fclose(output);

    // Destructs tree.
    
    treeDestructor(Tree1);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
