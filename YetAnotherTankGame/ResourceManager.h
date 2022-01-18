#pragma once
#include <map>
#include <vector>
#include "Typedefs.h"
#include "Logging.h"
#include "ResHolder_TileMaps.h"
#include "ResHolder_TankBlueprint.h"
#include "ResHolder_Image.h"


// ************************************************************************************************
class IResourceHolder;

// ************************************************************************************************
class CResourceManager
{
   friend CResourceManager &Resources();

private:
   const int RESLIST_COL_ID          = 0;
   const int RESLIST_COL_TYPE        = 1;
   const int RESLIST_COL_FILE        = 2;
   const int RESLIST_COL_FILEEXT     = 3;

   const std::vector<int> RESLIST_COLUMNS = {
      RESLIST_COL_ID,        
      RESLIST_COL_TYPE,      
      RESLIST_COL_FILE,
      RESLIST_COL_FILEEXT,
   };

   CResourceManager();

   CLogger m_Logger;
   std::map<String, IResourceHolder *> m_AllHolders;

   void AddHolder(const String &type, IResourceHolder *pHolder);
   void AddHolders();

   CResHolderTileMaps m_TileMapHolder;
   CResHolderTankBlueprints m_TankBlueprintHolder;
   CResHolderImages m_ImageHolder;

public:
   bool Load(const String &reslist);

   void ShutDown();

   const CResHolderTileMaps &TileMaps = m_TileMapHolder;
   const CResHolderTankBlueprints &TankBlueprints = m_TankBlueprintHolder;
   const CResHolderImages &Images = m_ImageHolder;
};

// ************************************************************************************************
CResourceManager &Resources();
