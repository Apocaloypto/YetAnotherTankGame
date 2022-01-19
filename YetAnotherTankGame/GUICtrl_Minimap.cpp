#include "GUICtrl_Minimap.h"

#include "Functions.h"
#include "Context.h"
#include "Memory.h"
#include "ResourceManager.h"


// ################################################################################################
const String CGUICtrl_Minimap::NORTH_INDICATOR_RESID = "NORTH";
const String CGUICtrl_Minimap::PLAYER_INDICATOR_RESID = "PLAYER";
const String CGUICtrl_Minimap::ENEMY_INDICATOR_RESID = "ENEMY";

// ************************************************************************************************
CGUICtrl_Minimap::CGUICtrl_Minimap(Int32 vx, Int32 vy, Int32 vw, Int32 vh)
   : CGUIControl(vx, vy, vw, vh)
{
}

// ************************************************************************************************
void CGUICtrl_Minimap::DrawNorthIndicator(Degrees playerRot) const
{
   CImage *pNorthIcon = Resources().Images.Get(NORTH_INDICATOR_RESID);
   if (Memory().m_Images.IsValid(pNorthIcon))
   {
      CPixelPos northIconPos = MathFun::CalcRotatedPointOnOuterEdge(GetDim(), playerRot + 90);

      pNorthIcon->Draw(pNorthIcon->GetCenter(), GetPos() + northIconPos, 0);
   }
}

// ************************************************************************************************
void CGUICtrl_Minimap::Draw() const
{
   DrawFun::DrawRect(GetPos(), GetDim(), CGUIControl::GUI_BACKGROUND_COLOR, CGUIControl::GUI_BORDER_WIDTH, CGUIControl::GUI_FOREGROUND_COLOR);
   if (Memory().m_Maps.IsValid(Context().m_pCurrentMap))
   {
      CImage *pMinimap = Context().m_pCurrentMap->MiniMap();
      if (pMinimap)
      {
         const CTilePos playerPos = Context().m_pCurrentMap->GetMapObjectPos(Context().PLAYER_ID);
         const Degrees playerRot = Context().m_pCurrentMap->GetMapObjectRotation(Context().PLAYER_ID);

         pMinimap->DrawViewPort(GetPos(), GetDim(), playerPos, -playerRot);

         DrawNorthIndicator(-playerRot);
      }
   }
}
