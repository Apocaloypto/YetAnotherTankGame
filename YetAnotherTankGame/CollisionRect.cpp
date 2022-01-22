#include "CollisionRect.h"

#include "Window.h"
#include "Functions.h"
#include "Line2D.h"
#include <cassert>


// ################################################################################################
CCollisionRect::CCollisionRect()
   : CCollisionRect(CPixelDim(), CPixelPos(), CPixelPos(), 0)
{
}

// ************************************************************************************************
CCollisionRect::CCollisionRect(const CPixelDim &dimension, const CPixelPos &pos, const CPixelPos &handle, Degrees rotation)
   : m_Dimension(dimension),
   m_Handle(handle),
   m_Rotation(rotation),
   m_Position(pos)
{
}

// ************************************************************************************************
CPixelPos CCollisionRect::GetHandlePosition() const
{
   return m_Position + m_Handle;
}

// ************************************************************************************************
CCollisionRect CCollisionRect::CopyRotated(const CPixelPos &aroundwhat, Degrees rotation) const
{
   CPixelPos newposition = MathFun::RotateAround(m_Position, aroundwhat, rotation);
   return CCollisionRect(m_Dimension, newposition, m_Handle, m_Rotation + rotation);
}

// ************************************************************************************************
std::map<CCollisionRect::Edge, CPixelPos> CCollisionRect::GetEdges() const
{
   CPixelPos ulunrotated = CPixelPos(m_Position.m_X - m_Handle.m_X, m_Position.m_Y - m_Handle.m_Y);
   CPixelPos urunrotated = CPixelPos(m_Position.m_X + (m_Dimension.m_Width - m_Handle.m_X), m_Position.m_Y - m_Handle.m_Y);

   CPixelPos blunrotated = CPixelPos(m_Position.m_X - m_Handle.m_X, m_Position.m_Y + (m_Dimension.m_Height - m_Handle.m_Y));
   CPixelPos brunrotated = CPixelPos(m_Position.m_X + (m_Dimension.m_Width - m_Handle.m_X), m_Position.m_Y + (m_Dimension.m_Height - m_Handle.m_Y));

   return {
      {Edge::UpperLeft, MathFun::RotateAround(ulunrotated, m_Position, m_Rotation)},
      {Edge::UpperRight, MathFun::RotateAround(urunrotated, m_Position, m_Rotation)},
      {Edge::BottomLeft, MathFun::RotateAround(blunrotated, m_Position, m_Rotation)},
      {Edge::BottomRight, MathFun::RotateAround(brunrotated, m_Position, m_Rotation)}
   };
}

// ************************************************************************************************
bool CCollisionRect::IsPointInRect(const std::map<Edge, CPixelPos> &chk, const CPixelPos &pt)
{
   const CPixelPos chk_ul = chk.at(Edge::UpperLeft);
   const CPixelPos chk_ur = chk.at(Edge::UpperRight);
   const CPixelPos chk_bl = chk.at(Edge::BottomLeft);

   return pt.m_X >= chk_ul.m_X &&
      pt.m_X <= chk_ur.m_X &&
      pt.m_Y >= chk_ul.m_Y &&
      pt.m_Y <= chk_bl.m_Y;
}

// ************************************************************************************************
bool CCollisionRect::CollidesInternal(const CCollisionRect &thisone, const CCollisionRect &thatone, std::map<Edge, CPixelPos> &thispoints, std::map<Edge, CPixelPos> &thatpoints, std::vector<Edge> &crossingEdges)
{
   crossingEdges.clear();

   Degrees toZero = thisone.m_Rotation > 0 ? -thisone.m_Rotation : thisone.m_Rotation;

   CCollisionRect thisrotated = thisone.CopyRotated(thisone.GetHandlePosition(), toZero);
   CCollisionRect thatrotated = thatone.CopyRotated(thisone.GetHandlePosition(), toZero); // Kein Fehler: Um dieses Handle drehen, damit die Rects noch richtig zueinander stehen!

   thispoints = thisrotated.GetEdges();
   thatpoints = thatrotated.GetEdges();

   bool somePointIn = false;

   for (const auto &[edge, pos] : thatpoints)
   {
      if (IsPointInRect(thispoints, pos))
      {
         crossingEdges.push_back(edge);
         somePointIn = true;
      }
   }

   return somePointIn;
}

// ************************************************************************************************
bool CCollisionRect::Collides(const CCollisionRect &other) const
{
   std::map<Edge, CPixelPos> thispoints;
   std::map<Edge, CPixelPos> thatpoints;
   std::vector<Edge> crossingEdges;

   return
      CollidesInternal(*this, other, thispoints, thatpoints, crossingEdges) ||
      CollidesInternal(other, *this, thispoints, thatpoints, crossingEdges);
}

