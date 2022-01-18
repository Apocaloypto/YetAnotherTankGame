#pragma once
#include <SFML/Graphics.hpp>
#include "Typedefs.h"
#include "TemplateUsings.h"


// ************************************************************************************************
class CGUIControl
{
private:
   static constexpr Int32 VIRTUAL_WIDTH = 800;
   static constexpr Int32 VIRTUAL_HEIGHT = 600;

   const Int32 m_VX;
   const Int32 m_VY;
   const Int32 m_VW;
   const Int32 m_VH;

   Real GetFactX() const;
   Real GetFactY() const;

protected:
   static constexpr Real GUI_BORDER_WIDTH = 2;
   static const sf::Color GUI_BACKGROUND_COLOR;
   static const sf::Color GUI_FOREGROUND_COLOR;

   CGUIControl(Int32 vx, Int32 vy, Int32 vw, Int32 vh);

   CPixelPos GetPos() const;
   CPixelDim GetDim() const;

public:
   virtual ~CGUIControl();

   virtual void Draw() const = 0;
};
