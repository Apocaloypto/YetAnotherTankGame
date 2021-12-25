#include "Memory.h"


// ################################################################################################
CMemoryManager::~CMemoryManager()
{
   DeleteAll();
}

// ************************************************************************************************
void CMemoryManager::DeleteAll()
{
   m_Maps.DeleteAll();
   m_MapObjects.DeleteAll();
}

// ################################################################################################
CMemoryManager &Memory()
{
   static CMemoryManager inst;
   return inst;
}
