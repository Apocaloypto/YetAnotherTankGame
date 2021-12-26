#pragma once
#include <vector>
#include <optional>
#include "ResHolderMapBaseT.h"


class CTankBlueprint;
class CTankSpecsBlueprint;

namespace IDL
{
   class CIDLDataSet;
}

class CResHolderTankBlueprints : public CResHolderMapBaseT<CTankBlueprint *>
{
private:
   const int TANKS_COL_ID = 0;
   const int TANKS_COL_NAME = 1;
   const int TANKS_COL_MAXSPEED = 2;
   const int TANKS_COL_SECS = 3;
   const int TANKS_COL_STAB = 4;
   const int TANKS_COL_FILE = 5;

   const std::vector<int> TANK_COLUMNS = {
      TANKS_COL_ID,
      TANKS_COL_NAME,
      TANKS_COL_MAXSPEED,
      TANKS_COL_SECS,
      TANKS_COL_STAB,
      TANKS_COL_FILE
   };

   std::optional<CTankSpecsBlueprint> ParseSpecs(const IDL::CIDLDataSet &dataset) const;

protected:
   virtual void DeleteElement(CTankBlueprint *pObj) override;

public:
   virtual bool Load(const String &id, const String &filename) override;
};
