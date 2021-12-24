#include "Functions.h"
#include <algorithm>
#include <locale>
#include <iomanip>


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
