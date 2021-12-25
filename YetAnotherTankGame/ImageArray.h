#pragma once
#include <vector>
#include "Typedefs.h"


class CImage;

class CImageArray
{
private:
   std::vector<CImage *> m_Images;

   void Clear();

public:
   CImageArray() = default;
   CImageArray(const std::vector<CImage *> &images);
   ~CImageArray();

   static CImageArray *CreateFrom(const std::string &file, UInt32 framewidth, UInt32 frameheight, UInt32 framecount);

   Int32 GetCount() const;
   CImage *GetAt(Int32 index);

   bool StreamSave(std::ofstream &dest) const;
   bool StreamLoad(std::ifstream &src);
};
