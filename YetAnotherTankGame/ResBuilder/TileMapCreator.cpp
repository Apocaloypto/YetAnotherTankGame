#include "TileMapCreator.h"

#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include "../Functions.h"
#include "../TileMap.h"
#include "../Image.h"
#include "../ImageArray.h"


using namespace std;

// ################################################################################################
namespace TileMap
{
   // *********************************************************************************************
   CTileSet *CreateTileSet(const string &tileset, Int32 tilesize, Int32 tilescount)
   {
      CImageArray *pImages = CImageArray::CreateFrom(tileset, tilesize, tilesize, tilescount);
      if (!pImages)
         return nullptr;

      return new CTileSet(pImages, tilesize);
   }

   // *********************************************************************************************
   UInt32 ConvertHexStringToUInt(const std::string &str)
   {
      UInt32 retval;
      stringstream ss;
      ss << hex << str;
      ss >> retval;
      return retval;
   }

   // *********************************************************************************************
   bool ParseTileAssoc(map<UInt32, CTileInfo> &color2TileInfo, const string &tileassoc)
   {
      color2TileInfo.clear();

      ifstream stream;
      stream.open(tileassoc);

      if (!stream.is_open() || !stream.good())
         return false;

      string line;
      while (getline(stream, line))
      {
         StringFun::Trim(line);

         if (line.empty())
            continue;

         if (StringFun::StartsWith(line, "#"))
            continue;

         vector<string> splitted = StringFun::Split(line, ',');
         if (splitted.size() >= 3)
         {
            string strColor = splitted[0];
            string strTileId = splitted[1];
            string strColl = splitted[2];

            UInt32 color = ConvertHexStringToUInt(strColor);
            UInt32 tileid = stoi(strTileId);
            bool coll = (stoi(strColl) != 0);

            color2TileInfo.insert_or_assign(color, CTileInfo(tileid, coll));
         }
      }

      stream.close();
      return true;
   }

   // *********************************************************************************************
   CTileArray *CreateTileArray(const string &minimap, const string &tileassoc)
   {
      map<UInt32, CTileInfo> color2TileInfo;
      if (!ParseTileAssoc(color2TileInfo, tileassoc))
         return nullptr;

      CImage *pImg = CImage::CreateFrom(minimap);
      if (!pImg)
         return nullptr;

      sf::Image img = pImg->Texture().copyToImage();
      CArray2D<CTileInfo *> data(img.getSize().x, img.getSize().y);

      for (Int32 y = 0; y < pImg->GetDimensions().m_Height; y++)
      {
         for (Int32 x = 0; x < pImg->GetDimensions().m_Width; x++)
         {
            sf::Color col = img.getPixel(x, y);
            UInt32 colAsUInt32 = (col.a << 24 | col.r << 16 | col.g << 8 | col.b);

            if (color2TileInfo.count(colAsUInt32) > 0)
            {
               CTileInfo &tinfo = color2TileInfo.at(colAsUInt32);

               data.GetAt(x, y) = new CTileInfo(tinfo);
            }
            else
            {
               int i = 4;
            }
         }
      }

      delete pImg;

      return new CTileArray(data);
   }

   // *********************************************************************************************
   CTileMap *CreateTileMap(const string &minimap, const string &tileassoc, const string &tileset, Int32 tilesize, Int32 tilescount)
   {
      CTileArray *pTiles = CreateTileArray(minimap, tileassoc);
      if (!pTiles)
         return nullptr;

      CTileSet *pSet = CreateTileSet(tileset, tilesize, tilescount);
      if (!pSet)
      {
         // fuck memory-leaks
         return nullptr;
      }

      CTileMap *pRetVal = new CTileMap(*pSet, *pTiles);

      delete pSet;
      delete pTiles;

      return pRetVal;
   }

   // *********************************************************************************************
   bool MakeTileMap(const string &target, const string &minimap, const string &tileassoc, const string &tileset, Int32 tilesize, Int32 tilescount)
   {
      CTileMap *pMap = CreateTileMap(minimap, tileassoc, tileset, tilesize, tilescount);
      if (!pMap)
         return false;

      ofstream stream;
      stream.open(target, ios::binary);
      if (!stream.is_open())
         return false;

      pMap->StreamSave(stream);

      stream.close();

      delete pMap;

      return true;
   }
}

