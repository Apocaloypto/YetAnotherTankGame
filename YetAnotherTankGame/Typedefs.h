#pragma once
#include <string>
#include "Point2D.h"
#include "Vector2D.h"
#include "Dim2D.h"


// Typedefs
typedef int Int32;
typedef unsigned int UInt32;
typedef __int64 Int64;

typedef float Real;
typedef Real Degrees;

typedef std::string String;

// usings
using CPixelPos = CPoint2D<Real>;
using CPixelDim = CDim2D<Real>;

using CTilePos = CPoint2D<Real>;
using CTileDim = CDim2D<Real>;

// misc
namespace sf
{
   class Shader;
}

typedef sf::Shader CShader;