// ************************************************************************************************
std::optional<CPixelPos> CCollisionRect::GetIntersectionPointInRect(const std::map<Edge, CPixelPos> &checked, const CLine2D<Pixels> &line)
{
   std::optional<CPixelPos> cross;

#define CHECK_CROSS(edge1, edge2) (cross = line.Crosses(CLine2D<Real>(checked.at(Edge::UpperLeft), checked.at(Edge::UpperRight)))).has_value()

   if (CHECK_CROSS(Edge::UpperLeft, Edge::UpperRight))
   {
      return cross.value();
   }

   if (CHECK_CROSS(Edge::UpperRight, Edge::BottomRight))
   {
      return cross.value();
   }

   if (CHECK_CROSS(Edge::BottomLeft, Edge::BottomRight))
   {
      return cross.value();
   }

   if (CHECK_CROSS(Edge::UpperLeft, Edge::UpperRight))
   {
      return cross.value();
   }

   return CPixelPos();

#undef CHECK_CROSS
}

// ************************************************************************************************
CLine2D<Pixels> CCollisionRect::MakeIntersectionLine(const std::map<Edge, CPixelPos> &checked, const CLine2D<Pixels> &line1, const CLine2D<Pixels> &line2)
{
   std::optional<CPixelPos> line1chk = GetIntersectionPointInRect(checked, line1);
   if (!line1chk.has_value())
   {
      assert(false); // Sollte nicht eintreten!
      return CLine2D<Pixels>();
   }

   std::optional<CPixelPos> line2chk = GetIntersectionPointInRect(checked, line2);
   if (!line2chk.has_value())
   {
      assert(false); // Sollte nicht eintreten!
      return CLine2D<Pixels>();
   }

   return CLine2D<Pixels>(line1chk.value(), line2chk.value());
}

// ************************************************************************************************
std::optional<CCollisionRect::Edge> CCollisionRect::GetMissingEdge(const std::vector<Edge> &chk)
{
   std::vector<Edge> cmp = {
      Edge::UpperLeft,
      Edge::UpperRight,
      Edge::BottomLeft,
      Edge::BottomRight
   };

   for (Edge e : chk)
   {
      cmp.erase(std::remove_if(cmp.begin(), cmp.end(), [&](const Edge &t) -> bool { return t == e; }), cmp.end());
   }

   if (cmp.size() == 1)
   {
      return cmp[0];
   }
   else
   {
      return std::nullopt;
   }
}

// ************************************************************************************************
CLine2D<Pixels> CCollisionRect::MakeForSimpleCase(const std::map<Edge, CPixelPos> &checked, Edge baseedge, Edge neighbor1, Edge neighbor2)
{
   CLine2D<Pixels> line1(checked.at(baseedge), checked.at(neighbor1));
   CLine2D<Pixels> line2(checked.at(baseedge), checked.at(neighbor2));

   return MakeIntersectionLine(checked, line1, line2);
}

