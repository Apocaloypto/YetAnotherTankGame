#pragma once
#include "Vector2D.h"


template <class T>
class CPoint2D
{
private:
   T m_XInternal;
   T m_YInternal;

public:
   const T &m_X = m_XInternal;
   const T &m_Y = m_YInternal;

   CPoint2D(T x, T y)
      : m_XInternal(x), m_YInternal(y)
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
      m_XInternal += rhs.m_X;
      m_YInternal += rhs.m_Y;
      return *this;
   }

   CPoint2D operator*(T mult) const
   {
      return CPoint2D(m_X * mult, m_Y * mult);
   }
};
