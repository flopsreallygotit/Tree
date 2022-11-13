# Akinator
Simple fun program on tree, tries to guess what you guessed.
Works only on Linux.

# Tree
Binary tree data structure.
For average wood enjoyers.

![image](https://user-images.githubusercontent.com/89828695/200059067-19c18d16-3e0b-4587-a66a-29a9ee777e42.png)

# Main commands

## Constructor and destructor
Constructs and destructs tree:
```
stack *myCoolTree = treeConstructor();
treeDestructor(myCoolTree);
```

## Tree root
Returns pointer to root of tree:
```
node *coolRoot = treeRoot(myCoolTree);
```

## Insert and push
Inserts element in tree:
```
treeInsert(myCoolTree, 1);
```
Inserts element in subtree (Pushes to node):
```
pushLeafToNode(coolRoot, 2);
```
Pushes element in root:
```
treeRootPush(myCoolTree, 12);
```

## Dump
Dumps info about tree in file:
```
treeDump(myCoolTree);
```

# Quick Start
First clone repo:
```
git clone "https://github.com/flopsreallygotit/Tree.git"
```

To play in akinator switch to Tree/Akinator/ and write:
```
make
./main
```
