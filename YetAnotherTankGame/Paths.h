#pragma once
#include <map>
#include "Typedefs.h"


// ************************************************************************************************
enum class PathsEntry
{
   Undefined,

   ResList,
};

// ************************************************************************************************
class CPaths
{
private:
   std::map<PathsEntry, String> m_Data;

   static PathsEntry GetTypeFromString(const String &keyname);

public:
   bool Load(const String &file);

   String Get(PathsEntry entry) const;
};
