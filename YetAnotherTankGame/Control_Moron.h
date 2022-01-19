#pragma once
#include "IController.h"


class CControllerMoron : public IController
{
public:
   virtual Real GetTowerMod() const override;

   virtual Real GetLeftTrackMod() const override;

   virtual Real GetRightTrackMod() const override;

   virtual bool Shoot() const override;
};
