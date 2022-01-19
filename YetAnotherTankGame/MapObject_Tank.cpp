#include "MapObject_Tank.h"
#include "Memory.h"


// ################################################################################################
CMapObjectTank::CMapObjectTank(CTankUsing *pTankUsing)
   : m_pUsing(pTankUsing)
{
}

// ************************************************************************************************
const CTilePos &CMapObjectTank::GetPosition() const
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      return m_pUsing->Position;
   }
   else
   {
      static CTilePos dummy;
      return dummy;
   }
}

// ************************************************************************************************
Degrees CMapObjectTank::GetRotation() const
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
      return m_pUsing->Rotation;
   else
      return 0;
}

// ************************************************************************************************
void CMapObjectTank::Draw(const CPixelPos &screen)
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      m_pUsing->Draw(screen);
   }
}

// ************************************************************************************************
void CMapObjectTank::Update()
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      m_pUsing->Update();
   }
}
