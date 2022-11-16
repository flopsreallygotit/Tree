#include "akinatorUtils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (const int argc, const char *argv[])
{
    setlocale(LC_CTYPE, "");

    const char *filename = simpleCommandLineParser(argc, argv);

    CHECKERROR(filename != NULL &&
               "Error in filenameparsing", 
               -1);

    tree *Tree = openAndParseFile(filename);

    CHECKERROR(Tree     != NULL && 
               "Error in file parsing.", 
               -1);


    FILE *file = fopen("output.htm", "w");

    treeDump(Tree, "Dump before changes.", file);

    startGame(Tree);

    changeDataBase(Tree, filename);
    
    treeDump(Tree, "Dump after changes.", file);

    treeDestructor(Tree);
    fclose(file);

    return 0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
