#include <iostream>

#include "Const.h"
#include "FileSystem.h"
#include "Messages.h"


// ************************************************************************************************
void CreateDirectories()
{
   CreateDir(CMB_DST("common"));
   CreateDir(CMB_DST("data"));
}

// ************************************************************************************************
void CopyStaticFiles()
{
   CopyFile(CMB_SRC("StaticFiles/common/paths"), CMB_DST("common/paths"));
   CopyFile(CMB_SRC("StaticFiles/data/reslist.dat"), CMB_DST("data/reslist.dat"));
}

// ************************************************************************************************
int main(int argc, char **argv)
{
   Msg::PrintL("Building resources to '" DST_DIR "'...");

   Msg::PrintL("Creating directories...");
   CreateDirectories();

   Msg::PrintL("Copying static files...");
   CopyStaticFiles();

   Msg::PrintL("Done.");

   return 0;
}
