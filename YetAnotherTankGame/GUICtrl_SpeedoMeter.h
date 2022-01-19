#pragma once
#include "GUICtrl_ProgressBar.h"


class CGUICtrl_SpeedOMeter : public CGUICtrl_ProgressBar
{
public:
   enum class Track
   {
      Left,
      Right,
   };

private:
   static const sf::Color BAR_COLOR;

   const Track m_Track;

   Real GetValue();

public:
   CGUICtrl_SpeedOMeter(Int32 vx, Int32 vy, Int32 vw, Int32 vh, Track track);
};
