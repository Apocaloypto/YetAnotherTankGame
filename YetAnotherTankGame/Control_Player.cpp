#include "Control_Player.h"

#include "InputStates.h"


// ################################################################################################
void CControllerPlayer::Update(Real &lefttrack, Real &righttrack, Real &tower) const
{
   if (Input().IsPressed(InputCommand::TowerRotateRight))
   {
      tower = 1.0;
   }

   if (Input().IsPressed(InputCommand::TowerRotateLeft))
   {
      tower -= 1.0;
   }
}
