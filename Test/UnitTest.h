#ifndef UNITTEST_H
#define UNITTEST_H

#include "Test/TestTree.h"

/*
 *  UnitTest is a class that contains and runs different types of unit test.
 *  It will create variables from different Test class and statically run the
 *  test cases.
 *
 *
 *  UnitTest will run each test class by using "QTest::qExec()" call for each
 *  test.
 *
 *  For example:
 *
 *  TestTree testTree;
 *  QTest::qExec(&testTree);
 *
 */


class UnitTest
{

public:
    //unused contructor
    UnitTest();

    static void run();
};

#endif // UNITTEST_H
