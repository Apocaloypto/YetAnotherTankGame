#pragma once
#include <cmath>
#include "Vector2D.h"
#include "Typedefs.h"


template <class T>
class CPoint2D
{
public:
   T m_X;
   T m_Y;

   CPoint2D(T x, T y)
      : m_X(x), m_Y(y)
   {
   }

   CPoint2D()
      : CPoint2D(0, 0)
   {
   }

   CPoint2D(const CPoint2D &right)
      : CPoint2D(right.m_X, right.m_Y)
   {
   }

   bool operator==(const CPoint2D &rhs) const
   {
      return m_X == rhs.m_X && m_Y == rhs.m_Y;
   }

   bool operator!=(const CPoint2D &rhs) const
   {
      return !(*this == rhs);
   }

   CPoint2D operator-(const CPoint2D &rhs) const
   {
      return CPoint2D(m_X - rhs.m_X, m_Y - rhs.m_Y);
   }

   CPoint2D operator-(const CVector2D<T> &rhs) const
   {
      return CPoint2D(m_X - rhs.m_X, m_Y - rhs.m_Y);
   }

   CPoint2D operator+(const CPoint2D<T> &rhs) const
   {
      return CPoint2D(m_X + rhs.m_X, m_Y + rhs.m_Y);
   }

   CPoint2D &operator+=(const CPoint2D<T> &rhs)
   {
      m_X += rhs.m_X;
      m_Y += rhs.m_Y;
      return *this;
   }

   CPoint2D operator*(T mult) const
   {
      return CPoint2D(m_X * mult, m_Y * mult);
   }

   T GetDistanceTo(const CPoint2D &other) const
   {
      return (T)sqrt(((other.m_X - m_X) * (other.m_X - m_X)) + ((other.m_Y - m_Y) * (other.m_Y - m_Y)));
   }
};
