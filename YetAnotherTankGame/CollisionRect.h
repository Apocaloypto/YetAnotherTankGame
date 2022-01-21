#pragma once
#include <map>
#include "Typedefs.h"
#include "TemplateUsings.h"
#include <SFML/Graphics.hpp>


class CCollisionRect
{
private:
   enum class Edge
   {
      UpperLeft,
      UpperRight,
      BottomLeft,
      BottomRight,
   };

   CPixelPos GetHandlePosition() const;

   std::map<Edge, CPixelPos> GetEdges() const;
   CCollisionRect CopyRotated(const CPixelPos &aroundwhat, Degrees rotation) const;

   // Debug-Stuff:
   static void DrawEdge(const std::map<Edge, CPixelPos> &edges, Edge from, Edge to);
   static void DrawEdges(const std::map<Edge, CPixelPos> &edges);

   static bool IsPointInRect(const std::map<Edge, CPixelPos> &chk, const CPixelPos &pt);

   static bool CollidesInternal(const CCollisionRect &thisone, const CCollisionRect &thatone);

public:
   const CPixelDim m_Dimension;
   const CPixelPos m_Handle;
   const CPixelPos m_Position;
   const Degrees m_Rotation;

   CCollisionRect();
   CCollisionRect(const CPixelDim &dimension, const CPixelPos &pos, const CPixelPos &handle, Degrees rotation);

   // Debug-Stuff:
   void DebugDraw(const sf::Color &color) const;

   bool Collides(const CCollisionRect &other) const;
};
