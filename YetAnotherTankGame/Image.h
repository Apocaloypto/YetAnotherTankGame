#pragma once
#include <SFML/Graphics.hpp>
#include "Typedefs.h"


class CImage
{
public:
   static const sf::Color DEFAULT_SPRITE_COLOR;

   CImage();
   virtual ~CImage();

   operator const sf::Sprite &() const;
   operator sf::Sprite &();

   static CImage *CreateFrom(const sf::Image &img, UInt32 x = 0, UInt32 y = 0, UInt32 w = 0, UInt32 h = 0);
   static CImage *CreateFrom(const std::string &file, UInt32 x = 0, UInt32 y = 0, UInt32 w = 0, UInt32 h = 0);

   sf::Sprite        &Sprite();
   sf::Texture       &Texture();
   const sf::Texture &Texture() const;

   CPixelDim GetDimensions() const;

   CPixelPos GetCenter() const;

   void Draw(const CPixelPos &origin, const CPixelPos &screen, Degrees rot, const CPixelPos &rotorigin = CPixelPos(), sf::Color drawingColor = DEFAULT_SPRITE_COLOR, const CShader *pUseShader = nullptr);
   void DrawViewPort(const CPixelPos &screen, const CPixelDim &dim, const CPixelPos &origin, Degrees rot);

   bool StreamSave(std::ofstream &dest);
   bool StreamLoad(std::ifstream &src);

private:
   sf::Texture m_Texture;
   sf::Sprite  m_Sprite;
};
