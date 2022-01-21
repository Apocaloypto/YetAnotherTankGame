#pragma once
#include "Image.h"
#include "Typedefs.h"
#include "TilePosAndRot.h"
#include "CollisionRect.h"
#include "PhysicalData.h"


class CDynamicDamageModel;
class IController;

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

   CPixelDim GetDimensions() const;

   CDynamicDamageModel *CreateDDMTower(Real stability) const;
   CDynamicDamageModel *CreateDDMWanne(Real stability) const;
};

// ************************************************************************************************
class CTankSpecsBlueprint
{
public:
   const String m_Name;
   const KmPerH m_MaxSpeed;
   const MPerS m_Acceleration;
   const DegPerS m_TowerRotationSpeed;
   const Real m_Stability;
   const KG m_Weight;

   CTankSpecsBlueprint(const String &name, KmPerH maxspeed, MPerS acceleration, DegPerS towerRotSpeed, Real stability, KG weight);
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

   const IController *m_pController;

   CDynamicDamageModel *m_pDamageModelTurm;
   CDynamicDamageModel *m_pDamageModelWanne;

   CTilePos m_Pos;
   Degrees m_Rot;

   Degrees m_TowerRot;

   MPerS m_CurrentSpeedLT;
   MPerS m_CurrentSpeedRT;

   MPerS GetMaxTrackSpeed() const;
   MPerS GetMaxTrackSpeedTurn() const;

   void DoTowerUpdate();
   void DoMovingUpdate(MPerS base, MPerS &newval, Real mod) const;
   void DoMovingUpdate(MPerS &side, Real mod);

   void DoTrackSlowDown(MPerS base, MPerS &newval, Real deduction) const;

   void ApplyUpdates(MPerS &currentSpeedLT, MPerS &currentSpeedRT, CTilePos &newpos, Degrees &newrot) const;

   void DoTurnOverSide(MPerS &currentSpeedLT, MPerS &currentSpeedRT, CTilePos &newpos, Degrees &newrot) const;
   void DoInPlaceTurn(MPerS &currentSpeedLT, MPerS &currentSpeedRT, CTilePos &newpos, Degrees &newrot) const;
   void DoNormalDrive(CTilePos &newpos, Degrees &newrot) const;

   Degrees CalcNewRotation(Real curveRadius, MPerS speed, Int32 directionMod) const;

public:
   const CTilePos &Position = m_Pos;
   const Degrees &Rotation = m_Rot;

   Real GetLeftTrackSpeedPercentage() const;
   Real GetRightTrackSpeedPercentage() const;

   CTankUsing(const CTankBlueprint *pBlueprint, const CTilePos &pos, Degrees rot, Degrees towerrot, const IController *pController);
   virtual ~CTankUsing();

   void Draw(const CPixelPos &screen);

   CCollisionRect GetCollisionRect(const CPixelPos &pos) const;
   CPhysicalData GetPhysicalData() const;

   CTilePosAndRot PreUpdate() const;
   void DoUpdate(const CTilePosAndRot &newvals, bool collision);

   void OnCollisionWithMapObject(const CCollisionRect &thisrect, const CCollisionRect &otherrect, const CPhysicalData &otherphysicaldata);
};
