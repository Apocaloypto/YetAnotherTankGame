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
   virtual void Update() override;
};
