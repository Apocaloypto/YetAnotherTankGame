#include "Game.h"
#include "ResourceManager.h"
#include "Settings.h"
#include "Const.h"


// ################################################################################################
CGame::CGame()
   : m_Logger("Game")
{
}

// ************************************************************************************************
bool CGame::Initialize()
{
   if (!m_Paths.Load(Const::PATHS_FILE))
   {
      m_Logger.Log(LogType::Error, "unable to load paths file (" + Const::PATHS_FILE + ")");
      return false;
   }

   String reslist = m_Paths.Get(PathsEntry::ResList);
   if (reslist.empty())
   {
      m_Logger.Log(LogType::Error, "no reslist given");
      return false;
   }

   return Resources().Load(reslist);
}

// ************************************************************************************************
void CGame::Frame()
{
}

// ************************************************************************************************
bool CGame::IsEnding() const
{
   return false;
}

// ************************************************************************************************
void CGame::ShutDown()
{
   Resources().ShutDown();
}

// ################################################################################################
CGame &Game()
{
   static CGame inst;
   return inst;
}
