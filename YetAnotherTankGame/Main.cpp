#include <iostream>

#include "ExitCodes.h"
#include "Logging.h"
#include "Window.h"
#include "InputStates.h"
#include "Game.h"
#include "Functions.h"

#ifdef DEBUG_MODE
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// uncomment this line to build resources:
// #define RESBUILD

#ifdef RESBUILD
#include "ResBuilder/ResBuilder.h"
#endif


using namespace std;

int main(int argc, char **argv)
{
   static_assert(sizeof(void *) == 8, "Enable 64-Bit-Support!");

#ifdef DEBUG_MODE
   _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef RESBUILD
   BuildResources();
   return EXIT(ExitCode::Success);
#endif

   GENERAL_LOG.Log(LogType::Message, "Starting game...");

   InitializeWindow();

   DrawFun::InitGL();
   MathFun::InitRand();

   if (!Game().Initialize())
   {
      GENERAL_LOG.Log(LogType::Error, "Failed to initialize game");
      Game().ShutDown();
      DestroyWindow();
      return EXIT(ExitCode::FailedToInitializeGame);
   }

   while (Window().isOpen() && !Game().IsEnding())
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

      Game().Frame();

      Window().display();
   }

   Game().ShutDown();
   DestroyWindow();

   GENERAL_LOG.Log(LogType::Message, "Game ended normally");

   return EXIT(ExitCode::Success); 
}
