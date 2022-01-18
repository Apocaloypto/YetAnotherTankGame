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
   m_TankUsings.DeleteAll();
   m_TankBlueprints.DeleteAll();
   m_Controller.DeleteAll();
   m_Images.DeleteAll();
}

// ################################################################################################
CMemoryManager &Memory()
{
   static CMemoryManager inst;
   return inst;
}
