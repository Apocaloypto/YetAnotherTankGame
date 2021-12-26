#pragma once
#include "Image.h"
#include "Typedefs.h"


class CDynamicDamageModel;

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
   const CPixelPos &TurnpointWanne = m_TurnpointWanne;
   const CPixelPos &TurnpointTurm = m_TurnpointTurm;
   const CPixelPos &TurmPosAufWanne = m_TurmPosAufWanne;

   CTankModelBlueprint();
   CTankModelBlueprint(CImage *pWanne, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, CImage *pTurm, const CPixelPos &turnpointTurm);
   virtual ~CTankModelBlueprint();

   static CTankModelBlueprint *LoadFrom(const String &filename);

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);

   CDynamicDamageModel *CreateDDMTower(Real stability) const;
   CDynamicDamageModel *CreateDDMWanne(Real stability) const;
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

// ************************************************************************************************
class CTankUsing
{
private:
   void DestroyDmgModels();
   void InitDmgModels();

   const CTankBlueprint *m_pBlueprint;

   CDynamicDamageModel *m_pDamageModelTurm;
   CDynamicDamageModel *m_pDamageModelWanne;

   CTilePos m_Pos;
   Degrees m_Rot;
   Degrees m_TowerRot;

public:
   const CTilePos &Position = m_Pos;

   CTankUsing(const CTankBlueprint *pBlueprint, const CTilePos &pos, Degrees rot, Degrees towerrot);
   virtual ~CTankUsing();

   void Draw(const CPixelPos &screen);

   void Update();
};
