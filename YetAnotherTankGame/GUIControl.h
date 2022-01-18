#pragma once
#include "Typedefs.h"


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
   CGUIControl(Int32 vx, Int32 vy, Int32 vw, Int32 vh);

   Int32 GetX() const;
   Int32 GetY() const;
   Int32 GetW() const;
   Int32 GetH() const;

public:
   virtual ~CGUIControl();

   virtual void Draw() const = 0;
};
