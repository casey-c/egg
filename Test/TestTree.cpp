#include "Test/TestTree.h"

TestTree::TestTree()
{

}

void TestTree::testCompare()
{
    QCOMPARE(2, 3); //QCOMPARE( actual, expected)
    QCOMPARE("TEST","TEST");
}

