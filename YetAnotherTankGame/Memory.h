#pragma once
#include <set>
#include "Tilemap.h"
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
      DeleteAll();
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

   void Delete(T obj)
   {
      if (IsValid(obj))
      {
         m_Data.erase(obj);
         delete obj;
      }
   }

   void DeleteAll()
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

   CMemoryPool<CTileMap *> m_Maps;
   CMemoryPool<ITileMapObject *> m_MapObjects;
};

// ************************************************************************************************
CMemoryManager &Memory();
