#include "Functions.h"
#include "Window.h"
#include "Image.h"
#include <algorithm>
#include <locale>
#include <iomanip>
#include <SFML/OpenGL.hpp>


typedef void stbi_write_func(void *context, void *data, int size);
extern "C" int stbi_write_png_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data, int stride_in_bytes);

// ################################################################################################
namespace StringFun
{
   bool StartsWith(const String &str, const String &chk)
   {
      return str.rfind(chk, 0) == 0;
   }

   std::vector<String> Split(const String &whole, char delim)
   {
      std::vector<String> tokens;
      size_t prev = 0, pos = 0;

      do
      {
         pos = whole.find(delim, prev);
         if (pos == String::npos)
            pos = whole.length();

         String token = whole.substr(prev, pos - prev);
         if (!token.empty())
            tokens.push_back(token);

         prev = pos + 1;
      } while (pos < whole.length() && prev < whole.length());

      return tokens;
   }

   void TrimLeft(String &in)
   {
      auto it = std::find_if(in.begin(), in.end(), [](char ch) -> bool
         {
            return !std::isspace<char>(ch, std::locale::classic());
         });

      in.erase(in.begin(), it);
   }

   void TrimRight(String &in)
   {
      auto it = find_if(in.rbegin(), in.rend(), [](char ch) -> bool
         {
            return !std::isspace<char>(ch, std::locale::classic());
         });

      in.erase(it.base(), in.end());
   }

   void Trim(String &in)
   {
      TrimLeft(in);
      TrimRight(in);
   }

   void ReplaceAll(String &str, const String &search, const String &replace)
   {
      if (search.empty())
         return;

      size_t start_pos = 0;
      while ((start_pos = str.find(search, start_pos)) != String::npos)
      {
         str.replace(start_pos, search.length(), replace);
         start_pos += replace.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
      }
   }

   bool TryParseReal(const String &in, Real &result)
   {
      try
      {
         result = std::stof(in);
         return true;
      }
      catch (...)
      {
         return false;
      }
   }
   
   bool TryParseInt32(const String &in, Int32 &result)
   {
      try
      {
         result = std::stoi(in);
         return true;
      }
      catch (...)
      {
         return false;
      }
   }
}

// ################################################################################################
namespace MathFun
{
   MPerS KmPerH2MPerS(KmPerH val)
   {
      return (MPerS)(val / 3.6);
   }

   Radians DegToRad(Degrees deg)
   {
      return deg * (PI / 180.0f);
   }

   Degrees RadToDeg(Radians rad)
   {
      return rad * (180.0f / PI);
   }

   Real GetFraction(Real whole)
   {
      Real dontCare;
      return modf(whole, &dontCare);
   }

   Real Normalize(Real in, Real lower, Real upper)
   {
      if (in < lower)
         return lower;
      else if (in > upper)
         return upper;
      else
         return in;
   }

   Real ModuloFloat(Real in, int mod)
   {
      int inInInt  = (int)in;
      int rest     = inInInt % mod;

      return (Real)rest + (in - inInInt);
   }

   Degrees NormalizeAngle(Degrees in)
   {
      if (in < 0)
      {
         while (in < 0)
            in += 360;

         return in;
      }
      else if (in >= 360)
         return ModuloFloat(in, 360);
      else
         return in;
   }

   Real Sign(const CPoint2D<Real> &pos1, const CPoint2D<Real> &pos2, const CPoint2D<Real> &pos3)
   {
      return (pos1.m_X - pos3.m_X) * (pos2.m_Y - pos3.m_Y) - (pos2.m_X - pos3.m_X) * (pos1.m_Y - pos3.m_Y);
   }

   bool IsPointInTriangle(const CPoint2D<Real> &triPos1, const CPoint2D<Real> &triPos2, const CPoint2D<Real> &triPos3, const CPoint2D<Real> &ptCheck)
   {
      Real d1 = Sign(ptCheck, triPos1, triPos2);
      Real d2 = Sign(ptCheck, triPos2, triPos3);
      Real d3 = Sign(ptCheck, triPos3, triPos1);

      bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
      bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

      return !(has_neg && has_pos);
   }

