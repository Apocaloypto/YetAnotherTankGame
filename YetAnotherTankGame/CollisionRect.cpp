#include "CollisionRect.h"

#include "Window.h"


// ################################################################################################
CCollisionRect::CCollisionRect(const CPixelDim &dimension, const CPixelPos &handle, Degrees rotation)
   : m_Dimension(dimension),
   m_Handle(handle),
   m_Rotation(rotation)
{
}

// ************************************************************************************************
void CCollisionRect::DebugDraw(const CPixelPos &thispos, const sf::Color &color) const
{
   sf::RectangleShape shape(sf::Vector2f(m_Dimension.m_Width, m_Dimension.m_Height));
   shape.setOrigin(sf::Vector2f(m_Handle.m_X, m_Handle.m_Y));
   shape.setRotation(m_Rotation);
   shape.setPosition(sf::Vector2f(thispos.m_X, thispos.m_Y));
   shape.setFillColor(color);

   Window().draw(shape);
}

// ************************************************************************************************
bool CCollisionRect::Collides(const CCollisionRect &other, const CPixelPos &otherpos, const CPixelPos &thispos) const
{
   return false;
}
