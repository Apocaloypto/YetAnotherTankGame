#include "Context.h"

#include "Tilemap.h"
#include "Memory.h"
#include "Tanks.h"
#include "MapObject_Tank.h"
#include "ResourceManager.h"
#include "Control_Player.h"
#include "Control_Moron.h"


// ################################################################################################
const String CContext::PLAYER_ID = "ThePlayer1337";

// ************************************************************************************************
void CContext::Reset()
{
   m_pCurrentMap = nullptr;
}

// ************************************************************************************************
bool CContext::SpawnOnCurrentMap(const String &mapobjid, const String &tankblueprintid, IController *pController, const CTilePos &pos, Degrees rot, Degrees towerrot, CTankUsing *&pCreatedTankUsing)
{
   if (Memory().m_Maps.IsValid(m_pCurrentMap))
   {
      if (!Memory().m_Controller.IsValid(pController))
      {
         return false;
      }

      CTankBlueprint *pBlueprint = Resources().TankBlueprints.Get(tankblueprintid);
      if (!Memory().m_TankBlueprints.IsValid(pBlueprint))
      {
         return false;
      }

      pCreatedTankUsing = Memory().m_TankUsings.Add(new CTankUsing(pBlueprint, pos, rot, towerrot, pController));
      if (!Memory().m_TankUsings.IsValid(m_pPlayersTank))
      {
         return false;
      }

      CMapObjectTank *pMapObj = Memory().m_MapObjects.AddTyped(new CMapObjectTank(pCreatedTankUsing));
      if (!Memory().m_MapObjects.IsValid(pMapObj))
      {
         return false;
      }

      return m_pCurrentMap->AddMapObject(mapobjid, pMapObj);
   }
   else
      return false;
}

// ************************************************************************************************
bool CContext::SpawnPlayerOnMap()
{
   if (Memory().m_TankUsings.IsValid(m_pPlayersTank))
   {
      Memory().m_TankUsings.Delete(m_pPlayersTank);
      m_pPlayersTank = nullptr;
   }

   CControllerPlayer *pPlayerCtrl = Memory().m_Controller.AddTyped(new CControllerPlayer());
   if (!Memory().m_Controller.IsValid(pPlayerCtrl))
   {
      return false;
   }

   return SpawnOnCurrentMap(PLAYER_ID, "TANK01", pPlayerCtrl, CTilePos(15, 15), 30, 0, m_pPlayersTank);
}

// ************************************************************************************************
bool CContext::SpawnEnemyOnMap(const String &mapobjid, const CTilePos &pos, Degrees rot, Degrees towerrot)
{
   CControllerMoron *pEnemyCtrl = Memory().m_Controller.AddTyped(new CControllerMoron());
   if (!Memory().m_Controller.IsValid(pEnemyCtrl))
   {
      return false;
   }

   CTankUsing *pUsing = nullptr;
   return SpawnOnCurrentMap(mapobjid, "TANK01", pEnemyCtrl, pos, rot, towerrot, pUsing);
}

// ************************************************************************************************
Meter CContext::ToMeter(Real what) const
{
   if (Memory().m_Maps.IsValid(m_pCurrentMap))
   {
      return what / m_pCurrentMap->TileSet.GetTileSize();
   }
   else
   {
      return 0;
   }
}

// ################################################################################################
CContext &Context()
{
   static CContext inst;
   return inst;
}
