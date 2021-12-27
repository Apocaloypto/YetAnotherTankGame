#include "Control_Player.h"

#include "InputStates.h"


// ################################################################################################
Real CControllerPlayer::GetTowerMod() const
{
   Real tower = 0.0;

   if (Input().IsPressed(InputCommand::TowerRotateRight))
   {
      tower = 1.0;
   }

   if (Input().IsPressed(InputCommand::TowerRotateLeft))
   {
      tower -= 1.0;
   }

   return tower;
}

// ************************************************************************************************
Real CControllerPlayer::GetLeftTrackMod() const
{
   Real lefttrack = 0.0;

   if (Input().IsPressed(InputCommand::LeftTrackForward))
   {
      lefttrack = 1.0;
   }

   if (Input().IsPressed(InputCommand::LeftTrackBackward))
   {
      lefttrack -= 1.0;
   }

   return lefttrack;
}

// ************************************************************************************************
Real CControllerPlayer::GetRightTrackMod() const
{
   Real righttrack = 0.0;

   if (Input().IsPressed(InputCommand::RightTrackForward))
   {
      righttrack = 1.0;
   }

   if (Input().IsPressed(InputCommand::RightTrackBackward))
   {
      righttrack -= 1.0;
   }

   return righttrack;
}

// ************************************************************************************************
bool CControllerPlayer::Shoot() const
{
   return false;
}
