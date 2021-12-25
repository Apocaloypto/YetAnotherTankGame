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
};
