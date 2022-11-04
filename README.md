# Akinator
Simple fun program on tree, tries to guess what you guessed.

# Tree
For average wood enjoyers.

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

## Dump
Dumps info about tree in file:
```
simpleTreeDump(myCoolTree);
```

# Quick Start
First clone repo:
```
git clone "https://github.com/flopsreallygotit/Tree.git"
```
For example switch to Tree/Example/ and write:
```
make
./main
```

To play in akinator switch to Tree/Akinator/ and write:
```
make
./main
```
