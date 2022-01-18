#include "GUIPage.h"

#include "GUIControl.h"


// ################################################################################################
CGUIPage::CGUIPage(const std::vector<CGUIControl *> &controls)
   : m_Controls(controls)
{
}

// ************************************************************************************************
CGUIPage::~CGUIPage()
{
   for (CGUIControl *pCtrl : m_Controls)
   {
      delete pCtrl;
   }

   m_Controls.clear();
}

// ************************************************************************************************
void CGUIPage::Draw() const
{
   for (const CGUIControl *pCtrl : m_Controls)
   {
      pCtrl->Draw();
   }
}
