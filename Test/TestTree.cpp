#include "Test/TestTree.h"

/*
 *  Tests creation of TreeState and initial values
 */
void TestTree::emptyTree()
{
    TreeState* tree = new TreeState();

    //root node should be highligited as created
    QCOMPARE(tree->getRoot(),tree->getHighlighted());
    delete tree;
}

/*
 *  Tests equality of TreeState
 */
void TestTree::equalEmptyTree()
{
    TreeState* tree = new TreeState();
    TreeState* tree2 = new TreeState();

    //root node should be highligited as created
    QCOMPARE(TreeState::equals(tree, tree2),true);
    delete tree;
}

/*
 *  Tests equality of TreeState
 */
void TestTree::equalNodeTree()
{
    TreeState* tree = new TreeState();
    TreeState* tree2 = new TreeState();

    tree->addChildStatement("S");
    tree2->addChildStatement("S");

    //root node should be highligited as created
    QCOMPARE(TreeState::equals(tree, tree2),true);
    delete tree;
}
