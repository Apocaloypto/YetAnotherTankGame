#include <iostream>

#include "Const.h"
#include "FileSystem.h"
#include "Messages.h"
#include "TileMapCreator.h"


// ************************************************************************************************
void CreateDirectories()
{
   CreateDir(CMB_DST("common"));
   CreateDir(CMB_DST("data"));
   CreateDir(CMB_DST("data/maps"));
}

// ************************************************************************************************
void CopyStaticFiles()
{
   CopyFile(CMB_SRC("StaticFiles/common/paths"), CMB_DST("common/paths"));
   CopyFile(CMB_SRC("StaticFiles/data/reslist.dat"), CMB_DST("data/reslist.dat"));
}

// ************************************************************************************************
void CompileTileMaps()
{
#define STD_PARAMS(dir) CMB_SRC(dir "/map.png"), CMB_SRC(dir "/assoc.txt"), CMB_SRC(dir "/set.png")

   TileMap::MakeTileMap(
      CMB_DST("data/maps/map01.map"),
      STD_PARAMS("Maps/Map01"),
      32, 
      5);

#undef STD_PARAMS
}

// ************************************************************************************************
int main(int argc, char **argv)
{
   Msg::PrintL("Building resources to '" DST_DIR "'...");

   Msg::PrintL("Creating directories...");
   CreateDirectories();

   Msg::PrintL("Copying static files...");
   CopyStaticFiles();

   Msg::PrintL("Compiling tilemaps...");
   CompileTileMaps();

   Msg::PrintL("Done.");

   return 0;
}
