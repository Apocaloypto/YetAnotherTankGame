#include "Tanks.h"
#include "Functions.h"
#include "Memory.h"
#include "DDM2D.h"
#include "IController.h"
#include "Settings.h"
#include "Context.h"
#include <limits>


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
CTankSpecsBlueprint::CTankSpecsBlueprint(const String &name, KmPerH maxspeed, MPerS acceleration, DegPerS towerRotSpeed, Real stability, KG weight)
   : m_Name(name),
   m_MaxSpeed(maxspeed),
   m_Acceleration(acceleration),
   m_TowerRotationSpeed(towerRotSpeed),
   m_Stability(stability),
   m_Weight(weight)
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
void CTankUsing::DoTrackSlowDown(MPerS base, MPerS &newval, Real deduction) const
{
   if (base != 0.0)
   {
      if (deduction < abs(base))
      {
         if (base > 0.0)
         {
            deduction *= -1;
         }
         newval += deduction;
      }
      else
      {
         newval = 0.0;
      }
   }
}

// ************************************************************************************************
void CTankUsing::DoMovingUpdate(MPerS base, MPerS &newval, Real mod) const
{
   base += MathFun::Normalize(mod, -1.0, 1.0) * m_pBlueprint->m_Specs.m_Acceleration;
   newval = MathFun::Normalize(base, -GetMaxTrackSpeed(), GetMaxTrackSpeed());

   if (mod == 0.0)
   {
      DoTrackSlowDown(newval, newval, m_pBlueprint->m_Specs.m_Acceleration / (Real)100.0);
   }
}

