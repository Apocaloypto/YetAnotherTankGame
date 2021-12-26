#include "Tanks.h"
#include "Functions.h"
#include "Memory.h"


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
