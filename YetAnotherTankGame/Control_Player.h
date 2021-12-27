#pragma once
#include "IController.h"


class CControllerPlayer : public IController
{
public:
   CControllerPlayer() = default;

   virtual Real GetTowerMod() const override;
   virtual Real GetLeftTrackMod() const override;
   virtual Real GetRightTrackMod() const override;
   virtual bool Shoot() const override;
};
