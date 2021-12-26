#pragma once
#include "Image.h"
#include "Typedefs.h"


// ************************************************************************************************
class CTankModelBlueprint
{
private:
   CImage *m_pWanne;
   CPixelPos m_TurnpointWanne;
   CPixelPos m_TurmPosAufWanne;
   CImage *m_pTurm;
   CPixelPos m_TurnpointTurm;

public:
   CTankModelBlueprint();
   CTankModelBlueprint(CImage *pWanne, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, CImage *pTurm, const CPixelPos &turnpointTurm);
   virtual ~CTankModelBlueprint();

   static CTankModelBlueprint *LoadFrom(const String &filename);

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};

// ************************************************************************************************
class CTankSpecsBlueprint
{
public:
   const String m_Name;
   const KmPerH m_MaxSpeed;
   const Seconds m_SecsTilMaxSpeed;
   const Real m_Stability;

   CTankSpecsBlueprint(const CTankSpecsBlueprint &right);
   CTankSpecsBlueprint(const String &name, KmPerH maxspeed, Seconds secstilmaxspeed, Real stability);
};

// ************************************************************************************************
class CTankBlueprint
{
public:
   CTankModelBlueprint *m_pModel;
   CTankSpecsBlueprint m_Specs;

   CTankBlueprint(CTankModelBlueprint *pModel, const CTankSpecsBlueprint &specs);
   ~CTankBlueprint();
};
