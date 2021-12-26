#pragma once
#include <string>


// Typedefs
typedef int Int32;
typedef unsigned int UInt32;
typedef __int64 Int64;

typedef float Real;
typedef Real Degrees;
typedef Real Radians;

typedef Real KmPerH;
typedef Real Seconds;
typedef Real KgTimesMPerS;
typedef Real Pixels;

typedef std::string String;

// misc
namespace sf
{
   class Shader;
}

typedef sf::Shader CShader;
