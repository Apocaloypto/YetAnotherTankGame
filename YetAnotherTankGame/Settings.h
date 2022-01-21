#pragma once
#include <map>
#include <SFML/Window.hpp>
#include "Typedefs.h"


// ************************************************************************************************
class CSettings;

// ************************************************************************************************
typedef sf::Keyboard::Key KeyboardKey;

// ************************************************************************************************
enum class InputCommand
{
   LeftTrackForward,
   LeftTrackBackward,
   RightTrackForward,
   RightTrackBackward,

   TowerRotateLeft,
   TowerRotateRight,

   Fire,

   Pause,
};

// ************************************************************************************************
class CInputConfig
{
   friend CSettings;

private:
   std::map<InputCommand, KeyboardKey> m_CmdToKey;

   void SetDefaultConfig();
   void AddKey(InputCommand cmd, KeyboardKey key);

   CInputConfig();

public:
   const KeyboardKey KEY_NOT_FOUND = KeyboardKey::Unknown;

   const std::map<InputCommand, KeyboardKey> &CommandToKey = m_CmdToKey;
};

// ************************************************************************************************
class CSettings
{
   friend CSettings &Settings();

private:
   CInputConfig m_InputConfig;

   CSettings() = default;

public:
   const UInt32 ScreenWidth = 1366;
   const UInt32 ScreenHeight = 768;
   const UInt32 FrameLimit = 60;

   const CInputConfig &InputConfig = m_InputConfig;

   template<class T>
   inline T ToPerFrameValue(T base)
   {
      return base / (T)FrameLimit;
   }

   template<class T>
   inline T ToPerSecondValue(T base)
   {
      return base * (T)FrameLimit;
   }
};

// ************************************************************************************************
CSettings &Settings();
