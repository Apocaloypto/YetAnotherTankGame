#include "InputStates.h"

#include "Window.h"


// ################################################################################################
void CInputState::Update()
{
   for (const auto &[cmd, key] : Settings().InputConfig.CommandToKey)
   {
      bool bCurrentlyPressed = sf::Keyboard::isKeyPressed(key);

      if (bCurrentlyPressed)
      {
         m_States[cmd] = KeyState::Press;
      }
      else
      {
         if (IsPressed(cmd))
         {
            m_States[cmd] = KeyState::Hit;
         }
         else
         {
            m_States[cmd] = KeyState::Nothing;
         }
      }

      if (!Window().hasFocus())
      {
         m_States[cmd] = KeyState::Nothing;
      }
   }
}

// ************************************************************************************************
bool CInputState::HasState(InputCommand cmd, KeyState state) const
{
   if (m_States.count(cmd) > 0)
   {
      return m_States.at(cmd) == state;
   }
   else
   {
      return false;
   }
}

// ************************************************************************************************
bool CInputState::IsPressed(InputCommand cmd) const
{
   return HasState(cmd, KeyState::Press);
}

// ************************************************************************************************
bool CInputState::IsHit(InputCommand cmd) const
{
   return HasState(cmd, KeyState::Hit);
}

// ################################################################################################
CInputState &Input()
{
   static CInputState inst;
   return inst;
}
