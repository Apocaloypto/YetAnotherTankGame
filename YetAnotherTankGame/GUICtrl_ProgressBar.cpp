#include "GUICtrl_ProgressBar.h"

#include "Functions.h"


// ################################################################################################
CGUICtrl_ProgressBar::CGUICtrl_ProgressBar(Int32 vx, Int32 vy, Int32 vw, Int32 vh, const std::function<Real()> &valgetter, const sf::Color &foreground)
   : CGUIControl(vx, vy, vw, vh),
   m_ValGetter(valgetter),
   m_ForeGround(foreground)
{
}

// ************************************************************************************************
void CGUICtrl_ProgressBar::Draw() const
{
   DrawFun::DrawRect(GetPos(), GetDim(), CGUIControl::GUI_BACKGROUND_COLOR, CGUIControl::GUI_BORDER_WIDTH, CGUIControl::GUI_FOREGROUND_COLOR);

   Real filling = m_ValGetter() * GetDim().m_Width;
   Real height = GetDim().m_Height;

   DrawFun::DrawRect(GetPos(), CPixelDim(filling, height), m_ForeGround, 0);
}
