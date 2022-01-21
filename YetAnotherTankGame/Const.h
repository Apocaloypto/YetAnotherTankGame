#pragma once
#include "Typedefs.h"


namespace Const
{
   extern const String APP_TITLE;
   extern const String APP_AUTHOR;
   extern const String APP_VERSION;

   extern const String PATHS_FILE;

   constexpr Int32 DDM2D_MODEL_RESOLUTION = 5;
   constexpr Real DDM2D_IMPULSE_TO_DISPLACEMENT_MULT = (Real)1 / 16384;
   constexpr Real DDM2D_RANDOM_DISPLACEMENT = (Real)2.5;
};
