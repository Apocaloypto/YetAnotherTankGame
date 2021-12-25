#include "Tanks.h"
#include "Functions.h"
#include "Memory.h"


// ################################################################################################
const String STREAM_HEADER = "tnk";

// ################################################################################################
CTankBlueprint::CTankBlueprint()
   : CTankBlueprint(nullptr, CPixelPos(), CPixelPos(), nullptr, CPixelPos())
{
}

// ************************************************************************************************
CTankBlueprint::CTankBlueprint(CImage *pWanne, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, CImage *pTurm, const CPixelPos &turnpointTurm)
   : m_pWanne(pWanne),
   m_TurnpointWanne(turnpointWanne),
   m_TurmPosAufWanne(turmPosAufWanne),
   m_pTurm(pTurm),
   m_TurnpointTurm(turnpointTurm)
{
}

// ************************************************************************************************
bool CTankBlueprint::StreamSave(std::ofstream &dest) const
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
bool CTankBlueprint::StreamLoad(std::ifstream &src)
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
