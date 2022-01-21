#pragma once
#include <map>
#include <vector>
#include "Typedefs.h"
#include "TemplateUsings.h"
#include "Line2D.h"
#include <SFML/Graphics.hpp>
#include <optional>


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

   std::map<Edge, CPixelPos> GetEdges() const;
   CCollisionRect CopyRotated(const CPixelPos &aroundwhat, Degrees rotation) const;

   static bool IsPointInRect(const std::map<Edge, CPixelPos> &chk, const CPixelPos &pt);

   static bool CollidesInternal(const CCollisionRect &thisone, const CCollisionRect &thatone, std::map<Edge, CPixelPos> &thispoints, std::map<Edge, CPixelPos> &thatpoints, std::vector<Edge> &crossingEdges);

   static CLine2D<Pixels> MakeForSimpleCase(const std::map<Edge, CPixelPos> &checked, Edge baseedge, Edge neighbor1, Edge neighbor2);
   static std::optional<Edge> GetMissingEdge(const std::vector<Edge> &chk);
   static std::optional<CPixelPos> GetIntersectionPointInRect(const std::map<Edge, CPixelPos> &checked, const CLine2D<Pixels> &line);
   static CLine2D<Pixels> MakeIntersectionLine(const std::map<Edge, CPixelPos> &checked, const CLine2D<Pixels> &line1, const CLine2D<Pixels> &line2);
   static CLine2D<Pixels> GetIntersectingLine(const std::map<Edge, CPixelPos> &checkedAgainst, const std::map<Edge, CPixelPos> &checked, const std::vector<Edge> &crossingEdges);
   static std::optional<Pixels> GetIntersectingWidthInternal(const CCollisionRect &thisone, const CCollisionRect &thatone);

public:
   const CPixelDim m_Dimension;
   const CPixelPos m_Handle;
   const CPixelPos m_Position;
   const Degrees m_Rotation;

   CCollisionRect();
   CCollisionRect(const CPixelDim &dimension, const CPixelPos &pos, const CPixelPos &handle, Degrees rotation);

   CPixelPos GetHandlePosition() const;
   Pixels GetIntersectingWidth(const CCollisionRect &other) const;

   bool Collides(const CCollisionRect &other) const;
};
