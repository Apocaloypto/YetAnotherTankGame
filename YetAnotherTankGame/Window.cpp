#include "Window.h"

#include "Logging.h"
#include "Const.h"
#include "Settings.h"


// ################################################################################################
CWindow *pWindow = nullptr;
CLogger Logger("Window");

// ################################################################################################
void InitializeWindow()
{
   if (!pWindow)
   {
      pWindow = new sf::RenderWindow(sf::VideoMode(Settings().ScreenWidth, Settings().ScreenHeight), Const::APP_TITLE + " [v" + Const::APP_VERSION + "]");
      pWindow->setFramerateLimit(Settings().FrameLimit);
   }
   else
   {
      Logger.Log(LogType::Warning, "RenderWindow is already initialized, doing nothing...");
   }
}

// ************************************************************************************************
void DestroyWindow()
{
   if (pWindow)
   {
      delete pWindow;
      pWindow = nullptr;
   }
   else
   {
      Logger.Log(LogType::Warning, "RenderWindow is alread destructed, doing nothing...");
   }
}

// ************************************************************************************************
sf::RenderWindow &Window()
{
   if (!pWindow)
      Logger.Log(LogType::Error, "RenderWindow not initialized but accessed, app might crash!");

   return *pWindow;
}
