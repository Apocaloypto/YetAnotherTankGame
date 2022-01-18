#pragma once
#include "ResHolderMapBaseT.h"


class CImage;

class CResHolderImages : public CResHolderMapBaseT<CImage *>
{
protected:
   virtual void DeleteElement(CImage *pObj) override;

public:
   virtual bool Load(const String &id, const String &filename, const String &fileextra) override;
};
