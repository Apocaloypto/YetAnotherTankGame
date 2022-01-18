#pragma once
#include <vector>


// ************************************************************************************************
class CGUIControl;

// ************************************************************************************************
class CGUIPage
{
private:
   std::vector<CGUIControl *> m_Controls;

public:
   CGUIPage(const std::vector<CGUIControl *> &controls);
   virtual ~CGUIPage();

   void Draw() const;
};
