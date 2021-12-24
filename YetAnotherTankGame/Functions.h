#pragma once
#include <vector>
#include "Typedefs.h"


// ************************************************************************************************
namespace StringFun
{
   bool StartsWith(const String &str, const String &chk);
   std::vector<String> Split(const String &whole, char delim);

   void TrimLeft(String &in);
   void TrimRight(String &in);
   void Trim(String &in);
}
