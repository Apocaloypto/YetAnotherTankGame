#pragma once
#include "IController.h"


class CControllerPlayer : public IController
{
public:
   CControllerPlayer() = default;

   virtual void Update(Real &lefttrack, Real &righttrack, Real &tower) const override;
};
