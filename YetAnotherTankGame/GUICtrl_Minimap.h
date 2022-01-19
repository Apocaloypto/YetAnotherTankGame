#pragma once
#include "Typedefs.h"
#include "GUIControl.h"


class CGUICtrl_Minimap : public CGUIControl
{
private:
   static const String NORTH_INDICATOR_RESID;
   static const String PLAYER_INDICATOR_RESID;
   static const String ENEMY_INDICATOR_RESID;

   void DrawNorthIndicator(Degrees playerRot) const;
   void DrawPlayerIndicator() const;

public:
   CGUICtrl_Minimap(Int32 vx, Int32 vy, Int32 vw, Int32 vh);

   virtual void Draw() const override;
};
