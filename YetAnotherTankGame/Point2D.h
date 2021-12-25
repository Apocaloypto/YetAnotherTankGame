#pragma once


template <class T>
class CPoint2D
{
public:
   const T m_X;
   const T m_Y;

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
};
