#pragma once
#include <string>
#include "../YetAnotherTankGame/TypeDefs.h"


class CTileMap;

namespace TileMap
{
   bool MakeTileMap(const std::string &target, const std::string &minimap, const std::string &tileassoc, const std::string &tileset, Int32 tilesize, Int32 tilescount);
}
