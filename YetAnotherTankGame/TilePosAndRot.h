#pragma once
#include "Typedefs.h"
#include "TemplateUsings.h"


class CTilePosAndRot
{
public:
   const CTilePos m_NewPos;
   const Degrees m_NewRot;

   CTilePosAndRot(const CTilePos &pos, Degrees rot)
      : m_NewPos(pos), m_NewRot(rot)
   {
   }

   CTilePosAndRot(const CTilePosAndRot &right)
      : CTilePosAndRot(right.m_NewPos, right.m_NewRot)
   {
   }
};