// ************************************************************************************************
CLine2D<Pixels> CCollisionRect::GetIntersectingLine(const std::map<Edge, CPixelPos> &checkedAgainst, const std::map<Edge, CPixelPos> &checked, const std::vector<Edge> &crossingEdges)
{
   struct CEdgeNeighbor
   {
      const Edge m_NeighborOne;
      const Edge m_NeighborTwo;

      CEdgeNeighbor(Edge one, Edge two)
         : m_NeighborOne(one), m_NeighborTwo(two)
      {
      }
   };

   // zunächst einmal die Nachbarpunkte zu einem Punkt:
   static const std::map<Edge, CEdgeNeighbor> NEIGHBORS = {
      {Edge::UpperLeft, CEdgeNeighbor(Edge::UpperRight, Edge::BottomLeft)},
      {Edge::UpperRight, CEdgeNeighbor(Edge::UpperLeft, Edge::BottomRight)},
      {Edge::BottomLeft, CEdgeNeighbor(Edge::UpperLeft, Edge::BottomRight)},
      {Edge::BottomRight, CEdgeNeighbor(Edge::BottomLeft, Edge::UpperRight)}
   };

   // Abhängig von der Anzahl der crossingEdges nun die Linie ermitteln:
   switch (crossingEdges.size())
   {
   case 1:
   {
      // Idee: Wir checken die beiden Linien, die von der einen Crossing-Edge ausgehen. Die Linie zwischen den
      // beiden Punkten, wo diese Crossing-Edge-Linien kreuzen ist die Intersecting-Linie:
      Edge crossingEdge = crossingEdges[0];

      const CEdgeNeighbor &neighbors = NEIGHBORS.at(crossingEdge);
      return MakeForSimpleCase(checked, crossingEdge, neighbors.m_NeighborOne, neighbors.m_NeighborTwo);
   }
   break;
   case 2:
   {
      // Idee: Durch die zwei Punkte ergeben sich prinzipiell drei linien:
      // 1. die linie zwischen den beiden Punkten
      // 2. die linie von dem einen Punkt zu seinem Nachbarn, der nicht der andere Punkt ist und
      // 3. die linie von dem zweiten Punkt zu seinem Nachbarn, der nicht der erste Punkt ist
      // Wir sind interessiert an den linien 2 und 3, da die linie zwischen den intersecting-punkten 
      // dieser beiden linien die intersecting-linie ist. 
      // wir finden heraus, welche beiden linien das sind, indem wir prüfen, welche edge doppelt vorkommt (GetEdgeMulti),
      // das ist die linie 1, die wir also nicht brauchen und nehmen die übrig gebliebenen:
      const CEdgeNeighbor &neighbor1 = NEIGHBORS.at(crossingEdges[0]);
      const CEdgeNeighbor &neighbor2 = NEIGHBORS.at(crossingEdges[1]);

      std::optional<Edge> endpoint0 = std::nullopt;
      std::optional<Edge> endpoint1 = std::nullopt;

#define CHECK_AGAINST(edge1, edge2) (crossingEdges[0] == edge1 && crossingEdges[1] == edge2) || (crossingEdges[1] == edge1 && crossingEdges[0] == edge2)

      if (CHECK_AGAINST(Edge::UpperLeft, Edge::UpperRight))
      {
         endpoint0 = Edge::BottomLeft;
         endpoint1 = Edge::BottomRight;
      }
      else if (CHECK_AGAINST(Edge::UpperRight, Edge::BottomRight))
      {
         endpoint0 = Edge::UpperLeft;
         endpoint1 = Edge::BottomLeft;
      }
      else if (CHECK_AGAINST(Edge::BottomRight, Edge::BottomLeft))
      {
         endpoint0 = Edge::UpperRight;
         endpoint1 = Edge::UpperLeft;
      }
      else if (CHECK_AGAINST(Edge::BottomLeft, Edge::UpperLeft))
      {
         endpoint0 = Edge::BottomRight;
         endpoint1 = Edge::UpperRight;
      }

#undef CHECK_AGAINST

      if (!endpoint0.has_value() || !endpoint1.has_value())
      {
         assert(false);
         return CLine2D<Pixels>();
      }

      CLine2D<Pixels> line1(checked.at(crossingEdges[0]), checked.at(endpoint0.value()));
      CLine2D<Pixels> line2(checked.at(crossingEdges[1]), checked.at(endpoint1.value()));

      return MakeIntersectionLine(checked, line1, line2);
   }
   break;
   case 3:
   {
      // Idee: Wir suchen den Punkt, der außerhalb des Rechtecks liegt. Danach erfolgt alles wie im
      // ersten Case:
      std::optional<Edge> missing = GetMissingEdge(crossingEdges);
      if (!missing.has_value())
      {
         assert(false); // Sollte nicht eintreten!
         return CLine2D<Pixels>();
      }

      const CEdgeNeighbor &neighbors = NEIGHBORS.at(missing.value());
      return MakeForSimpleCase(checked, missing.value(), neighbors.m_NeighborOne, neighbors.m_NeighborTwo);
   }
   break;
   case 4:
      return CLine2D<Pixels>(); // Das eine Rect ist komplett in dem anderen -> Der Fall wird nicht beachtet.
   default:
      assert(false); // Sollte nicht eintreten!
      return CLine2D<Pixels>();
   }
}

// ************************************************************************************************
std::optional<Pixels> CCollisionRect::GetIntersectingWidthInternal(const CCollisionRect &thisone, const CCollisionRect &thatone)
{
   std::map<Edge, CPixelPos> checkedAgainst;
   std::map<Edge, CPixelPos> checked;
   std::vector<Edge> crossingEdges;

   if (CollidesInternal(thisone, thatone, checkedAgainst, checked, crossingEdges))
   {
      return GetIntersectingLine(checkedAgainst, checked, crossingEdges).GetLength();
   }
   else
   {
      return std::nullopt;
   }
}

// ************************************************************************************************
Pixels CCollisionRect::GetIntersectingWidth(const CCollisionRect &other) const
{
   std::optional<Pixels> intersect = GetIntersectingWidthInternal(*this, other);
   if (intersect.has_value())
   {
      return intersect.value();
   }

   intersect = GetIntersectingWidthInternal(other, *this);
   if (intersect.has_value())
   {
      return intersect.value();
   }

   return 0;
}
