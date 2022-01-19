#include "GUICtrl_SpeedoMeter.h"

#include "Context.h"
#include "Tanks.h"
#include "Memory.h"


// ################################################################################################
const sf::Color CGUICtrl_SpeedOMeter::BAR_COLOR = sf::Color(0x00FF00FF);

// ************************************************************************************************
CGUICtrl_SpeedOMeter::CGUICtrl_SpeedOMeter(Int32 vx, Int32 vy, Int32 vw, Int32 vh, Track track)
   : CGUICtrl_ProgressBar(vx, vy, vw, vh, std::bind(&CGUICtrl_SpeedOMeter::GetValue, this), BAR_COLOR),
   m_Track(track)
{
}

// ************************************************************************************************
Real CGUICtrl_SpeedOMeter::GetValue()
{
   if (!Memory().m_TankUsings.IsValid(Context().GetPlayersTank()))
   {
      return 0;
   }

   if (m_Track == Track::Left)
   {
      return Context().GetPlayersTank()->GetLeftTrackSpeedPercentage();
   }
   else if (m_Track == Track::Right)
   {
      return Context().GetPlayersTank()->GetRightTrackSpeedPercentage();
   }
   else
   {
      return 0;
   }
}

// ************************************************************************************************
void CGUICtrl_SpeedOMeter::Draw() const 
{
   __super::Draw();
}
