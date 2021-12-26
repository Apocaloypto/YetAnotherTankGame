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

   for (const IDL::CIDLDataSet &dataset : datasets)
   {
      if (m_AllHolders.count(dataset[RESLIST_COL_TYPE]) <= 0)
      {
         m_Logger.Log(LogType::Error, "unknown type '" + dataset[RESLIST_COL_TYPE] + "'");
         continue;
      }

      IResourceHolder *pHolder = m_AllHolders.at(dataset[RESLIST_COL_TYPE]);
      if (!pHolder)
      {
         m_Logger.Log(LogType::Error, "invalid state in manager!");
         continue;
      }

      if (!pHolder->Load(dataset[RESLIST_COL_ID], dataset[RESLIST_COL_FILE]))
      {
         m_Logger.Log(LogType::Error, "failed to load resource '" + dataset[RESLIST_COL_ID] + "' (type: " + dataset[RESLIST_COL_TYPE] + ")");
         continue;
      }
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
   AddHolder("TMAP", &m_TileMapHolder);
   AddHolder("TANK", &m_TankBlueprintHolder);
}

// ************************************************************************************************
void CResourceManager::ShutDown()
{
   for (const auto &[type, pHolder] : m_AllHolders)
   {
      pHolder->DeleteAll();
   }
}

// ################################################################################################
CResourceManager &Resources()
{
   static CResourceManager inst;
   return inst;
}
