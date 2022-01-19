#pragma once
#include "Typedefs.h"
#include "TemplateUsings.h"


// ************************************************************************************************
class CTileMap;
class CTankUsing;
class IController;

// ************************************************************************************************
class CContext
{
   friend CContext &Context();

private:
   CContext() = default;

   CTankUsing *m_pPlayersTank = nullptr;

   bool SpawnOnCurrentMap(const String &mapobjid, const String &tankblueprintid, IController *pController, const CTilePos &pos, Degrees rot, Degrees towerrot, CTankUsing *&pCreatedTankUsing);

public:
   static const String PLAYER_ID;

   CTileMap *m_pCurrentMap = nullptr;

   const CTankUsing *GetPlayersTank() const { return m_pPlayersTank; }
   bool SpawnPlayerOnMap();
   bool SpawnEnemyOnMap(const String &mapobjid, const CTilePos &pos, Degrees rot, Degrees towerrot);

   void Reset();

   Meter ToMeter(Real what) const;
};

// ************************************************************************************************
CContext &Context();
