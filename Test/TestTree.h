#ifndef TESTTREE_H
#define TESTTREE_H

#include <QtTest/QtTest>
#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 *  TestTree is a unit test that will test creation, deletion, copy and remove
 *  of both TreeNode and TreeState.
 */

class TestTree: public QObject {
    Q_OBJECT

public:

/* Each test will be considered as a slot function */
private slots:

    //Basic Tree Testing
    void emptyTree();

    //Equality Testing
    void equalEmptyTree();
    void equalNodeTree();

};

//QTEST_MAIN(TestTree)

#endif // TESTTREE_H
