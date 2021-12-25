#pragma once
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Typedefs.h"


// ************************************************************************************************
namespace StringFun
{
   bool StartsWith(const String &str, const String &chk);
   std::vector<String> Split(const String &whole, char delim);

   void TrimLeft(String &in);
   void TrimRight(String &in);
   void Trim(String &in);

   void ReplaceAll(String &str, const String &search, const String &replace);
}

// ************************************************************************************************
namespace MathFun
{
   Real GetFraction(Real whole);
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
