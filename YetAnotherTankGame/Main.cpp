#include <iostream>

#include "ExitCodes.h"


using namespace std;

int main(int argc, char **argv)
{
   static_assert(sizeof(void *) == 8, "Enable 64-Bit-Support!");

   cout << "Hallo, Welt!" << endl;
   return EXIT(ExitCode::Success); 
}
