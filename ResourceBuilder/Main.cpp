#include <iostream>

#include "Const.h"
#include "FileSystem.h"
#include "Messages.h"
#include "TileMapCreator.h"
#include "TankBuilder.h"


// ************************************************************************************************
void CreateDirectories()
{
   CreateDir(CMB_DST("common"));
   CreateDir(CMB_DST("data"));
   CreateDir(CMB_DST("data/maps"));
   CreateDir(CMB_DST("data/tanks"));
}

// ************************************************************************************************
void CopyStaticFiles()
{
   CopyFile(CMB_SRC("StaticFiles/common/paths"), CMB_DST("common/paths"));

   CopyFile(CMB_SRC("StaticFiles/data/reslist.dat"), CMB_DST("data/reslist.dat"));
   CopyFile(CMB_SRC("StaticFiles/data/tanks.idl"), CMB_DST("data/tanks.idl"));
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
void BuildTanks()
{
   Tanks::CreateTank(
      CMB_DST("data/tanks/tank01.mdl"),
      CMB_SRC("Tanks/thefirst.png"),
      CMB_SRC("Tanks/thefirst_tower.png"),
      CPixelPos(50, 100),
      CPixelPos(50, 99),
      CPixelPos(48, 150));
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

   Msg::PrintL("Building tanks...");
   BuildTanks();

   Msg::PrintL("Done.");

   return 0;
}
