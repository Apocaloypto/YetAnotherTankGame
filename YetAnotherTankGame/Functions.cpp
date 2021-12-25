#include "Functions.h"
#include <algorithm>
#include <locale>
#include <iomanip>


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
}

// ################################################################################################
namespace MathFun
{
   Real GetFraction(Real whole)
   {
      Real dontCare;
      return modf(whole, &dontCare);
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
