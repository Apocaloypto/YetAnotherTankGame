#pragma once
#include "Typedefs.h"


class IController
{
public:
   virtual ~IController() {}

   virtual Real GetTowerMod() const = 0;
   virtual Real GetLeftTrackMod() const = 0;
   virtual Real GetRightTrackMod() const = 0;
   virtual bool Shoot() const = 0;
};
