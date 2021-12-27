#pragma once
#include "Typedefs.h"


class IController
{
public:
   virtual ~IController() {}

   virtual void Update(Real &lefttrack, Real &righttrack, Real &tower) const = 0;
};
