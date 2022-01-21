#pragma once
#include "ITileMapObject.h"
#include "Typedefs.h"


class CTankUsing;

class CMapObjectTank : public ITileMapObject
{
private:
   CTankUsing *m_pUsing;

public:
   CMapObjectTank(CTankUsing *pTankUsing);

   virtual const CTilePos &GetPosition() const override;
   virtual Degrees GetRotation() const override;
   virtual void Draw(const CPixelPos &screen) override;
   virtual CTilePosAndRot PreUpdate() const override;
   virtual void Update(const CTilePosAndRot &values, bool collision) override;
   virtual CCollisionRect GetCollisionRect(const CPixelPos &pos) const override;
   virtual void OnCollisionWithTileMap(bool xcoll, bool ycoll, Int32 tilesize) override;
   virtual void OnCollisionWithEndOfMap(bool xcoll, bool ycoll, Int32 tilesize) override;
};
