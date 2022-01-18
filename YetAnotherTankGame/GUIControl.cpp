#include "GUIControl.h"

#include "Settings.h"


// ################################################################################################
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
Int32 CGUIControl::GetX() const
{
   return (Int32)(m_VX * GetFactX());
}

// ************************************************************************************************
Int32 CGUIControl::GetY() const
{
   return (Int32)(m_VY * GetFactY());
}

// ************************************************************************************************
Int32 CGUIControl::GetW() const
{
   return (Int32)(m_VW * GetFactX());
}

// ************************************************************************************************
Int32 CGUIControl::GetH() const
{
   return (Int32)(m_VH * GetFactY());
}
