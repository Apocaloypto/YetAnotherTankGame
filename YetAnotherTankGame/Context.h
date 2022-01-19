#pragma once
#include "Typedefs.h"


// ************************************************************************************************
class CTileMap;
class CTankUsing;

// ************************************************************************************************
class CContext
{
   friend CContext &Context();

private:
   CContext() = default;

   CTankUsing *m_pPlayersTank = nullptr;

public:
   static const String PLAYER_ID;

   CTileMap *m_pCurrentMap = nullptr;

   const CTankUsing *GetPlayersTank() const { return m_pPlayersTank; }
   bool SpawnPlayerOnMap();

   void Reset();

   Meter ToMeter(Real what) const;
};

// ************************************************************************************************
CContext &Context();
