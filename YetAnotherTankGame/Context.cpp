#include "Context.h"


// ################################################################################################
void CContext::Reset()
{
   m_pCurrentMap = nullptr;
}

// ################################################################################################
CContext &Context()
{
   static CContext inst;
   return inst;
}
