#include "TankBuilder.h"

#include "../Tanks.h"
#include <fstream>


namespace Tanks
{
   bool CreateTank(const std::string &target, const std::string &wanne, const std::string &turm, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, const CPixelPos &turnpointTurm)
   {
      CImage *pWanne = CImage::CreateFrom(wanne);
      if (!pWanne)
         return false;

      CImage *pTurm = CImage::CreateFrom(turm);
      if (!pTurm)
      {
         delete pWanne;
         return false;
      }

      CTankModelBlueprint blueprt(pWanne, turnpointWanne, turmPosAufWanne, pTurm, turnpointTurm);

      std::ofstream dest(target, std::ios::binary);
      if (!dest.is_open() || !dest.good())
         return false;

      blueprt.StreamSave(dest);

      dest.close();

      return true;
   }
}
