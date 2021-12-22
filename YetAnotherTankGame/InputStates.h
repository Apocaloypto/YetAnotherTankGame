#pragma once
#include <map>
#include "Settings.h"


// ************************************************************************************************
class CInputState
{
   friend CInputState &Input();

private:
   enum class KeyState
   {
      Nothing,
      Hit,
      Press,
   };

   std::map<InputCommand, KeyState> m_States;

   CInputState() = default;

   bool HasState(InputCommand cmd, KeyState state) const;

public:
   void Update();

   bool IsPressed(InputCommand cmd) const;
   bool IsHit(InputCommand cmd) const;
};

// ************************************************************************************************
CInputState &Input();
