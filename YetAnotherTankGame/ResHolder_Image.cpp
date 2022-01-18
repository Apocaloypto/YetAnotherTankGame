#include "ResHolder_Image.h"

#include "Image.h"
#include "Memory.h"
#include "Logging.h"
#include "DataPack.h"


// ################################################################################################
void CResHolderImages::DeleteElement(CImage *pObj)
{
   Memory().m_Images.Delete(pObj);
}


// ************************************************************************************************
bool CResHolderImages::Load(const String &id, const String &filename, const String &fileextra)
{
   CLogger logger("ImgLoader");

   CImage *pImg = Memory().m_Images.Add(CDataPack::ReadObject<CImage>(fileextra, filename));
   if (!pImg)
   {
      logger.Log(LogType::Error, "unable to load image '" + filename + "'");
      return false;
   }

   return Add(id, pImg);
}
