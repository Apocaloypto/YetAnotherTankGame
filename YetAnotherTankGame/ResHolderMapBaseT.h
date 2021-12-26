#pragma once
#include <map>
#include "Typedefs.h"
#include "IResourceHolder.h"


template <class T>
class CResHolderMapBaseT : public IResourceHolder
{
private:
   std::map<String, T> m_Data;

protected:
   CResHolderMapBaseT() = default;

   bool Add(const String &id, T pObj)
   {
      if (m_Data.count(id) == 0)
      {
         m_Data.insert(std::pair<String, T>(id, pObj));
         return true;
      }
      else
      {
         return false;
      }
   }

   bool CheckId(const String &id) const
   {
      return m_Data.count(id) == 0;
   }

   virtual void DeleteElement(T pObj) = 0;

public:
   virtual ~CResHolderMapBaseT() {}

   virtual void DeleteAll() override
   {
      for (const auto &[id, pObj] : m_Data)
      {
         DeleteElement(pObj);
      }

      m_Data.clear();
   }

   T Get(const String &id) const
   {
      if (m_Data.count(id) != 0)
      {
         return m_Data.at(id);
      }
      else
      {
         return nullptr;
      }
   }
};
