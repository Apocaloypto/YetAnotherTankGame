#pragma once
#include <functional>
#include "GUIControl.h"


class CGUICtrl_ProgressBar : public CGUIControl
{
private:
   const sf::Color m_ForeGround;
   const std::function<Real()> m_ValGetter;

public:
   CGUICtrl_ProgressBar(Int32 vx, Int32 vy, Int32 vw, Int32 vh, const std::function<Real()> &valgetter, const sf::Color &foreground);

   virtual void Draw() const override;
};
