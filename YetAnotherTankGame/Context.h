#pragma once
#include "Typedefs.h"


// ************************************************************************************************
class CTileMap;

// ************************************************************************************************
class CContext
{
   friend CContext &Context();

private:
   CContext() = default;

public:
   static const String PLAYER_ID;

   CTileMap *m_pCurrentMap;

   bool SpawnPlayerOnMap();

   void Reset();
};

// ************************************************************************************************
CContext &Context();
