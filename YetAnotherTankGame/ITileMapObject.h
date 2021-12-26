#pragma once
#include "TypeDefs.h"
#include "TemplateUsings.h"


class ITileMapObject
{
public:
   virtual ~ITileMapObject() {}

   virtual const CTilePos &GetPosition() const = 0;

   virtual void Draw(const CPixelPos &screen) = 0;

   virtual void Update() = 0;
};
