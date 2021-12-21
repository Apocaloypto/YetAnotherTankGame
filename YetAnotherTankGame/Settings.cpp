#include "Settings.h"


// ################################################################################################
CInputConfig::CInputConfig()
{
   SetDefaultConfig();
}

// ************************************************************************************************
void CInputConfig::AddKey(InputCommand cmd, KeyboardKey key)
{
   m_CmdToKey.insert(std::pair<InputCommand, KeyboardKey>(cmd, key));
}

// ************************************************************************************************
void CInputConfig::SetDefaultConfig()
{
   m_CmdToKey.clear();

   AddKey(InputCommand::LeftTrackForward, KeyboardKey::Q);
   AddKey(InputCommand::LeftTrackBackward, KeyboardKey::A);
   AddKey(InputCommand::RightTrackForward, KeyboardKey::E);
   AddKey(InputCommand::RightTrackBackward, KeyboardKey::D);
   AddKey(InputCommand::TowerRotateLeft, KeyboardKey::Left);
   AddKey(InputCommand::TowerRotateRight, KeyboardKey::Right);
   AddKey(InputCommand::Fire, KeyboardKey::Space);
   AddKey(InputCommand::Pause, KeyboardKey::Escape);
}

// ################################################################################################
CSettings &Settings()
{
   static CSettings inst;
   return inst;
}
