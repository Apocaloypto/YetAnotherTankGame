#include "TextFileWriter.h"
#include <fstream>


using namespace std;

// ################################################################################################
bool WriteTextFile(const string &filename, const vector<string> &lines)
{
   ofstream dest(filename);
   if (!dest.is_open() || !dest.good())
   {
      return false;
   }

   for (const string &line : lines)
   {
      dest << line << endl;
   }

   dest.close();
   return true;
}