// ************************************************************************************************
void CTankUsing::DoMovingUpdate(MPerS &side, Real mod)
{
   DoMovingUpdate(side, side, mod);
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
void CTankUsing::ApplyUpdates(MPerS &currentSpeedLT, MPerS &currentSpeedRT, CTilePos &newpos, Degrees &newrot) const
{
   if (m_CurrentSpeedLT != 0.0 || m_CurrentSpeedRT != 0.0)
   {
      if (MathFun::HasDifferentSign(m_CurrentSpeedLT, m_CurrentSpeedRT))
      {
         DoInPlaceTurn(currentSpeedLT, currentSpeedRT, newpos, newrot);
         return;
      }
      else if (m_CurrentSpeedLT == 0.0 || m_CurrentSpeedRT == 0.0)
      {
         DoTurnOverSide(currentSpeedLT, currentSpeedRT, newpos, newrot);
         return;
      }
      else
      {
         DoNormalDrive(newpos, newrot);
         return;
      }
   }

   newpos = m_Pos;
   newrot = m_Rot;
}

// ************************************************************************************************
void CTankUsing::DoNormalDrive(CTilePos &newpos, Degrees &newrot) const
{
   constexpr Meter MAX_CURVE_RADIUS = 100.0f;

   MPerS additiveSpeed = 0;
   if (m_CurrentSpeedLT > 0 && m_CurrentSpeedRT > 0)
   {
      additiveSpeed = std::max(m_CurrentSpeedLT, m_CurrentSpeedRT);
   }
   else
   {
      additiveSpeed = std::min(m_CurrentSpeedLT, m_CurrentSpeedRT);
   }

   // Erstmal Rotation:
   const Meter MIN_CURVE_RADIUS = Context().ToMeter((Real)(m_pBlueprint->m_pModel->GetDimensions().m_Width / 2.0));

   Int32 directionMod = 0;

   if (m_CurrentSpeedLT != m_CurrentSpeedRT)
   {
      if (m_CurrentSpeedLT > 0 && m_CurrentSpeedRT > 0)
      {
         Real ratio = 1;
         if (m_CurrentSpeedLT < m_CurrentSpeedRT)
         {
            // Linkskurve vorn
            ratio = m_CurrentSpeedLT / m_CurrentSpeedRT;
            directionMod = -1;
         }
         else
         {
            // Rechtskurve vorn
            ratio = m_CurrentSpeedRT / m_CurrentSpeedLT;
            directionMod = 1;
         }

         // ratio:
         // 0 => DoTurnOverSide -> Kurvenradius = halbe Tankbreite
         // 1 => Geradeaus -> Kurvenradius = Inf.
         Meter curveRadius = ratio * MAX_CURVE_RADIUS;
         if (curveRadius < MIN_CURVE_RADIUS)
         {
            curveRadius = MIN_CURVE_RADIUS;
         }

         newrot = m_Rot + CalcNewRotation(curveRadius, additiveSpeed, directionMod);
      }
      else
      {
         Real ratio = 1;
         if (m_CurrentSpeedLT > m_CurrentSpeedRT)
         {
            // Linkskurve hinten
            ratio = m_CurrentSpeedLT / m_CurrentSpeedRT;
            directionMod = 1;
         }
         else
         {
            // Rechtskurve hinten
            ratio = m_CurrentSpeedRT / m_CurrentSpeedLT;
            directionMod = -1;
         }

         // ratio:
         // 0 => DoTurnOverSide -> Kurvenradius = halbe Tankbreite
         // 1 => Geradeaus -> Kurvenradius = Inf.
         Meter curveRadius = ratio * MAX_CURVE_RADIUS;
         if (curveRadius < MIN_CURVE_RADIUS)
         {
            curveRadius = MIN_CURVE_RADIUS;
         }

         newrot = m_Rot - CalcNewRotation(curveRadius, additiveSpeed, directionMod);
      }
   }

   // Dann Geschwindigkeit:
   newpos = MathFun::Move(m_Pos, m_Rot, Settings().ToPerFrameValue(-additiveSpeed));
}

// ************************************************************************************************
Degrees CTankUsing::CalcNewRotation(Real curveRadius, MPerS speed, Int32 directionMod) const
{
   Meter kurvenUmfang = (Real)(2.0 * curveRadius * MathFun::PI);
   Seconds turnDuration = kurvenUmfang / speed;
   return (Degrees)Settings().ToPerFrameValue(360.0 / turnDuration) * directionMod;
}

// ************************************************************************************************
void CTankUsing::DoTurnOverSide(MPerS &currentSpeedLT, MPerS &currentSpeedRT, CTilePos &newpos, Degrees &newrot) const
{
   // Turn über eine Seite
   currentSpeedLT = MathFun::Normalize(currentSpeedLT, -GetMaxTrackSpeedTurn(), GetMaxTrackSpeedTurn());
   currentSpeedRT = MathFun::Normalize(currentSpeedRT, -GetMaxTrackSpeedTurn(), GetMaxTrackSpeedTurn());

   MPerS diffSpeed = m_CurrentSpeedLT + m_CurrentSpeedRT;

   Int32 directionMod = 0;
   if (diffSpeed > 0.0)
   {
      directionMod = m_CurrentSpeedLT > 0.0 || m_CurrentSpeedRT < 0.0 ? 1 : -1;
   }
   else
   {
      directionMod = m_CurrentSpeedLT < 0.0 || m_CurrentSpeedRT > 0.0 ? 1 : -1;
   }

   newrot = m_Rot + CalcNewRotation((Real)(Context().ToMeter(m_pBlueprint->m_pModel->GetDimensions().m_Width) / 2.0), diffSpeed, directionMod);
   newrot = MathFun::NormalizeAngle(newrot);

   newpos = MathFun::Move(m_Pos, m_Rot, Settings().ToPerFrameValue(-diffSpeed));
}

// ************************************************************************************************
void CTankUsing::DoInPlaceTurn(MPerS &currentSpeedLT, MPerS &currentSpeedRT, CTilePos &newpos, Degrees &newrot) const
{
   // Turn über Mitte
   Int32 directionMod = currentSpeedLT < 0.0 ? -1 : 1;

   currentSpeedLT = MathFun::Normalize(currentSpeedLT, -GetMaxTrackSpeedTurn(), GetMaxTrackSpeedTurn());
   currentSpeedRT = MathFun::Normalize(currentSpeedRT, -GetMaxTrackSpeedTurn(), GetMaxTrackSpeedTurn());

   Meter kurvenUmfang = (Real)(Context().ToMeter(m_pBlueprint->m_pModel->GetDimensions().m_Width) * MathFun::PI);
   MPerS diffSpeed = abs(currentSpeedLT) + abs(currentSpeedRT);
   Seconds turnDuration = kurvenUmfang / diffSpeed;

   newrot = m_Rot + (Degrees)Settings().ToPerFrameValue(360.0 / turnDuration) * directionMod;
   newrot = MathFun::NormalizeAngle(newrot);
}

// ************************************************************************************************
CCollisionRect CTankUsing::GetCollisionRect(const CPixelPos &pos) const
{
   if (Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      return CCollisionRect(m_pDamageModelWanne->GetDimensions(), pos, m_pBlueprint->m_pModel->TurnpointWanne, MathFun::NormalizeAngle(m_Rot));
   }
   else
   {
      return CCollisionRect();
   }
}

// ************************************************************************************************
CTilePosAndRot CTankUsing::PreUpdate() const
{
   if (Memory().m_Controller.IsValid(m_pController) && Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      MPerS currSpeedLT = m_CurrentSpeedLT;
      MPerS currSpeedRT = m_CurrentSpeedRT;

      DoMovingUpdate(m_CurrentSpeedLT, currSpeedLT, m_pController->GetLeftTrackMod());
      DoMovingUpdate(m_CurrentSpeedRT, currSpeedRT, m_pController->GetRightTrackMod());

      CTilePos newpos = m_Pos;
      Degrees newrot = m_Rot;

      ApplyUpdates(currSpeedLT, currSpeedRT, newpos, newrot);

      return CTilePosAndRot(newpos, newrot);
   }

   return CTilePosAndRot(m_Pos, m_Rot);
}

// ************************************************************************************************
void CTankUsing::DoUpdate(const CTilePosAndRot &newvals, bool collision)
{
   if (Memory().m_Controller.IsValid(m_pController) && Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      DoTowerUpdate();

      DoMovingUpdate(m_CurrentSpeedLT, m_CurrentSpeedLT, m_pController->GetLeftTrackMod());
      DoMovingUpdate(m_CurrentSpeedRT, m_CurrentSpeedRT, m_pController->GetRightTrackMod());
      
      if (!collision)
      {
         ApplyUpdates(m_CurrentSpeedLT, m_CurrentSpeedRT, m_Pos, m_Rot);
      }
   }
}

// ************************************************************************************************
Real CTankUsing::GetLeftTrackSpeedPercentage() const
{
   return abs(m_CurrentSpeedLT) / GetMaxTrackSpeed();
}

// ************************************************************************************************
Real CTankUsing::GetRightTrackSpeedPercentage() const
{
   return abs(m_CurrentSpeedRT) / GetMaxTrackSpeed();
}

// ************************************************************************************************
CPhysicalData CTankUsing::GetPhysicalData() const
{
   if (Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      CTilePosAndRot afterUpdate = PreUpdate();
      return CPhysicalData(CVector2D<Meter>(afterUpdate.m_NewPos.m_X - m_Pos.m_X, afterUpdate.m_NewPos.m_Y - m_Pos.m_Y), m_pBlueprint->m_Specs.m_Weight);
   }
   else
   {
      return CPhysicalData();
   }
}

// ************************************************************************************************
void CTankUsing::OnCollisionWithMapObject(const CCollisionRect &thisrect, const CCollisionRect &otherrect, const CPhysicalData &otherphysicaldata)
{
   m_pDamageModelWanne->Crash(thisrect.GetHandlePosition() - otherrect.GetHandlePosition(), GetPhysicalData().GetImpulse(otherphysicaldata), thisrect.GetIntersectingWidth(otherrect), m_Rot);
}
