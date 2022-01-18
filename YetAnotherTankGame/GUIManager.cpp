#include "GUIManager.h"

#include "GUIPage.h"
#include "GUICtrl_Minimap.h"


// ################################################################################################
CGUIManager::CGUIManager()
   : m_CurrentPage(GUIPage::None)
{
   InitPages();
}

// ************************************************************************************************
CGUIManager::~CGUIManager()
{
   for (auto &[id, page] : m_Pages)
   {
      delete page;
   }

   m_Pages.clear();
}

// ************************************************************************************************
void CGUIManager::InitPages()
{
#define BEGIN_PAGE(page) m_Pages.insert(std::pair<GUIPage, CGUIPage *>(page, new CGUIPage({
#define END_PAGE() })));

   BEGIN_PAGE(GUIPage::HUD)
      new CGUICtrl_Minimap(0, 500, 100, 100)
   END_PAGE()

#undef END_PAGE
#undef BEGIN_PAGE
}

// ************************************************************************************************
void CGUIManager::SetPage(GUIPage page)
{
   m_CurrentPage = page;
}

// ************************************************************************************************
void CGUIManager::Draw()
{
   if (m_CurrentPage == GUIPage::None)
   {
      return;
   }

   m_Pages.at(m_CurrentPage)->Draw();
}

// ################################################################################################
CGUIManager &GUI()
{
   static CGUIManager inst;
   return inst;
}
