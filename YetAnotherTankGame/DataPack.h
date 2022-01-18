#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <filesystem>
#include "Functions.h"


class CDataPack
{
private:
   static const std::string DATAPACK_HEADER;

   struct CFileTableEntry
   {
      std::string m_Identifier;
      size_t      m_StartAddress;

      CFileTableEntry(const std::string &ident, size_t startaddr)
         : m_Identifier(ident),
           m_StartAddress(startaddr)
      {
      }
   };

   static size_t GetFileSize(const std::string &filename)
   {
      using namespace std::filesystem;

      std::error_code ec;
      size_t retval = (size_t) file_size(filename, ec);
      return !ec ? retval : 0;
   }

   static bool DeleteFile(const std::string &filename)
   {
      using namespace std::filesystem;

      std::error_code ec;
      remove(filename, ec);
      return !ec;
   }

public:
   template<class TStreamable>
   using StreamableCreator = std::function<TStreamable(const std::string &file)>;

   template<class TStreamable>
   using StreamableDeleter = std::function<void(TStreamable)>;

   struct CMetaFileTable
   {
      const std::string m_Identifier;
      const std::string m_BaseFile;

      CMetaFileTable(const std::string &ident, const std::string &basefile)
         : m_Identifier(ident),
           m_BaseFile(basefile)
      {
      }
   };

private:
   template<class TStreamable>
   static void CreateFileTable(std::vector<CFileTableEntry> &result, const std::vector<CMetaFileTable> &files, const StreamableCreator<TStreamable> &creator, const StreamableDeleter<TStreamable> &deleter)
   {
      size_t laststartaddress = 0;
      for (const CMetaFileTable &file : files)
      {
         result.push_back(CFileTableEntry(file.m_Identifier, laststartaddress));

         TStreamable pStreamable = creator(file.m_BaseFile);

         const std::string TEMP_FILE = file.m_BaseFile + ".temp";

         std::ofstream out;
         out.open(TEMP_FILE, std::ios::binary);
         if (pStreamable->StreamSave(out))
         {
            out.close();
            
            size_t filesize = GetFileSize(TEMP_FILE);
            if (filesize != 0)
               laststartaddress += filesize;
         }
         else
            out.close();

         DeleteFile(TEMP_FILE);

         deleter(pStreamable);
      }
   }

   static void WriteFileTable(std::ofstream &target, const std::vector<CFileTableEntry> &filetable)
   {
      StreamFun::StreamWriteT(target, filetable.size());

      for (const CFileTableEntry &fte : filetable)
      {
         StreamFun::StreamWriteT(target, fte.m_Identifier);
         StreamFun::StreamWriteT(target, fte.m_StartAddress);
      }
   }

   template<class TStreamable>
   static void WriteFiles(std::ofstream &target, const std::vector<CMetaFileTable> &files, const StreamableCreator<TStreamable> &creator, const StreamableDeleter<TStreamable> &deleter)
   {
      for (const CMetaFileTable &file : files)
      {
         TStreamable pStreamable = creator(file.m_BaseFile);

         pStreamable->StreamSave(target);

         deleter(pStreamable);
      }
   }

public:
   template<class TStreamable>
   static bool WritePackage(const std::string &target, const std::vector<CMetaFileTable> &files, const StreamableCreator<TStreamable> &creator, const StreamableDeleter<TStreamable> &deleter)
   {
      std::ofstream targetstream;
      targetstream.open(target, std::ios::binary);
      if (!targetstream.is_open() || !targetstream.good())
         return false;

      // Header:
      StreamFun::StreamWriteT(targetstream, DATAPACK_HEADER);

      // FileTable:
      std::vector<CFileTableEntry> filetable;
      CreateFileTable<TStreamable>(filetable, files, creator, deleter);
      WriteFileTable(targetstream, filetable);

      // Data:
      WriteFiles<TStreamable>(targetstream, files, creator, deleter);

      targetstream.close();
      return true;
   }

private:
   static bool ReadFileTable(std::vector<CFileTableEntry> &result, std::ifstream &source)
   {
      size_t ftsize = 0;
      if (!StreamFun::StreamReadT(source, ftsize))
         return false;

      for (size_t ftentry = 0; ftentry < ftsize; ftentry++)
      {
         std::string ident;
         size_t start;

         if (!StreamFun::StreamReadT(source, ident) || !StreamFun::StreamReadT(source, start))
            return false;

         result.push_back(CFileTableEntry(ident, start));
      }

      return true;
   }

   static const CFileTableEntry *FindFileTableEntry(const std::vector<CFileTableEntry> &filetable, const std::string &identifier)
   {
      auto it = std::find_if(filetable.begin(), filetable.end(), 
         [&](const CFileTableEntry &ftentry) -> bool 
         {
            return ftentry.m_Identifier == identifier;
         });

      if (it != filetable.end())
         return &(*it);
      else
         return nullptr;
   }

public:
   template<class TStreamable>
   static TStreamable *ReadObject(const std::string &package, const std::string &identifier)
   {
      std::ifstream packagestream;
      packagestream.open(package, std::ios::binary);
      if (!packagestream.is_open() || !packagestream.good())
         return nullptr;

      // header:
      if (std::string header; !StreamFun::StreamReadT(packagestream, header) || header != DATAPACK_HEADER)
      {
         packagestream.close();
         return nullptr;
      }

      // FileTable:
      std::vector<CFileTableEntry> filetable;
      if (!ReadFileTable(filetable, packagestream))
      {
         packagestream.close();
         return nullptr;
      }

      // find start-address:
      const CFileTableEntry *pftentry = FindFileTableEntry(filetable, identifier);
      if (!pftentry)
      {
         packagestream.close();
         return nullptr;
      }

      // seek to startpos:
      packagestream.seekg(pftentry->m_StartAddress, std::ios::cur);

      // create the streamable:
      TStreamable *retval = new TStreamable;
      if (!retval->StreamLoad(packagestream))
      {
         delete retval;
         return nullptr;
      }

      packagestream.close();

      return retval;
   }
};
