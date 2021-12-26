#include "Tanks.h"
#include "Functions.h"
#include "Memory.h"
#include "DDM2D.h"


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

// ################################################################################################
CTankSpecsBlueprint::CTankSpecsBlueprint(const CTankSpecsBlueprint &right)
   : CTankSpecsBlueprint(right.m_Name, right.m_MaxSpeed, right.m_SecsTilMaxSpeed, right.m_Stability)
{
}

// ************************************************************************************************
CTankSpecsBlueprint::CTankSpecsBlueprint(const String &name, KmPerH maxspeed, Seconds secstilmaxspeed, Real stability)
   : m_Name(name), m_MaxSpeed(maxspeed), m_SecsTilMaxSpeed(secstilmaxspeed), m_Stability(stability)
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
CTankUsing::CTankUsing(const CTankBlueprint *pBlueprint, const CTilePos &pos, Degrees rot, Degrees towerrot)
   : m_pBlueprint(pBlueprint),
   m_pDamageModelTurm(nullptr),
   m_pDamageModelWanne(nullptr),
   m_Pos(pos),
   m_Rot(rot),
   m_TowerRot(towerrot)
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
void CTankUsing::Draw(const CPixelPos &screen, Degrees rot)
{
   if (Memory().m_TankBlueprints.IsValid(m_pBlueprint))
   {
      if (m_pDamageModelWanne)
      {
         m_pDamageModelWanne->Draw(screen, m_pBlueprint->m_pModel->TurnpointWanne, MathFun::NormalizeAngle(rot), m_pBlueprint->m_pModel->TurnpointWanne, nullptr);
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