   void InitRand()
   {
      srand((UInt32)time(nullptr));
   }

   int GetRandomNumber()
   {
      return rand();
   }

   int GetRandomNumber(int lower, int upper)
   {
      if (upper - lower == 0)
         return 0;
      else
         return (GetRandomNumber() % (upper - lower)) + lower;
   }

   int GetRandomNumberWithNegative(int lower, int upper)
   {
      return GetRandomNumber(0, upper) - lower;
   }

   Real GetRandomReal()
   {
      return GetRandomNumber() / (Real)RAND_MAX;
   }

   Real GetRandomRealBetween(Real lower, Real upper)
   {
      return (GetRandomReal() * abs(upper - lower)) + abs(lower);
   }

   bool Maybe()
   {
      return GetRandomNumber(0, 2) == 1;
   }

   CPoint2D<Real> RotateAround(const CPoint2D<Real> &base, const CPoint2D<Real> &origin, Degrees rot)
   {
      Real sin_angle = sin(DegToRad(rot));
      Real cos_angle = cos(DegToRad(rot));

      Real basex = base.m_X;
      Real basey = base.m_Y;

      basex -= origin.m_X;
      basey -= origin.m_Y;

      Real xnew = basex * cos_angle - basey * sin_angle;
      Real ynew = basex * sin_angle + basey * cos_angle;

      return CPoint2D<Real>(xnew + origin.m_X, ynew + origin.m_Y);
   }

   Degrees GetRotationBetween(const CPoint2D<Real> &one, const CPoint2D<Real> &two)
   {
      return RadToDeg(atan2(two.m_Y - one.m_Y, two.m_X - one.m_X)) + 90.0f;
   }

   CPoint2D<Real> Move(const CPoint2D<Real> base, Degrees rot, Real dist)
   {
      return CPoint2D<Real>(base.m_X + cos(MathFun::DegToRad(rot + 90)) * dist, base.m_Y + sin(MathFun::DegToRad(rot + 90)) * dist);
   }

   bool HasDifferentSign(Real one, Real two)
   {
      if (one > 0.0 && two < 0.0)
      {
         return true;
      }
      else if (one < 0.0 && two > 0.0)
      {
         return true;
      }
      else
      {
         return false;
      }
   }

   CPoint2D<Real> CalcRotatedPointOnOuterEdge(const CDim2D<Real> &area, Degrees rot)
   {
      rot = NormalizeAngle(rot);

      const Degrees wi = RadToDeg(atan2(area.m_Width, area.m_Height));

      const Real cosrot = cos(MathFun::DegToRad(rot));
      const Real sinrot = sin(MathFun::DegToRad(rot));

      if (rot >= wi && rot < 180 - wi)
      {
         return CPoint2D<Real>(area.m_Width / 2 - area.m_Height / 2 * cosrot / sinrot, 0);
      }
      else if (rot >= 180 - wi && rot < 180 + wi)
      {
         return CPoint2D<Real>(area.m_Width, area.m_Height / 2 + area.m_Width / 2 * sinrot / cosrot);
      }
      else if (rot >= 180 + wi && rot < 360 - wi)
      {
         return CPoint2D<Real>(area.m_Width / 2 + area.m_Height / 2 * cosrot / sinrot, area.m_Height);
      }
      else if ((rot >= 360 - wi && rot < 360) || (rot >= 0 && rot < wi))
      {
         return CPoint2D<Real>(0, area.m_Height - (area.m_Height / 2 + area.m_Width / 2 * sinrot / cosrot));
      }
      else
      {
         return CPoint2D<Real>();
      }
   }
}

// ################################################################################################
namespace StreamFun
{
   bool StreamWriteBytes(std::ofstream &dest, char *val, size_t valsize)
   {
      if (!dest.is_open() || !dest.good())
         return false;

      dest.write((char *) val, valsize);
      return dest.good();
   }

