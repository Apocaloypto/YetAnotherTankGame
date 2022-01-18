#include "TileMap.h"

#include "ImageArray.h"
#include "Functions.h"
#include "Logging.h"
#include "Image.h"
#include "Memory.h"
#include "Settings.h"
#include "ITileMapObject.h"


using namespace std;

// ################################################################################################
CTileSet::CTileSet()
   : CTileSet(nullptr,0)
{
}

// ************************************************************************************************
CTileSet::CTileSet(CImageArray *pTiles, Int32 tilesize)
   : m_pTiles(pTiles), m_TileSize(tilesize)
{
}

// ************************************************************************************************
CTileSet::CTileSet(CTileSet &right)
   : m_pTiles(right.m_pTiles), m_TileSize(right.m_TileSize)
{
   right.m_pTiles = nullptr;
}

// ************************************************************************************************
CTileSet::~CTileSet()
{
   if (m_pTiles)
   {
      delete m_pTiles;
   }
}

// ************************************************************************************************
bool CTileSet::StreamSave(std::ofstream &dest) const
{
   if (!dest.is_open() || !dest.good())
      return false;

   if (!m_pTiles)
      return false;

   StreamFun::StreamWriteT(dest, m_TileSize);

   return m_pTiles->StreamSave(dest);
}

// ************************************************************************************************
bool CTileSet::StreamLoad(std::ifstream &src)
{
   if (m_pTiles)
   {
      delete m_pTiles;
      m_pTiles = nullptr;
   }

   if (!src.is_open() || !src.good())
      return false;

   StreamFun::StreamReadT(src, m_TileSize);

   m_pTiles = new CImageArray();
   if (m_pTiles->StreamLoad(src))
      return true;
   else
   {
      delete m_pTiles;
      m_pTiles = nullptr;
      return false;
   }
}

// ************************************************************************************************
void CTileSet::Draw(const CTileInfo &info, const CPixelPos &at, const CShader *pUseShader)
{
   if (m_pTiles)
   {
      CImage *pImg = m_pTiles->GetAt(info.GetId());
      if (pImg)
      {
         pImg->Draw(CPixelPos(), at, 0, CPixelPos(), CImage::DEFAULT_SPRITE_COLOR, pUseShader);
      }
   }
}

// ************************************************************************************************
Int32 CTileSet::GetTileSize() const
{
   return m_TileSize;
}

// ################################################################################################
CTileInfo::CTileInfo()
   : CTileInfo(0, false)
{
}

// ************************************************************************************************
CTileInfo::CTileInfo(const CTileInfo &right)
   : CTileInfo(right.m_Id, right.m_Collision)
{
}

// ************************************************************************************************
CTileInfo::CTileInfo(Int32 id, bool collision)
   : m_Id(id), m_Collision(collision)
{
}

// ************************************************************************************************
bool CTileInfo::StreamSave(std::ofstream &dest) const
{
   if (!dest.is_open() || !dest.good())
      return false;

   StreamFun::StreamWriteT(dest, m_Id);
   StreamFun::StreamWriteT(dest, m_Collision);

   return true;
}

// ************************************************************************************************
bool CTileInfo::StreamLoad(std::ifstream &src)
{
   if (!src.is_open() || !src.good())
      return false;

   StreamFun::StreamReadT(src, m_Id);
   StreamFun::StreamReadT(src, m_Collision);

   return true;
}

// ################################################################################################
CTileArray::CTileArray()
{
}

// ************************************************************************************************
CTileArray::CTileArray(CArray2D<CTileInfo *> &src)
{
   src.TransferTo(m_Data);
}

// ************************************************************************************************
CTileArray::CTileArray(CTileArray &right)
   : CTileArray(right.m_Data)
{
}

// ************************************************************************************************
CTileArray::~CTileArray()
{
   m_Data.ForEach([](Int32, Int32, CTileInfo *&pElem) -> void
      {
         if (pElem)
         {
            delete pElem;
            pElem = nullptr;
         }
      });
}

// ************************************************************************************************
bool CTileArray::StreamSave(std::ofstream &dest) const
{
   if (!dest.is_open() || !dest.good())
      return false;

   StreamFun::StreamWriteT(dest, m_Data.GetWidth());
   StreamFun::StreamWriteT(dest, m_Data.GetHeight());

   m_Data.ForEach([&](Int32, Int32, const CTileInfo * const &pElem) -> void
      {
         pElem->StreamSave(dest);
      });

   return true;
}

// ************************************************************************************************
bool CTileArray::StreamLoad(std::ifstream &src)
{
   if (!src.is_open() || !src.good())
      return false;

   Int32 width, height;
   StreamFun::StreamReadT(src, width);
   StreamFun::StreamReadT(src, height);

   m_Data.Resize(width, height);

   m_Data.ForEach([&](Int32, Int32, CTileInfo *&pElem) -> void
      {
         pElem = new CTileInfo();
         pElem->StreamLoad(src);
      });

   return true;
}

// ************************************************************************************************
const CTileInfo *CTileArray::GetAt(Int32 x, Int32 y) const
{
   return m_Data.GetAt(x, y);
}

// ################################################################################################
const string CTileMap::STREAM_HEADER = "tmap";

// ************************************************************************************************
CTileMap::CTileMap(CTileSet &tileset, CTileArray &tilearray, CImage *pMinimap)
   : m_Set(tileset), m_Tiles(tilearray), m_pMinimap(pMinimap)
{
}

// ************************************************************************************************
CTileMap::~CTileMap()
{
   if (m_pMinimap)
   {
      delete m_pMinimap;
   }
}

