#pragma once
#include <map>
#include <vector>
#include "Typedefs.h"
#include "Logging.h"
#include "IResourceHolder.h"


// ************************************************************************************************
class CResourceManager
{
   friend CResourceManager &Resources();

private:
   const int RESLIST_COL_ID          = 0;
   const int RESLIST_COL_TYPE        = 1;
   const int RESLIST_COL_FILE        = 2;

   const std::vector<int> RESLIST_COLUMNS = {
      RESLIST_COL_ID,        
      RESLIST_COL_TYPE,      
      RESLIST_COL_FILE,      
   };

   CResourceManager();

   CLogger m_Logger;
   std::map<String, IResourceHolder *> m_AllHolders;

   void AddHolder(const String &type, IResourceHolder *pHolder);
   void AddHolders();

public:
   bool Load(const String &reslist);
};

// ************************************************************************************************
CResourceManager &Resources();
