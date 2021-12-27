#pragma once
#include "IController.h"


class CControllerPlayer : public IController
{
public:
   CControllerPlayer() = default;

   virtual Real GetTowerMod() const override;
   virtual void GetTrackMod(Real &lefttrack, Real &righttrack) const override;
   virtual bool Shoot() const override;
};
