#include "ResourceManager.h"
#include <fstream>
#include "IDLParser.h"


// ################################################################################################
CResourceManager::CResourceManager()
   : m_Logger("ResMgr")
{
   AddHolders();
}

// ************************************************************************************************
bool CResourceManager::Load(const String &reslist)
{
   std::ifstream src(reslist);
   if (!src.is_open() || !src.good())
   {
      m_Logger.Log(LogType::Error, "unable to open file '" + reslist + "'");
      return false;
   }

   IDL::CIDLDataSets datasets;
   if (!IDL::ParseIDL(datasets, reslist, RESLIST_COLUMNS))
   {
      m_Logger.Log(LogType::Error, "failed to load reslist");
      return false;
   }

   return true;
}

// ************************************************************************************************
void CResourceManager::AddHolder(const String &type, IResourceHolder *pHolder)
{
   m_AllHolders.insert_or_assign(type, pHolder);
}

// ************************************************************************************************
void CResourceManager::AddHolders()
{
}

// ################################################################################################
CResourceManager &Resources()
{
   static CResourceManager inst;
   return inst;
}
