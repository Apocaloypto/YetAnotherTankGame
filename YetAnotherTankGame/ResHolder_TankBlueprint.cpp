#include "ResHolder_TankBlueprint.h"

#include "Tanks.h"
#include "Memory.h"
#include "IDLParser.h"
#include "Logging.h"
#include "Functions.h"


// ################################################################################################
void CResHolderTankBlueprints::DeleteElement(CTankBlueprint *pObj)
{
   Memory().m_TankBlueprints.Delete(pObj);
}

// ************************************************************************************************
std::optional<CTankSpecsBlueprint> CResHolderTankBlueprints::ParseSpecs(const IDL::CIDLDataSet &dataset) const
{
#define PARSE_REAL_DATA(name, idl) Real name; if (!StringFun::TryParseReal(dataset[idl], name)) { return std::nullopt; };

   String tankname = dataset[TANKS_COL_NAME];

   PARSE_REAL_DATA(maxspeed, TANKS_COL_MAXSPEED);
   PARSE_REAL_DATA(seconds, TANKS_COL_SECS);
   PARSE_REAL_DATA(stability, TANKS_COL_STAB);

   return CTankSpecsBlueprint(tankname, maxspeed, seconds, stability);

#undef PARSE_REAL_DATA
}

// ************************************************************************************************
bool CResHolderTankBlueprints::Load(const String &id, const String &filename)
{
   CLogger logger("TankLoader");

   IDL::CIDLDataSets datasets;
   if (!IDL::ParseIDL(datasets, filename, TANK_COLUMNS))
   {
      return false;
   }

   for (const IDL::CIDLDataSet &dataset : datasets)
   {
      if (!CheckId(dataset[TANKS_COL_ID]))
      {
         logger.Log(LogType::Warning, "unable to load tank with id '" + dataset[TANKS_COL_ID] + "': id already in use");
         continue;
      }

      // Modell:
      CTankModelBlueprint *pModel = CTankModelBlueprint::LoadFrom(dataset[TANKS_COL_FILE]);
      if (!pModel)
      {
         logger.Log(LogType::Warning, "unable to load model '" + dataset[TANKS_COL_FILE] + "'");
         continue;
      }

      // Specs:
      std::optional<CTankSpecsBlueprint> specs = ParseSpecs(dataset);
      if (!specs.has_value())
      {
         logger.Log(LogType::Warning, "unable to parse specs for '" + dataset[TANKS_COL_ID] + "'");
         continue;
      }

      CTankBlueprint *pBluePrint = Memory().m_TankBlueprints.Add(new CTankBlueprint(pModel, specs.value()));
      if (!pBluePrint)
      {
         logger.Log(LogType::Warning, "unable to register tank-object");
         continue;
      }

      if (!Add(dataset[TANKS_COL_ID], pBluePrint))
      {
         logger.Log(LogType::Warning, "unable to add tank-object");
         continue;
      }
   }

   return true;
}
