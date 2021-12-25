#pragma once
#include <set>
#include "ITileMapObject.h"


// ************************************************************************************************
template<class T>
class CMemoryPool
{
private:
   std::set<T> m_Data;

public:
   virtual ~CMemoryPool()
   {
      Delete();
   }

   T Add(T obj)
   {
      if (obj != nullptr && m_Data.count(obj) == 0)
      {
         m_Data.insert(obj);
         return obj;
      }
      else
      {
         if (obj != nullptr)
            delete obj;

         return nullptr;
      }
   }

   bool IsValid(T obj) const
   {
      return obj != nullptr && m_Data.count(obj) != 0;
   }

   void Delete()
   {
      for (T obj : m_Data)
      {
         delete obj;
      }

      m_Data.clear();
   }
};

// ************************************************************************************************
class CMemoryManager
{
   friend CMemoryManager &Memory();
   
private:
   CMemoryManager() = default;
   virtual ~CMemoryManager();

public:
   void DeleteAll();

   CMemoryPool<ITileMapObject *> m_MapObjects;
};

// ************************************************************************************************
CMemoryManager &Memory();
