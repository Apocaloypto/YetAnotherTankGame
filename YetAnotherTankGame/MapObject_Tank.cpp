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
CCollisionRect CMapObjectTank::GetCollisionRect(const CPixelPos &pos) const
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      return m_pUsing->GetCollisionRect(pos);
   }
   else
   {
      return CCollisionRect();
   }
}

// ************************************************************************************************
CPhysicalData CMapObjectTank::GetPhysicalData() const
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      return m_pUsing->GetPhysicalData();
   }
   else
   {
      return CPhysicalData();
   }
}

// ************************************************************************************************
CTilePosAndRot CMapObjectTank::PreUpdate() const
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      return m_pUsing->PreUpdate();
   }

   return CTilePosAndRot(CTilePos(0, 0), 0);
}

// ************************************************************************************************
void CMapObjectTank::Update(const CTilePosAndRot &values, bool collision)
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      m_pUsing->DoUpdate(values, collision);
   }
}

// ************************************************************************************************
void CMapObjectTank::OnCollisionWithTileMap(bool xcoll, bool ycoll, Int32 tilesize)
{
}

// ************************************************************************************************
void CMapObjectTank::OnCollisionWithEndOfMap(bool xcoll, bool ycoll, Int32 tilesize)
{
}

// ************************************************************************************************
void CMapObjectTank::OnCollisionWithMapObject(const CCollisionRect &thisrect, const CCollisionRect &otherrect, const CPhysicalData &otherphysicaldata)
{
   if (Memory().m_TankUsings.IsValid(m_pUsing))
   {
      m_pUsing->OnCollisionWithMapObject(thisrect, otherrect, otherphysicaldata);
   }
}
