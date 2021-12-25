#include "Image.h"

#include "Window.h"
#include "Settings.h"
#include "Functions.h"
#include "Logging.h"


using namespace std;

const string STREAM_HEADER = "img";

// ################################################################################################
// Wert aus offizieller Dokumentation:
const sf::Color CImage::DEFAULT_SPRITE_COLOR = sf::Color(255, 255, 255, 255);

// ################################################################################################
CImage::CImage()
{
}

// ************************************************************************************************
CImage::~CImage()
{
}

// ************************************************************************************************
CImage::operator const sf::Sprite &() const
{
   return m_Sprite;
}

// ************************************************************************************************
CImage::operator sf::Sprite &()
{
   return m_Sprite;
}

// ************************************************************************************************
sf::Sprite &CImage::Sprite()
{
   return m_Sprite;
}

// ************************************************************************************************
sf::Texture &CImage::Texture()
{
   return m_Texture;
}

// ************************************************************************************************
const sf::Texture &CImage::Texture() const
{
   return m_Texture;
}

// ************************************************************************************************
CImage *CImage::CreateFrom(const sf::Image &img, UInt32 x, UInt32 y, UInt32 width, UInt32 height)
{
   CImage *pRetVal = new CImage;

   if (!pRetVal->Texture().loadFromImage(img, sf::IntRect(x, y, width, height)))
   {
      delete pRetVal;
      return nullptr;
   }

   pRetVal->Sprite().setTexture(pRetVal->Texture());

   return pRetVal;
}

// ************************************************************************************************
CImage *CImage::CreateFrom(const string &file, UInt32 x, UInt32 y, UInt32 width, UInt32 height)
{
   CImage *pRetVal = new CImage;

   if (!pRetVal->Texture().loadFromFile(file, sf::IntRect(x, y, width, height)))
   {
      delete pRetVal;
      return nullptr;
   }

   pRetVal->Sprite().setTexture(pRetVal->Texture());

   return pRetVal;
}

// ************************************************************************************************
void CImage::Draw(const CPixelPos &origin, const CPixelPos &screen, Degrees rot, const CPixelPos &rotorigin, sf::Color drawingColor, const CShader *pUseShader)
{
   // TODO: Use rotoriginx, rotoriginy
   Sprite().setOrigin(origin.m_X, origin.m_Y);
   Sprite().setPosition(screen.m_X, screen.m_Y);

   Sprite().setColor(drawingColor);

   sf::RenderStates states;

   states.shader = pUseShader;
   states.transform.rotate(rot, screen.m_X + rotorigin.m_X, screen.m_Y + rotorigin.m_Y);
   // states.transform.scale(scalevec.m_X, scalevec.m_Y);

   Window().draw(Sprite(), states);
}

// ************************************************************************************************
void CImage::DrawViewPort(const CPixelPos &screen, const CPixelDim &dim, const CPixelPos &origin, Degrees rot)
{
   Sprite().setOrigin(origin.m_X, origin.m_Y);
   Sprite().setPosition(screen.m_X + (dim.m_Width / 2.0f), screen.m_Y + (dim.m_Height / 2.0f));
   Sprite().setRotation(rot);

   sf::View prevView = Window().getView();
   sf::View miniView(sf::FloatRect(sf::Vector2f(screen.m_X, screen.m_Y), sf::Vector2f(dim.m_Width, dim.m_Height)));
   miniView.setViewport(sf::FloatRect(
      screen.m_X / Settings().ScreenWidth,
      screen.m_Y / Settings().ScreenHeight,
      dim.m_Width / Settings().ScreenWidth,
      dim.m_Height / Settings().ScreenHeight));

   Window().setView(miniView);

   Window().draw(Sprite());

   Window().setView(prevView);
}

// ************************************************************************************************
CPixelDim CImage::GetDimensions() const
{
   sf::Vector2u dim = Texture().getSize();
   return CPixelDim((Real)dim.x, (Real)dim.y);
}

// ************************************************************************************************
CPixelPos CImage::GetCenter() const
{
   CPixelDim dim = GetDimensions();
   return CPixelPos((Real)(dim.m_Width / 2.0), (Real)(dim.m_Height / 2.0));
}

// ************************************************************************************************
bool CImage::StreamSave(ofstream &dest)
{
   if (!dest.is_open() || !dest.good())
      return false;

   sf::Image img = Texture().copyToImage();

   StreamFun::StreamWriteHeader(dest, STREAM_HEADER);

   return StreamFun::SaveImageToStream(img, dest);
}

// ************************************************************************************************
bool CImage::StreamLoad(ifstream &src)
{
   CLogger logger("ImgLoad");

   if (!src.is_open() || !src.good())
   {
      logger.Log(LogType::Warning, "stream bad or not opened");
      return false;
   }

   if (!StreamFun::StreamCheckHeader(src, STREAM_HEADER))
   {
      logger.Log(LogType::Warning, "Wrong streamheader '" + STREAM_HEADER + "'");
      return false;
   }

   int pngStreamSize = 0;
   if (!StreamFun::StreamReadT(src, pngStreamSize) || pngStreamSize <= 0)
   {
      logger.Log(LogType::Warning, "Unable to read pngstream-size, or pngstream-size less than or equal to 0");
      return false;
   }

   char *pData = new char[pngStreamSize];
   if (!StreamFun::StreamReadBytes(src, pData, pngStreamSize))
   {
      logger.Log(LogType::Warning, "Failed to load " + to_string(pngStreamSize) + " bytes from stream");
      delete[] pData;
      return false;
   }

   if (!m_Texture.loadFromMemory(pData, pngStreamSize))
   {
      logger.Log(LogType::Warning, "Texture().loadFromMemory() failed!");
      delete[] pData;
      return false;
   }
   else
      delete[] pData;

   m_Sprite.setTexture(m_Texture);

   return true;
}
