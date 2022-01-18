#include "GUIControl.h"

#include "Settings.h"


// ################################################################################################
const sf::Color CGUIControl::GUI_BACKGROUND_COLOR = sf::Color(0x00000080);
const sf::Color CGUIControl::GUI_FOREGROUND_COLOR = sf::Color(0x267F00FF);

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
Real CGUIControl::GetFactX() const
{
   return (Real)VIRTUAL_WIDTH / Settings().ScreenWidth;
}

// ************************************************************************************************
Real CGUIControl::GetFactY() const
{
   return (Real)VIRTUAL_HEIGHT / Settings().ScreenHeight;
}

// ************************************************************************************************
CPixelPos CGUIControl::GetPos() const
{
   return CPixelPos(m_VX * GetFactX(), m_VY * GetFactY());
}

// ************************************************************************************************
CPixelDim CGUIControl::GetDim() const
{
   return CPixelDim(m_VW * GetFactX(), m_VH * GetFactY());
}
