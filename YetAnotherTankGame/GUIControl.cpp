#include "GUIControl.h"

#include "Settings.h"


// ################################################################################################
const sf::Color CGUIControl::GUI_BACKGROUND_COLOR = sf::Color(0x00000080);
const sf::Color CGUIControl::GUI_FOREGROUND_COLOR = sf::Color(0x000000FF);

// ************************************************************************************************
CGUIControl::CGUIControl(Int32 vx, Int32 vy, Int32 vw, Int32 vh)
   : m_VX(vx),
   m_VY(vy),
   m_VW(vw),
   m_VH(vh)
{
}

// ************************************************************************************************
CGUIControl::~CGUIControl()
{
}

// ************************************************************************************************
Real CGUIControl::CalcModX(Int32 base) const
{
   return base / (Real)VIRTUAL_WIDTH * (Real)Settings().ScreenWidth;
}

// ************************************************************************************************
Real CGUIControl::CalcModY(Int32 base) const
{
   return base / (Real)VIRTUAL_HEIGHT * (Real)Settings().ScreenHeight;
}

// ************************************************************************************************
CPixelPos CGUIControl::GetPos() const
{
   return CPixelPos(CalcModX(m_VX), CalcModY(m_VY));
}

// ************************************************************************************************
CPixelDim CGUIControl::GetDim() const
{
   return CPixelDim(CalcModX(m_VW), CalcModY(m_VH));
}
