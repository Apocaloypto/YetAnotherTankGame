#pragma once
#include "ResHolderMapBaseT.h"


class CTileMap;

class CResHolderTileMaps : public CResHolderMapBaseT<CTileMap *>
{
protected:
   virtual void DeleteElement(CTileMap *pObj) override;

public:
   virtual bool Load(const String &id, const String &filename, const String &fileextra) override;
};
