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
   if (m_pCurrentMap)
   {
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

      CTankUsing *pUsing = Memory().m_TankUsings.Add(new CTankUsing(pBlueprint, CTilePos(15, 15), 30, 0, pPlayerCtrl));
      if (!Memory().m_TankUsings.IsValid(pUsing))
      {
         return false;
      }

      CMapObjectTank *pMapObj = Memory().m_MapObjects.AddTyped(new CMapObjectTank(pUsing));
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

// ################################################################################################
CContext &Context()
{
   static CContext inst;
   return inst;
}
