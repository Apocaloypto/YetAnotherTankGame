#pragma once
#include "Image.h"
#include "Typedefs.h"


// ************************************************************************************************
class CTankBlueprint
{
private:
   CImage *m_pWanne;
   CPixelPos m_TurnpointWanne;
   CPixelPos m_TurmPosAufWanne;
   CImage *m_pTurm;
   CPixelPos m_TurnpointTurm;

public:
   CTankBlueprint();
   CTankBlueprint(CImage *pWanne, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, CImage *pTurm, const CPixelPos &turnpointTurm);

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};
