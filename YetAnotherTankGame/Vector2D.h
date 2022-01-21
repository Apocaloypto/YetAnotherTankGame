#pragma once


template <class T>
class CVector2D
{
public:
   const T m_X;
   const T m_Y;

   CVector2D()
      : CVector2D(0, 0)
   {
   }

   CVector2D(T x, T y)
      : m_X(x), m_Y(y)
   {
   }

   CVector2D(const CVector2D &right)
      : CVector2D(right.m_X, right.m_Y)
   {
   }

   T GetMagnitude() const
   {
      return (T)sqrt(m_X * m_X + m_Y * m_Y);
   }
};
