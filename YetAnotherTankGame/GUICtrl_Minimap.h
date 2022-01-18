#pragma once
#include "Typedefs.h"
#include "GUIControl.h"


class CGUICtrl_Minimap : public CGUIControl
{
public:
   CGUICtrl_Minimap(Int32 vx, Int32 vy, Int32 vw, Int32 vh);

   virtual void Draw() const override;
};
