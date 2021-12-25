#pragma once
#include <functional>
#include "TypeDefs.h"


template<class T>
class CArray2D 
{
private:
   T DUMMY;

   T *m_pData;
   Int32 m_Width;
   Int32 m_Height;

   Int32 GetIndex(Int32 x, Int32 y) const
   {
      return x + (y * m_Width);
   }

public:
   CArray2D()
      : m_pData(nullptr), m_Width(0), m_Height(0), DUMMY(T(0))
   {
   }

   CArray2D(Int32 width, Int32 height)
      : CArray2D()
   {
      Resize(width, height);
   }

   ~CArray2D()
   {
      Clear();
   }

   void TransferTo(CArray2D<T> &dest)
   {
      dest.Resize(GetWidth(), GetHeight());

      ForEach([&](Int32 x, Int32 y, T &elem) -> void
         {
            dest.GetAt(x, y) = elem;
         });

      Clear();
   }

   Int32 GetWidth() const
   {
      return m_Width;
   }

   Int32 GetHeight() const
   {
      return m_Height;
   }

   void Clear()
   {
      if (m_pData)
      {
         delete[] m_pData;
         m_pData = nullptr;
      }
   }

   void Resize(Int32 width, Int32 height)
   {
      if (m_pData)
      {
         delete[] m_pData;
      }

      m_pData = new T[width * height];
      m_Width = width;
      m_Height = height;
   }

   T &GetAt(Int32 x, Int32 y)
   {
      if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
      {
         return m_pData[GetIndex(x, y)];
      }
      else
      {
         return DUMMY;
      }
   }

   const T &GetAt(Int32 x, Int32 y) const
   {
      if (x >= 0 && x < m_Width && y >= 0 && y < m_Height)
      {
         return m_pData[GetIndex(x, y)];
      }
      else
      {
         return DUMMY;
      }
   }

   void ForEach(const std::function<void(Int32, Int32, const T &)> &fnEach) const
   {
      if (!m_pData)
         return;

      for (Int32 y = 0; y < m_Height; y++)
      {
         for (Int32 x = 0; x < m_Width; x++)
         {
            fnEach(x, y, m_pData[GetIndex(x, y)]);
         }
      }
   }

   void ForEach(const std::function<void(Int32, Int32, T &)> &fnEach)
   {
      if (!m_pData)
         return;

      for (Int32 y = 0; y < m_Height; y++)
      {
         for (Int32 x = 0; x < m_Width; x++)
         {
            fnEach(x, y, m_pData[GetIndex(x, y)]);
         }
      }
   }
};
