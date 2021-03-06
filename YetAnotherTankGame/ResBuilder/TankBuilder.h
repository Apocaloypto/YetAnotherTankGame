#pragma once
#include <string>
#include "../TypeDefs.h"
#include "../TemplateUsings.h"


namespace Tanks
{
   bool CreateTank(const std::string &target, const std::string &wanne, const std::string &turm, const CPixelPos &turnpointWanne, const CPixelPos &turmPosAufWanne, const CPixelPos &turnpointTurm);
}
