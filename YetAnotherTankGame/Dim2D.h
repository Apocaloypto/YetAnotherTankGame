#pragma once
#include "Point2D.h"
#include "Vector2D.h"


template <class T>
class CDim2D
{
public:
   const T m_Width;
   const T m_Height;

   CDim2D()
      : CDim2D(0, 0)
   {
   }

   CDim2D(T w, T h)
      : m_Width(w), m_Height(h)
   {
   }

   CDim2D(const CDim2D &right)
      : m_Width(right.m_Width), m_Height(right.m_Height)
   {
   }

   CVector2D<T> AsVector() const
   {
      return CVector2D(m_Width, m_Height);
   }

   CDim2D operator/(T div) const
   {
      return CDim2D(m_Width / div, m_Height / div);
   }

   bool ContainsPoint(const CPoint2D<T> &offset, const CPoint2D<T> &chk) const
   {
      return chk.m_X >= offset.m_X && 
         chk.m_X < offset.m_X + m_Width &&
         chk.m_Y >= offset.m_Y &&
         chk.m_Y < offset.m_Y + m_Height;
   }
};
