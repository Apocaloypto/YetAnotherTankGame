#include "FileSystem.h"

#include <filesystem>
#include "Messages.h"

using namespace std;

// ************************************************************************************************
bool CreateDir(const string &path)
{
   using namespace filesystem;

   return create_directory(path);
}

// ************************************************************************************************
bool CopyFile(const string &src, const string &dest)
{
   using namespace filesystem;

   error_code ec;
   copy(src, dest, copy_options::overwrite_existing, ec);
   
   if (ec)
   {
      Msg::PrintL("Error copying file '" + src + "': " + ec.message());
      return false;
   }
   else
   {
      return true;
   }
}
