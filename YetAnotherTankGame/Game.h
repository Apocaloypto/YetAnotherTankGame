#pragma once
#include "Paths.h"
#include "Logging.h"


// ************************************************************************************************
enum class GameState
{
   InitializeContext,
   InGame,
};

// ************************************************************************************************
class CGame
{
   friend CGame &Game();

private:
   CGame();

   CLogger m_Logger;
   CPaths m_Paths;
   GameState m_State;

   void DoInitializeContext();
   void DoInGameFrame();

public:
   bool Initialize();
   void Frame();
   bool IsEnding() const;
   void ShutDown();
};

// ************************************************************************************************
CGame &Game();
