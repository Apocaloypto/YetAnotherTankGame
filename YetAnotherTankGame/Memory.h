#pragma once
#include <set>
#include "Typedefs.h"
#include "Tilemap.h"
#include "ITileMapObject.h"
#include "Tanks.h"
#include "Image.h"
#include "IController.h"


// ************************************************************************************************
template<class T>
class CMemoryPool
{
private:
   ptr_set<T> m_Data;

public:
   virtual ~CMemoryPool()
   {
      DeleteAll();
   }

   T *Add(T *obj)
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

   template<class T2>
   T2 *AddTyped(T2 *pObj)
   {
      return (T2 *)Add(pObj);
   }

   bool IsValid(const T *obj) const
   {
      return obj != nullptr && m_Data.count(obj) != 0;
   }

   void Delete(T *obj)
   {
      if (IsValid(obj))
      {
         m_Data.erase(obj);
         delete obj;
      }
   }

   void DeleteAll()
   {
      for (T *obj : m_Data)
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

   CMemoryPool<CTileMap> m_Maps;
   CMemoryPool<ITileMapObject> m_MapObjects;
   CMemoryPool<CTankBlueprint> m_TankBlueprints;
   CMemoryPool<CTankUsing> m_TankUsings;
   CMemoryPool<IController> m_Controller;
   CMemoryPool<CImage> m_Images;
};

// ************************************************************************************************
CMemoryManager &Memory();
