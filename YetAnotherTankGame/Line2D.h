#pragma once
#include <optional>
#include <utility>
#include "Point2D.h"


template<class T>
class CLine2D
{
public:
   CPoint2D<T> m_Start;
   CPoint2D<T> m_End;

   CLine2D(const CPoint2D<T> &start, const CPoint2D<T> &end)
      : m_Start(start), m_End(end)
   {
   }

   T GetLength() const
   {
      return m_Start.GetDistanceTo(m_End);
   }

   std::optional<CPoint2D<T>> Crosses(const CLine2D &other) const
   {
      // https://www.geeksforgeeks.org/program-for-point-of-intersection-of-two-lines/
      // Line AB represented as a1x + b1y = c1
      T a1 = m_End.m_Y - m_Start.m_Y;
      T b1 = m_Start.m_X - m_End.m_X;
      T c1 = a1 * (m_Start.m_X) + b1 * (m_Start.m_Y);

      // Line CD represented as a2x + b2y = c2
      T a2 = other.m_End.m_Y - other.m_Start.m_Y;
      T b2 = other.m_Start.m_X - other.m_End.m_X;
      T c2 = a2 * (other.m_Start.m_X) + b2 * (other.m_Start.m_Y);

      T determinant = a1 * b2 - a2 * b1;

      if (determinant == 0)
      {
         // The lines are parallel. This is simplified
         // by returning a pair of FLT_MAX
         return std::nullopt;
      }
      else
      {
         CPoint2D<T> crossingPoint((b2 * c1 - b1 * c2) / determinant, (a1 * c2 - a2 * c1) / determinant);

         if (crossingPoint.IsOnSegment(m_Start, m_End))
         {
            return crossingPoint;
         }
         else
         {
            return std::nullopt;
         }
      }
   }
};
