#pragma once
#include "Typedefs.h"


class IController
{
public:
   virtual ~IController() {}

   virtual Real GetTowerMod() const = 0;
   virtual void GetTrackMod(Real &lefttrack, Real &righttrack) const = 0;
   virtual bool Shoot() const = 0;
};
