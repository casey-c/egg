#ifndef TESTTREE_H
#define TESTTREE_H

#include <QtTest/QtTest>
#include "Tree/treenode.h"

/*
 *  TestTree is a unit test that will test creation, deletion, copy and remove
 *  of both TreeNode and TreeState.
 */

class TestTree: public QObject {
    Q_OBJECT

public:
    TestTree();

/* Each test will be considered as a slot function */
private slots:
    void testCompare();

};

#endif // TESTTREE_H
