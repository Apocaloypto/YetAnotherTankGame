#include "CollisionRect.h"

#include "Window.h"
#include "Functions.h"


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
void CCollisionRect::DebugDraw(const sf::Color &color) const
{
   sf::RectangleShape shape(sf::Vector2f(m_Dimension.m_Width, m_Dimension.m_Height));
   shape.setOrigin(sf::Vector2f(m_Handle.m_X, m_Handle.m_Y));
   shape.setRotation(m_Rotation);
   shape.setPosition(sf::Vector2f(m_Position.m_X, m_Position.m_Y));
   shape.setFillColor(color);

   Window().draw(shape);
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
      { Edge::UpperLeft, MathFun::RotateAround(ulunrotated, m_Position, m_Rotation) },
      { Edge::UpperRight, MathFun::RotateAround(urunrotated, m_Position, m_Rotation) },
      { Edge::BottomLeft, MathFun::RotateAround(blunrotated, m_Position, m_Rotation) },
      { Edge::BottomRight, MathFun::RotateAround(brunrotated, m_Position, m_Rotation) }
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
void CCollisionRect::DrawEdge(const std::map<Edge, CPixelPos> &edges, Edge from, Edge to)
{
   CPixelPos pos1 = edges.at(from);
   CPixelPos pos2 = edges.at(to);

   DrawFun::DrawLine(pos1, pos2, sf::Color(0x0000FFFF));
}

// ************************************************************************************************
void CCollisionRect::DrawEdges(const std::map<Edge, CPixelPos> &edges)
{
   DrawEdge(edges, Edge::UpperLeft, Edge::UpperRight);
   DrawEdge(edges, Edge::UpperRight, Edge::BottomRight);
   DrawEdge(edges, Edge::BottomRight, Edge::BottomLeft);
   DrawEdge(edges, Edge::BottomLeft, Edge::UpperLeft);
}

// ************************************************************************************************
bool CCollisionRect::CollidesInternal(const CCollisionRect &thisone, const CCollisionRect &thatone)
{
   Degrees toZero = thisone.m_Rotation > 0 ? -thisone.m_Rotation : thisone.m_Rotation;

   CCollisionRect thisrotated = thisone.CopyRotated(thisone.GetHandlePosition(), toZero);
   CCollisionRect thatrotated = thatone.CopyRotated(thisone.GetHandlePosition(), toZero); // Kein Fehler: Um dieses Handle drehen, damit die Rects noch richtig zueinander stehen!

   std::map<Edge, CPixelPos> thispoints = thisrotated.GetEdges();
   std::map<Edge, CPixelPos> thatpoints = thatrotated.GetEdges();

   // DrawEdges(thispoints);
   // DrawEdges(thatpoints);

   for (const auto &[edge, pos] : thatpoints)
   {
      if (IsPointInRect(thispoints, pos))
      {
         return true;
      }
   }

   return false;
}

// ************************************************************************************************
bool CCollisionRect::Collides(const CCollisionRect &other) const
{
   return CollidesInternal(*this, other) || CollidesInternal(other, *this);
}
