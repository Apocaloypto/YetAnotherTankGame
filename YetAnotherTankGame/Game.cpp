#include "Game.h"
#include "ResourceManager.h"
#include "Settings.h"
#include "Const.h"
#include "Context.h"
#include "Memory.h"


// ################################################################################################
CGame::CGame()
   : m_Logger("Game"),
   m_State(GameState::InitializeContext)
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
void CGame::DoInitializeContext()
{
   Context().m_pCurrentMap = Resources().TileMaps.Get("MAP01");

   m_State = GameState::InGame;
}

// ************************************************************************************************
void CGame::DoInGameFrame()
{
   if (Memory().m_Maps.IsValid(Context().m_pCurrentMap))
   {
      Context().m_pCurrentMap->Draw(CTilePos(0, 0));
   }
}

// ************************************************************************************************
void CGame::Frame()
{
   switch (m_State)
   {
   case GameState::InitializeContext:
      DoInitializeContext();
      break;
   case GameState::InGame:
      DoInGameFrame();
      break;
   }
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