   // ************************************************************************************************
   bool StreamWriteHeader(std::ofstream &dest, const std::string &header)
   {
      if (!dest.is_open() || !dest.good())
         return false;

      dest.write(header.c_str(), header.length());
      return dest.good();
   }

   // ************************************************************************************************
   bool StreamReadBytes(std::ifstream &src, char *&val, UInt32 valsize)
   {
      if (!src.is_open() || !src.good())
         return false;

      src.read((char *&)val, valsize);
      return src.good();
   }

   // ************************************************************************************************
   bool StreamCheckHeader(std::ifstream &src, const std::string &header)
   {
      if (!src.is_open() || !src.good())
         return false;
      
      for (size_t ch = 0; ch < header.length(); ch++)
      {
         if (char read; !StreamReadT(src, read) || header[ch] != read)
         {
            return false;
         }
      }

      return true;
   }

   // ************************************************************************************************
   void SaveImageToStreamHelper(void *pContext, void *pData, int size)
   {
      std::ofstream *pStream = (std::ofstream *)pContext;
      if (!pStream || !pStream->is_open() || !pStream->good())
         return;

      StreamFun::StreamWriteT(*pStream, size);
      StreamFun::StreamWriteBytes(*pStream, (char *)pData, size);
   }

   // ************************************************************************************************
   bool SaveImageToStream(const sf::Image &img, std::ofstream &stream)
   {
      if (!stream.is_open() || !stream.good())
         return false;

      return stbi_write_png_to_func(&SaveImageToStreamHelper, &stream, img.getSize().x, img.getSize().y, 4, img.getPixelsPtr(), 0) != 0;
   }
}

// ################################################################################################
namespace DrawFun
{
   void InitGL()
   {
      glEnable(GL_DEPTH_TEST);
      glDepthMask(GL_TRUE);
      glClearDepth(1.0f);
      glDisable(GL_LIGHTING);
      glViewport(0, 0, Window().getSize().x, Window().getSize().y);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      GLfloat ratio = (GLfloat)(static_cast<Real>(Window().getSize().x) / Window().getSize().y);
      glFrustum(-ratio, ratio, -1.0f, 1.0f, 1.0f, 500.0f);
   }

   // *********************************************************************************************
   void DrawTexturedPolygon(const CImage &image, Real xyuv[], size_t xyuvSize, const CPixelPos &screen, const CPixelPos &handle, CPixelPos rothandle, Degrees rot, const CShader *pUseShader)
   {
      if (xyuvSize < 6)
         return;

      rothandle = MathFun::RotateAround(rothandle, handle, rot);

      Window().resetGLStates();

      CShader::bind(pUseShader);

      glBindTexture(GL_TEXTURE_2D, image.Texture().getNativeHandle());
      glEnable(GL_TEXTURE_2D);

      glBegin(GL_POLYGON);
         for (int i = 0; i < xyuvSize; i += 4)
         {
            Real u = xyuv[i + 2];
            Real v = xyuv[i + 3];
            glTexCoord2d(u, v);

            CPoint2D<Real> xy(xyuv[i + 0], xyuv[i + 1]);

            xy = MathFun::RotateAround(xy, rothandle, rot);
            xy = xy - handle;

            glVertex2d((xy.m_X + screen.m_X), (xy.m_Y + screen.m_Y));
         }
      glEnd();

      CShader::bind(nullptr);

      Window().resetGLStates();
   }

   // *********************************************************************************************
   void DrawRect(const CPixelPos &pos, const CPixelDim &dim, sf::Color fill, Real borderwidth, sf::Color bordercolor)
   {
      sf::RectangleShape shape(sf::Vector2f(dim.m_Width, dim.m_Height));
      shape.setPosition(pos.m_X, pos.m_Y);
      shape.setFillColor(fill);
      shape.setOutlineThickness(borderwidth);

      if (borderwidth != 0)
      {
         if (bordercolor != INVISIBLE_COLOR)
            shape.setOutlineColor(bordercolor);
         else
            shape.setOutlineColor(fill);
      }

      Window().draw(shape);
   }
}
