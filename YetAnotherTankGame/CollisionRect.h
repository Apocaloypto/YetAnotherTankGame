#pragma once
#include "Typedefs.h"
#include "TemplateUsings.h"
#include <SFML/Graphics.hpp>


class CCollisionRect
{
public:
   const CPixelDim m_Dimension;
   const CPixelPos m_Handle;
   const Degrees m_Rotation;

   CCollisionRect(const CPixelDim &dimension, const CPixelPos &handle, Degrees rotation);

   void DebugDraw(const CPixelPos &thispos, const sf::Color &color) const;

   bool Collides(const CCollisionRect &other, const CPixelPos &otherpos, const CPixelPos &thispos) const;
};
