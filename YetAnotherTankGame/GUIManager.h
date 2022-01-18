#pragma once
#include <map>

// ************************************************************************************************
class CGUIPage;

// ************************************************************************************************
enum class GUIPage
{
   None,
   HUD,
};

// ************************************************************************************************
class CGUIManager
{
   friend CGUIManager &GUI();

private:
   std::map<GUIPage, CGUIPage *> m_Pages;
   GUIPage m_CurrentPage;

   CGUIManager();

   void InitPages();

public:
   virtual ~CGUIManager();

   void SetPage(GUIPage page);

   void Draw();
};

// ************************************************************************************************
CGUIManager &GUI();
