#include <iostream>

#include "ExitCodes.h"
#include "Logging.h"
#include "Window.h"


using namespace std;

int main(int argc, char **argv)
{
   static_assert(sizeof(void *) == 8, "Enable 64-Bit-Support!");

   GENERAL_LOG.Log(LogType::Message, "Starting game...");

   InitializeWindow();



   DestroyWindow();

   GENERAL_LOG.Log(LogType::Message, "Game ended normally");

   return EXIT(ExitCode::Success); 
}
