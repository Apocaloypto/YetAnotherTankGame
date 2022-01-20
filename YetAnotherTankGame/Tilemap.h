#pragma once
#include <map>
#include <string>
#include "TypeDefs.h"
#include "Array2D.h"
#include "Point2D.h"
#include "Dim2D.h"
#include "TemplateUsings.h"


class CImage;
class CImageArray;
class CTileInfo;
class ITileMapObject;

// ************************************************************************************************
class CTileSet
{
private:
   CImageArray *m_pTiles;
   Int32 m_TileSize;

public:
   CTileSet();
   CTileSet(CImageArray *pTiles, Int32 tilesize);
   CTileSet(CTileSet &right);
   ~CTileSet();

   void Draw(const CTileInfo &info, const CPixelPos &at, const CShader *pUseShader = nullptr);
   Int32 GetTileSize() const;

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};

// ************************************************************************************************
class CTileInfo
{
private:
   Int32 m_Id;
   bool m_Collision;

public:
   CTileInfo();
   CTileInfo(const CTileInfo &right);
   CTileInfo(Int32 id, bool collision);

   Int32 GetId() const { return m_Id; }
   bool HasCollision() const { return m_Collision; }

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};

// ************************************************************************************************
class CTileArray
{
private:
   CArray2D<CTileInfo *> m_Data;

public:
   CTileArray();
   CTileArray(CArray2D<CTileInfo *> &src);
   CTileArray(CTileArray &right);
   ~CTileArray();

   const CTileInfo *GetAt(const CTilePos &pos) const;

   Int32 GetWidth() const;
   Int32 GetHeight() const;

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};

// ************************************************************************************************
class CTileMap
{
private:
   static const std::string STREAM_HEADER;

   CTileSet m_Set;
   CTileArray m_Tiles;
   CImage *m_pMinimap = nullptr;

   std::map<std::string, ITileMapObject *> m_MapObjects;

   void DrawMapObjects(const CTilePos &pos, const CTileDim &dim, const CShader *pUseShader);
   void UpdateMapObjects(const CTilePos &pos, const CTileDim &dim);

   ITileMapObject *GetMapObject(const std::string &id) const;

   CTilePos GetDrawTilePosCenteredMapObj(const ITileMapObject &mapobj);

   bool FireTileCollEventIfNecessary(ITileMapObject &mapobj, const CTilePos &newpos, Degrees newrot) const;
   bool FireEndOfMapCollEventIfNecessary(ITileMapObject &mapobj, const CTilePos &newpos, Degrees newrot) const;

public:
   CTileMap() = default;
   CTileMap(CTileSet &tileset, CTileArray &tilearray, CImage *pMinimap);
   virtual ~CTileMap();

   const CTileSet &TileSet = m_Set;
   CImage *MiniMap() { return m_pMinimap; }

   CTileDim GetVisibleTileDim(bool makeSure) const;

   CTilePos GetMapObjectPos(const std::string &id) const;
   Degrees GetMapObjectRotation(const std::string &id) const;

   bool AddMapObject(const std::string &id, ITileMapObject *pMapObj);

   void Draw(const CTilePos &pos, const CShader *pUseShader = nullptr);
   void DrawFocused(const std::string &mapobjid, const CShader *pUseShader = nullptr);

   void Update(const CTilePos &pos);
   void UpdateFocused(const std::string &mapobjid);

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};
