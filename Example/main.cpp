#include "treeUtils.h"

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

    // Inserts element in subtree with root 12.

    // ...
    //  12
    // / \
    //-1  NULL

    node *minusPointer = pushLeafToNode(twelvePointer, -1);

    // ...
    //  12
    // / \
    //-1  1

    node *onePointer   = pushLeafToNode(twelvePointer,  1);

    // You can also use insert without saving it return value.

    // Twelve already hasn't leafs and element pushes to left/right leaf.
    // Zero is greater than minus one and it pushes to node with one.

    // ...
    //  12
    // / \
    //-1  1
    //    |
    //    0

    pushLeafToNode(twelvePointer, 0);

    // Dumps info about tree with pointers.

    simpleTreeDump(Tree1, stdout);

    // Destructs tree.
    
    treeDestructor(Tree1);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
