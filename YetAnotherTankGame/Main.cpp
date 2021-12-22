#include <iostream>

#include "ExitCodes.h"
#include "Logging.h"
#include "Window.h"
#include "InputStates.h"

#ifdef DEBUG_MODE
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


using namespace std;

int main(int argc, char **argv)
{
   static_assert(sizeof(void *) == 8, "Enable 64-Bit-Support!");

#ifdef DEBUG_MODE
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

   GENERAL_LOG.Log(LogType::Message, "Starting game...");

   InitializeWindow();

   while (Window().isOpen())
   {
      sf::Event event;
      while (Window().pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
         {
            Window().close();
         }
      }

      Input().Update();

      Window().clear();

      // Draw the frame:

      Window().display();
   }

   DestroyWindow();

   GENERAL_LOG.Log(LogType::Message, "Game ended normally");

   return EXIT(ExitCode::Success); 
}
