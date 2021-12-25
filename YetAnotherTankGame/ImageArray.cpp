#include "ImageArray.h"

#include "Image.h"
#include "Logging.h"
#include "Functions.h"


using namespace std;

const string STREAM_HEADER = "igst";

// ################################################################################################
void ClearImageArray(vector<CImage *> &images)
{
   for (CImage *pImg : images)
   {
      delete pImg;
   }

   images.clear();
}

// ################################################################################################
CImageArray *CImageArray::CreateFrom(const string &file, UInt32 framewidth, UInt32 frameheight, UInt32 framecount)
{
   vector<CImage *> data;

   for (UInt32 frame = 0; frame < framecount; frame++)
   {
      CImage *pInsert = CImage::CreateFrom(file, frame * framewidth, 0, framewidth, frameheight);
      if (pInsert)
      {
         data.push_back(pInsert);
      }
      else
      {
         ClearImageArray(data);
         return nullptr;
      }
   }

   return new CImageArray(data);
}

// ************************************************************************************************
CImageArray::CImageArray(const vector<CImage *> &images)
   : m_Images(images)
{
}

// ************************************************************************************************
CImageArray::~CImageArray()
{
   Clear();
}

// ************************************************************************************************
void CImageArray::Clear()
{
   ClearImageArray(m_Images);
}

// ************************************************************************************************
Int32 CImageArray::GetCount() const
{
   return (Int32)m_Images.size();
}

// ************************************************************************************************
CImage *CImageArray::GetAt(Int32 index)
{
   if (index >= 0 && index < GetCount())
   {
      return m_Images[index];
   }
   else
   {
      return nullptr;
   }
}

// ************************************************************************************************
bool CImageArray::StreamSave(ofstream &dest)
{
   if (!dest.is_open() || !dest.good())
      return false;

   if (!StreamFun::StreamWriteHeader(dest, STREAM_HEADER))
      return false;

   if (!StreamFun::StreamWriteT(dest, GetCount()))
      return false;

   for (CImage *pFrame : m_Images)
   {
      if (!pFrame->StreamSave(dest))
         return false;
   }

   return true;
}

// ************************************************************************************************
bool CImageArray::StreamLoad(std::ifstream &src)
{
   CLogger logger("ImgStripLoad");

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

   Int32 frmcount = 0;
   if (!StreamFun::StreamReadT(src, frmcount))
   {
      logger.Log(LogType::Warning, "Unable to determine framecount");
      return false;
   }

   if (frmcount == 0)
      return src.good();

   Clear();

   for (Int32 frm = 0; frm < frmcount; frm++)
   {
      CImage *pFrm = new CImage();
      m_Images.push_back(pFrm);

      if (!pFrm->StreamLoad(src))
      {
         Clear();
         return false;
      }
   }

   return true;
}

