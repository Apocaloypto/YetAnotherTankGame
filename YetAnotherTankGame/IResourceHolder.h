#pragma once
#include "Typedefs.h"


class IResourceHolder
{
public:
   virtual ~IResourceHolder() {}

   virtual bool Load(const String &id, const String &filename) = 0;
   virtual void DeleteAll() = 0;
};
