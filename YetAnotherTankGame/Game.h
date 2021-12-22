#pragma once


// ************************************************************************************************
class CGame
{
   friend CGame &Game();

private:
   CGame() = default;

public:
   bool Initialize();
   void Frame();
   bool IsEnding() const;
   void ShutDown();
};

// ************************************************************************************************
CGame &Game();
