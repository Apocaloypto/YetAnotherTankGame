#include "ResHolder_TankBlueprint.h"

#include "Tanks.h"
#include "Memory.h"
#include "IDLParser.h"


// ################################################################################################
void CResHolderTankBlueprints::DeleteElement(CTankBlueprint *pObj)
{
   Memory().m_TankBlueprints.Delete(pObj);
}

// ************************************************************************************************
bool CResHolderTankBlueprints::Load(const String &id, const String &filename)
{
   return false;
}
