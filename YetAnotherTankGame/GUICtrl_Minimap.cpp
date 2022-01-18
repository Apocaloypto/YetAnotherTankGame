#include "GUICtrl_Minimap.h"

#include "Functions.h"


// ################################################################################################
CGUICtrl_Minimap::CGUICtrl_Minimap(Int32 vx, Int32 vy, Int32 vw, Int32 vh)
   : CGUIControl(vx, vy, vw, vh)
{
}

// ************************************************************************************************
void CGUICtrl_Minimap::Draw() const
{
   DrawFun::DrawRect(GetPos(), GetDim(), CGUIControl::GUI_BACKGROUND_COLOR, CGUIControl::GUI_BORDER_WIDTH, CGUIControl::GUI_FOREGROUND_COLOR);
}
