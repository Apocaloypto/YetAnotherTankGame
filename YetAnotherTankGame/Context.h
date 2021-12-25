#pragma once
#include "Typedefs.h"


// ************************************************************************************************
class CTileMap;

// ************************************************************************************************
class CContext
{
   friend CContext &Context();

private:
   CContext() = default;

public:
   CTileMap *m_pCurrentMap;

   void Reset();
};

// ************************************************************************************************
CContext &Context();
