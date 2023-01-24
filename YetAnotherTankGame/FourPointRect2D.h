#pragma once
#include "Point2D.h"


template<class T>
class CFourPointRect2D
{
public:
   CPoint2D<T> m_UpperLeft;
   CPoint2D<T> m_UpperRight;
   CPoint2D<T> m_BottomLeft;
   CPoint2D<T> m_BottomRight;

   CFourPointRect2D() = default;

   CFourPointRect2D(const CPoint2D<T> &upperleft, const CPoint2D<T> &upperright, const CPoint2D<T> &bottomleft, const CPoint2D<T> &bottomright)
      : m_UpperLeft(upperleft),
      m_UpperRight(upperright),
      m_BottomLeft(bottomleft),
      m_BottomRight(bottomright)
   {
   }
};
