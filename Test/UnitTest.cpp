#include "Test/UnitTest.h"

UnitTest::UnitTest()
{

}

void UnitTest::run()
{
    TestTree testTree;
    QTest::qExec(&testTree);
}

