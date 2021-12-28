#include "Tanks.h"
#include "Functions.h"
#include "Memory.h"
#include "DDM2D.h"
#include "IController.h"
#include "Settings.h"
#include "Context.h"
#include <limits>
#include <iostream>


// ################################################################################################
const String STREAM_HEADER = "tnk";

// ################################################################################################
CTankModelBlueprint::CTankModelBlueprint()
   : CTankModelBlueprint(nullptr, CPixelPos(), CPixelPos(), nullptr, CPixelPos())
{
}

// ************************************************************************************************
CTankModelBlueprint::CTankModelBlueprint(CImage *pWanne, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, CImage *pTurm, const CPixelPos &turnpointTurm)
   : m_pWanne(pWanne),
   m_TurnpointWanne(turnpointWanne),
   m_TurmPosAufWanne(turmPosAufWanne),
   m_pTurm(pTurm),
   m_TurnpointTurm(turnpointTurm)
{
}

// ************************************************************************************************
CTankModelBlueprint::~CTankModelBlueprint()
{
   if (m_pWanne)
      delete m_pWanne;

   if (m_pTurm)
      delete m_pTurm;
}

// ************************************************************************************************
CTankModelBlueprint *CTankModelBlueprint::LoadFrom(const String &filename)
{
   std::ifstream src(filename, std::ios::binary);
   if (!src.is_open() || !src.good())
   {
      return nullptr;
   }

   CTankModelBlueprint *pModel = new CTankModelBlueprint();
   bool bSuccess = pModel->StreamLoad(src);

   src.close();

   if (bSuccess)
   {
      return pModel;
   }
   else
   {
      delete pModel;
      return nullptr;
   }
}

// ************************************************************************************************
bool CTankModelBlueprint::StreamSave(std::ofstream &dest) const
{
   if (!dest.is_open() || !dest.good())
      return false;

   if (!m_pWanne || !m_pTurm)
      return false;

   StreamFun::StreamWriteHeader(dest, STREAM_HEADER);

   m_pWanne->StreamSave(dest);
   StreamFun::StreamWritePoint2D(dest, m_TurnpointWanne);
   StreamFun::StreamWritePoint2D(dest, m_TurmPosAufWanne);
   m_pTurm->StreamSave(dest);
   StreamFun::StreamWritePoint2D(dest, m_TurnpointTurm);

   return dest.good();
}

// ************************************************************************************************
bool CTankModelBlueprint::StreamLoad(std::ifstream &src)
{
   if (m_pWanne)
   {
      delete m_pWanne;
      m_pWanne = nullptr;
   }

   if (m_pTurm)
   {
      delete m_pTurm;
      m_pTurm = nullptr;
   }

   if (!src.is_open() || !src.good())
      return false;

   if (!StreamFun::StreamCheckHeader(src, STREAM_HEADER))
      return false;

   m_pWanne = new CImage();
   m_pWanne->StreamLoad(src);

   StreamFun::StreamReadPoint2D(src, m_TurnpointWanne);
   StreamFun::StreamReadPoint2D(src, m_TurmPosAufWanne);

   m_pTurm = new CImage();
   m_pTurm->StreamLoad(src);

   StreamFun::StreamReadPoint2D(src, m_TurnpointTurm);

   return true;
}

// ************************************************************************************************
CDynamicDamageModel *CTankModelBlueprint::CreateDDMTower(Real stability) const
{
   if (m_pTurm)
   {
      return new CDynamicDamageModel(m_pTurm, stability);
   }
   else
      return nullptr;
}

// ************************************************************************************************
CDynamicDamageModel *CTankModelBlueprint::CreateDDMWanne(Real stability) const
{
   if (m_pWanne)
   {
      return new CDynamicDamageModel(m_pWanne, stability);
   }
   else
      return nullptr;
}

// ************************************************************************************************
CPixelDim CTankModelBlueprint::GetDimensions() const
{
   if (m_pWanne)
   {
      return m_pWanne->GetDimensions();
   }
   else
   {
      return CPixelDim();
   }
}

// ################################################################################################
CTankSpecsBlueprint::CTankSpecsBlueprint(const CTankSpecsBlueprint &right)
   : CTankSpecsBlueprint(right.m_Name, right.m_MaxSpeed, right.m_Acceleration, right.m_TowerRotationSpeed, right.m_Stability)
{
}

// ************************************************************************************************
CTankSpecsBlueprint::CTankSpecsBlueprint(const String &name, KmPerH maxspeed, MPerS acceleration, DegPerS towerRotSpeed, Real stability)
   : m_Name(name),
   m_MaxSpeed(maxspeed),
   m_Acceleration(acceleration),
   m_TowerRotationSpeed(towerRotSpeed),
   m_Stability(stability)
{
}

// ################################################################################################
CTankBlueprint::CTankBlueprint(CTankModelBlueprint *pModel, const CTankSpecsBlueprint &specs)
   : m_pModel(pModel), m_Specs(specs)
{
}

// ************************************************************************************************
CTankBlueprint::~CTankBlueprint()
{
   if (m_pModel)
      delete m_pModel;
}

// ################################################################################################
CTankUsing::CTankUsing(const CTankBlueprint *pBlueprint, const CTilePos &pos, Degrees rot, Degrees towerrot, const IController *pController)
   : m_pBlueprint(pBlueprint),
   m_pDamageModelTurm(nullptr),
   m_pDamageModelWanne(nullptr),
   m_Pos(pos),
   m_Rot(rot),
   m_TowerRot(towerrot),
   m_pController(pController),
   m_CurrentSpeedLT(0.0),
   m_CurrentSpeedRT(0.0)
{
   InitDmgModels();
}