// ************************************************************************************************
bool CTileMap::StreamSave(std::ofstream &dest) const
{
   if (!m_pMinimap)
      return false;

   if (!dest.is_open() || !dest.good())
      return false;

   StreamFun::StreamWriteHeader(dest, STREAM_HEADER);

   m_Set.StreamSave(dest);
   m_Tiles.StreamSave(dest);
   m_pMinimap->StreamSave(dest);

   return true;
}

// ************************************************************************************************
bool CTileMap::StreamLoad(std::ifstream &src)
{
   if (m_pMinimap)
   {
      delete m_pMinimap;
   }

   CLogger logger("tmaploader");

   if (!src.is_open() || !src.good())
   {
      logger.Log(LogType::Error, "stream bad or not opened");
      return false;
   }

   if (!StreamFun::StreamCheckHeader(src, STREAM_HEADER))
   {
      logger.Log(LogType::Error, "wrong streamheader");
      return false;
   }

   if (!m_Set.StreamLoad(src))
   {
      logger.Log(LogType::Error, "unable to read tileset");
      return false;
   }

   if (!m_Tiles.StreamLoad(src))
   {
      logger.Log(LogType::Error, "unable to read tileinfo");
      return false;
   }

   m_pMinimap = new CImage;
   if (!m_pMinimap->StreamLoad(src))
   {
      logger.Log(LogType::Error, "unable to read minimap");
      delete m_pMinimap;
      m_pMinimap = nullptr;
      return false;
   }

   return true;
}

// ************************************************************************************************
CTilePos CTileMap::GetDrawTilePosCenteredMapObj(const ITileMapObject &mapobj)
{
   return mapobj.GetPosition() - (GetVisibleTileDim(false) / 2.0f).AsVector();
}

// ************************************************************************************************
void CTileMap::DrawMapObjects(const CTilePos &pos, const CTileDim &dim, const CShader *pUseShader)
{
   for (auto &[id, pMapObj] : m_MapObjects)
   {
      if (Memory().m_MapObjects.IsValid(pMapObj) && dim.ContainsPoint(pos, pMapObj->GetPosition()))
      {
         CTilePos resulting = pMapObj->GetPosition() - pos;
         pMapObj->Draw(resulting * (Real)m_Set.GetTileSize());
      }
   }
}

// ************************************************************************************************
void CTileMap::UpdateMapObjects(const CTilePos &pos, const CTileDim &dim)
{
   for (auto &[id, pMapObj] : m_MapObjects)
   {
      if (Memory().m_MapObjects.IsValid(pMapObj) && dim.ContainsPoint(pos, pMapObj->GetPosition()))
      {
         pMapObj->Update();
      }
   }
}

// ************************************************************************************************
ITileMapObject *CTileMap::GetMapObject(const std::string &id) const
{
   if (m_MapObjects.count(id) > 0)
   {
      ITileMapObject *pMapObj = m_MapObjects.at(id);
      if (Memory().m_MapObjects.IsValid(pMapObj))
      {
         return pMapObj;
      }
      else
         return nullptr;
   }
   else
      return nullptr;
}

// ************************************************************************************************
bool CTileMap::AddMapObject(const std::string &id, ITileMapObject *pMapObj)
{
   if (m_MapObjects.count(id) <= 0 && pMapObj)
   {
      m_MapObjects.insert(pair<string, ITileMapObject *>(id, pMapObj));
      return true;
   }
   else
      return false;
}

// ************************************************************************************************
void CTileMap::Draw(const CTilePos &pos, const CShader *pUseShader)
{
   const CTileDim DRAWDIM = GetVisibleTileDim(true);

   for (Int32 y = 0; y < DRAWDIM.m_Height; y++)
   {
      CPixelPos screenPos(-MathFun::GetFraction(pos.m_X) * m_Set.GetTileSize(), (Real)((y - MathFun::GetFraction(pos.m_Y)) * m_Set.GetTileSize()));

      for (Int32 x = 0; x < DRAWDIM.m_Width; x++)
      {
         const CTileInfo *pTile = m_Tiles.GetAt((Int32)pos.m_X + x, (Int32)pos.m_Y + y);
         if (pTile)
         {
            m_Set.Draw(*pTile, screenPos, pUseShader);
         }

         screenPos += CPixelPos((Real)m_Set.GetTileSize(), 0);
      }
   }

   DrawMapObjects(pos, DRAWDIM, pUseShader);
}

// ************************************************************************************************
void CTileMap::DrawFocused(const std::string &mapobjid, const CShader *pUseShader)
{
   ITileMapObject *pMapObj = GetMapObject(mapobjid);
   if (pMapObj)
   {
      Draw(GetDrawTilePosCenteredMapObj(*pMapObj), pUseShader);
   }
}

// ************************************************************************************************
void CTileMap::Update(const CTilePos &pos)
{
   const CTileDim DRAWDIM = GetVisibleTileDim(true);

   UpdateMapObjects(pos, DRAWDIM);
}

// ************************************************************************************************
void CTileMap::UpdateFocused(const std::string &mapobjid)
{
   ITileMapObject *pMapObj = GetMapObject(mapobjid);
   if (pMapObj)
   {
      Update(GetDrawTilePosCenteredMapObj(*pMapObj));
   }
}

// ************************************************************************************************
CTileDim CTileMap::GetVisibleTileDim(bool makeSure) const
{
   const int add = makeSure ? 1 : 0;

   return CTileDim((Real)(Settings().ScreenWidth / m_Set.GetTileSize() + add), 
      (Real)(Settings().ScreenHeight / m_Set.GetTileSize() + add)); // int-division wanted!
}
