#pragma once
#include "ResHolderMapBaseT.h"


class CTankBlueprint;

class CResHolderTankBlueprints : public CResHolderMapBaseT<CTankBlueprint *>
{
protected:
   virtual void DeleteElement(CTankBlueprint *pObj) override;

public:
   virtual bool Load(const String &id, const String &filename) override;
};
