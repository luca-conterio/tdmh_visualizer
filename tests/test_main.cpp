
#include "logcontainer_test.h"
#include "configuration_test.h"
int main(int argc, char *argv[])
{
   int status = 0;
   {
          logcontainerTest t;
          status |= QTest::qExec(&t, argc, argv);
   }
   {
          ConfigurationTest t;
          status |= QTest::qExec(&t, argc, argv);
   }
   return status;
}
