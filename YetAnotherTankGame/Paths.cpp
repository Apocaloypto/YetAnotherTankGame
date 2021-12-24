#include "Paths.h"
#include <cassert>
#include <fstream>
#include "Functions.h"


using namespace std;

// ################################################################################################
PathsEntry CPaths::GetTypeFromString(const String &keyname)
{
   if (keyname == "reslist")
   {
      return PathsEntry::ResList;
   }
   else
   {
      assert(false);
      return PathsEntry::Undefined;
   }
}

// ************************************************************************************************
bool CPaths::Load(const String &file)
{
   m_Data.clear();

   ifstream src(file);

   if (!src.is_open() || !src.good())
   {
      return false;
   }

   String line;
   while (getline(src, line))
   {
      if (line.empty())
      {
         continue;
      }

      if (StringFun::StartsWith(line, "#"))
      {
         continue;
      }

      std::vector<String> splitted = StringFun::Split(line, '=');
      if (splitted.size() >= 2)
      {
         String key = splitted[0];
         String val = splitted[1];

         StringFun::Trim(key);
         StringFun::Trim(val);

         m_Data.insert_or_assign(GetTypeFromString(key), val);
      }
   }

   src.close();

   return true;
}

// ************************************************************************************************
String CPaths::Get(PathsEntry entry) const
{
   if (m_Data.count(entry) > 0)
   {
      return m_Data.at(entry);
   }
   else
   {
      return "";
   }
}
