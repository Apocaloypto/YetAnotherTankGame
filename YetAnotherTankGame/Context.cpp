#include "Context.h"

#include "Tilemap.h"
#include "Memory.h"
#include "Tanks.h"
#include "MapObject_Tank.h"
#include "ResourceManager.h"
#include "Control_Player.h"


// ################################################################################################
const String CContext::PLAYER_ID = "ThePlayer1337";

// ************************************************************************************************
void CContext::Reset()
{
   m_pCurrentMap = nullptr;
}

// ************************************************************************************************
bool CContext::SpawnPlayerOnMap()
{
   if (Memory().m_Maps.IsValid(m_pCurrentMap))
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

      CTankBlueprint *pBlueprint = Resources().TankBlueprints.Get("TANK01");
      if (!Memory().m_TankBlueprints.IsValid(pBlueprint))
      {
         return false;
      }

      m_pPlayersTank = Memory().m_TankUsings.Add(new CTankUsing(pBlueprint, CTilePos(15, 15), 30, 0, pPlayerCtrl));
      if (!Memory().m_TankUsings.IsValid(m_pPlayersTank))
      {
         return false;
      }

      CMapObjectTank *pMapObj = Memory().m_MapObjects.AddTyped(new CMapObjectTank(m_pPlayersTank));
      if (!Memory().m_MapObjects.IsValid(pMapObj))
      {
         return false;
      }

      m_pCurrentMap->AddMapObject(PLAYER_ID, pMapObj);

      return true;
   }
   else
      return false;
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
