#pragma once
#include "TypeDefs.h"
#include "TemplateUsings.h"
#include "TilePosAndRot.h"
#include "CollisionRect.h"


// ************************************************************************************************
class ITileMapObject
{
public:
   virtual ~ITileMapObject() {}

   virtual const CTilePos &GetPosition() const = 0;
   virtual Degrees GetRotation() const = 0;

   virtual void Draw(const CPixelPos &screen) = 0;

   virtual CTilePosAndRot PreUpdate() const = 0;
   virtual void Update(const CTilePosAndRot &values) = 0;

   virtual CCollisionRect GetCollisionRect() const = 0;

   virtual void OnCollisionWithTileMap(bool xcoll, bool ycoll, Int32 tilesize) = 0;
   virtual void OnCollisionWithEndOfMap(bool xcoll, bool ycoll, Int32 tilesize) = 0;
};
