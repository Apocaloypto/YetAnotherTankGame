#include "ResHolder_TileMaps.h"

#include "Tilemap.h"
#include "Memory.h"
#include "Logging.h"
#include <fstream>


// ################################################################################################
bool CResHolderTileMaps::Load(const String &id, const String &filename)
{
   CLogger logger("mapholder");

   std::ifstream src;
   src.open(filename, std::ios::binary);

   if (!src.is_open() || !src.good())
   {
      logger.Log(LogType::Error, "unable to read '" + filename + "'");
      return false;
   }

   CTileMap *pMap = Memory().m_Maps.Add(new CTileMap());
   if (!pMap)
   {
      logger.Log(LogType::Error, "unable to register tilemap-object!");
      return false;
   }
   
   bool loadSucc = pMap->StreamLoad(src);
   src.close();

   if (!loadSucc)
   {
      delete pMap;
      logger.Log(LogType::Error, "unable to read '" + filename + "'");
      return false;
   }

   return Add(id, pMap);
}

// ************************************************************************************************
void CResHolderTileMaps::DeleteElement(CTileMap *pObj)
{
   Memory().m_Maps.Delete(pObj);
}
