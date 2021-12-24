#include "ResourceManager.h"


// ################################################################################################
bool CResourceManager::Load(const String &reslist)
{
   return false;
}

// ################################################################################################
CResourceManager &Resources()
{
   static CResourceManager inst;
   return inst;
}
