#pragma once
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Typedefs.h"
#include "TemplateUsings.h"


class CImage;

// ************************************************************************************************
namespace StringFun
{
   bool StartsWith(const String &str, const String &chk);
   std::vector<String> Split(const String &whole, char delim);

   void TrimLeft(String &in);
   void TrimRight(String &in);
   void Trim(String &in);

   void ReplaceAll(String &str, const String &search, const String &replace);

   bool TryParseReal(const String &in, Real &result);
   bool TryParseInt32(const String &in, Int32 &result);
}

// ************************************************************************************************
namespace MathFun
{
   constexpr Real PI = 3.14159265358979f;

   MPerS KmPerH2MPerS(KmPerH val);

   Radians DegToRad(Degrees deg);
   Degrees RadToDeg(Radians rad);

   Real GetFraction(Real whole);
   Real Normalize(Real in, Real lower, Real upper);

   Real ModuloFloat(Real in, int mod);

   Degrees NormalizeAngle(Degrees in);

   bool IsPointInTriangle(const CPoint2D<Real> &triPos1, const CPoint2D<Real> &triPos2, const CPoint2D<Real> &triPos3, const CPoint2D<Real> &ptCheck);

   void  InitRand();
   int   GetRandomNumber();
   int   GetRandomNumber(int lower, int upper);
   int   GetRandomNumberWithNegative(int lower, int upper);
   Real  GetRandomReal();
   Real  GetRandomRealBetween(Real lower, Real upper);
   bool  Maybe();

   CPoint2D<Real> RotateAround(const CPoint2D<Real> &base, const CPoint2D<Real> &origin, Degrees rot);
   Degrees GetRotationBetween(const CPoint2D<Real> &one, const CPoint2D<Real> &two);

   CPoint2D<Real> Move(const CPoint2D<Real> base, Degrees rot, Real dist);
}

// ************************************************************************************************
namespace StreamFun
{
   constexpr UInt32 MAX_STREAM_STRING_LENGTH = 255;
   
   bool StreamWriteBytes(std::ofstream &dest, char *val, size_t valsize);

   template<class T>
   inline bool StreamWriteT(std::ofstream &dest, const T &val)
   {
      return StreamWriteBytes(dest, (char *)&val, sizeof(T));
   }

   template<>
   inline bool StreamWriteT<std::string>(std::ofstream &dest, const std::string &val)
   {
      if (!StreamWriteT(dest, val.length()))
         return false;

      return StreamWriteBytes(dest, (char *)val.c_str(), val.length());

      return dest.good();
   }

   template<class T>
   inline bool StreamWritePoint2D(std::ofstream &dest, const CPoint2D<T> &val)
   {
      if (!StreamWriteT(dest, val.m_X))
         return false;

      if (!StreamWriteT(dest, val.m_Y))
         return false;

      return dest.good();
   }

   bool StreamWriteHeader(std::ofstream &dest, const std::string &header);

   bool StreamReadBytes(std::ifstream &src, char *&val, UInt32 valsize);

   template<class T>
   inline bool StreamReadT(std::ifstream &src, T &val)
   {
      if (!src.is_open() || !src.good())
         return false;

      src.read((char *)&val, sizeof(T));
      return src.good();
   }

   template<>
   inline bool StreamReadT<std::string>(std::ifstream &src, std::string &val)
   {
      size_t strlen = 0;
      if (!StreamReadT(src, strlen))
         return false;

      if (strlen == 0)
         return src.good();

      if (strlen > MAX_STREAM_STRING_LENGTH)
         return false;

      char *buf = new char[strlen + 1];
      src.read(buf, strlen);
      buf[strlen] = '\0';

      val = buf;
      delete[] buf;

      return src.good();
   }

   template<class T>
   inline bool StreamReadPoint2D(std::ifstream &src, CPoint2D<T> &val)
   {
      if (!StreamReadT(src, val.m_X))
         return false;

      if (!StreamReadT(src, val.m_Y))
         return false;

      return src.good();
   }

   bool StreamCheckHeader(std::ifstream &src, const std::string &header);

   bool SaveImageToStream(const sf::Image &img, std::ofstream &stream);
}

// ************************************************************************************************
namespace DrawFun
{
   void InitGL();

   void DrawTexturedPolygon(const CImage &image, Real xyuv[], size_t xyuvSize, const CPixelPos &screen, const CPixelPos &handle, CPixelPos rothandle, Degrees rot, const CShader *pUseShader);
}