// ************************************************************************************************
CTankUsing::~CTankUsing()
{
   DestroyDmgModels();
}

// ************************************************************************************************
void CTankUsing::DestroyDmgModels()
{
   if (m_pDamageModelTurm)
   {
      delete m_pDamageModelTurm;
      m_pDamageModelTurm = nullptr;
   }

   if (m_pDamageModelWanne)
   {
      delete m_pDamageModelWanne;
      m_pDamageModelWanne = nullptr;
   }
}

// ************************************************************************************************
void CTankUsing::InitDmgModels()
{
   DestroyDmgModels();

   if (m_pBlueprint && Memory().m_TankBlueprints.IsValid(m_pBlueprint) && m_pBlueprint->m_pModel)
   {
      m_pDamageModelTurm = m_pBlueprint->m_pModel->CreateDDMTower(m_pBlueprint->m_Specs.m_Stability);
      m_pDamageModelWanne = m_pBlueprint->m_pModel->CreateDDMWanne(m_pBlueprint->m_Specs.m_Stability);
   }
}

// ************************************************************************************************
void CTankUsing::Draw(const CPixelPos &screen)
{
   if (Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      if (m_pDamageModelWanne)
      {
         m_pDamageModelWanne->Draw(screen, m_pBlueprint->m_pModel->TurnpointWanne, MathFun::NormalizeAngle(m_Rot), m_pBlueprint->m_pModel->TurnpointWanne, nullptr);
      }

      if (m_pDamageModelTurm)
      {
         CPixelPos diff = m_pBlueprint->m_pModel->TurmPosAufWanne - m_pBlueprint->m_pModel->TurnpointWanne;
         CPixelPos towerpos = MathFun::RotateAround(diff, CPixelPos(0, 0), m_Rot);

         m_pDamageModelTurm->Draw(
            towerpos + screen,
            m_pBlueprint->m_pModel->TurnpointTurm,
            MathFun::NormalizeAngle(m_Rot + m_TowerRot),
            m_pBlueprint->m_pModel->TurnpointTurm,
            nullptr);
      }
   }
}

// ************************************************************************************************
void CTankUsing::DoTowerUpdate()
{
   Real tower = m_pController->GetTowerMod();
   m_TowerRot += MathFun::Normalize(tower, -1.0, 1.0) * Settings().ToPerFrameValue(m_pBlueprint->m_Specs.m_TowerRotationSpeed);
}

// ************************************************************************************************
void CTankUsing::DoMovingUpdate(MPerS &side, Real mod)
{
   side += MathFun::Normalize(mod, -1.0, 1.0) * m_pBlueprint->m_Specs.m_Acceleration;
   side = MathFun::Normalize(side, -GetMaxTrackSpeed(), GetMaxTrackSpeed());

   if (mod == 0.0)
   {
      side -= m_pBlueprint->m_Specs.m_Acceleration / (Real)100.0;
   }
}

// ************************************************************************************************
MPerS CTankUsing::GetMaxTrackSpeed() const
{
   return MathFun::KmPerH2MPerS(m_pBlueprint->m_Specs.m_MaxSpeed);
}

// ************************************************************************************************
MPerS CTankUsing::GetMaxTrackSpeedTurn() const
{
   return MathFun::KmPerH2MPerS(8);
}

// ************************************************************************************************
void CTankUsing::ApplyUpdates()
{
   if (m_CurrentSpeedLT != 0.0 || m_CurrentSpeedRT != 0.0)
   {
      Int32 directionMod = m_CurrentSpeedLT < 0.0 ? -1 : 1;
      Meter curveRadius = 0.0;

      if (MathFun::HasDifferentSign(m_CurrentSpeedLT, m_CurrentSpeedRT))
      {
         // Turn �ber Mitte
         m_CurrentSpeedLT = MathFun::Normalize(m_CurrentSpeedLT, -GetMaxTrackSpeedTurn(), GetMaxTrackSpeedTurn());
         m_CurrentSpeedRT = MathFun::Normalize(m_CurrentSpeedRT, -GetMaxTrackSpeedTurn(), GetMaxTrackSpeedTurn());

         Meter kurvenUmfang = (Real)(Context().ToMeter(m_pBlueprint->m_pModel->GetDimensions().m_Width) * MathFun::PI);
         MPerS diffSpeed = abs(m_CurrentSpeedLT) + abs(m_CurrentSpeedRT);
         Seconds turnDuration = kurvenUmfang / diffSpeed;

         m_Rot += (Degrees)Settings().ToPerFrameValue(360.0 / turnDuration) * directionMod;
         m_Rot = MathFun::NormalizeAngle(m_Rot);
      }
      else if (m_CurrentSpeedLT == 0.0 || m_CurrentSpeedRT == 0.0)
      {
         // Turn �ber eine Seite
      }
      else
      {
         // Gleichl�ufig
      }
   }
}

// ************************************************************************************************
void CTankUsing::Update()
{
   if (Memory().m_Controller.IsValid(m_pController) && Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      DoTowerUpdate();
      DoMovingUpdate(m_CurrentSpeedLT, m_pController->GetLeftTrackMod());
      DoMovingUpdate(m_CurrentSpeedRT, m_pController->GetRightTrackMod());

      ApplyUpdates();
   }
}
