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
      return m_pUsing->m_Pos;
   }
   else
   {
      static CTilePos dummy;
      return dummy;
   }
}

// ************************************************************************************************
void CMapObjectTank::Draw(const CPixelPos &screen)
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      m_pUsing->Draw(screen, m_pUsing->m_Rot);
   }
}

// ************************************************************************************************
void CMapObjectTank::Update()
{
   m_pUsing->m_Rot += 0.5f;
   m_pUsing->m_TowerRot += -0.5f;
}
