#pragma once
#include "Paths.h"
#include "Logging.h"


// ************************************************************************************************
class CGame
{
   friend CGame &Game();

private:
   CGame();

   CLogger m_Logger;
   CPaths m_Paths;

public:
   bool Initialize();
   void Frame();
   bool IsEnding() const;
   void ShutDown();
};

// ************************************************************************************************
CGame &Game();
