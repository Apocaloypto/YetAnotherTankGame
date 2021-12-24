#pragma once
#include "Typedefs.h"


// ************************************************************************************************
class CResourceManager
{
   friend CResourceManager &Resources();

private:
   CResourceManager() = default;

public:
   bool Load(const String &reslist);
};

// ************************************************************************************************
CResourceManager &Resources();
