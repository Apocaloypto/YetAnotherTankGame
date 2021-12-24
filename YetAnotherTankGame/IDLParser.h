#pragma once
#include <map>
#include <string>
#include <vector>


namespace IDL
{
   // *********************************************************************************************
   class CIDLDataSet final
   {
   private:
      std::map<int, std::string> m_FieldValues;

   public:
      static const std::string DEFAULT_VALUE;
      static const std::string IDL_COMMENT;
      static const char        IDL_SEPARATOR;
      static const char        IDL_SEPARATOR_ALT;
      static const std::string IDL_USING;
      static const std::string IDL_AS;

      CIDLDataSet();
      ~CIDLDataSet();

      const std::string &operator[](int col) const;

      const std::string &GetVal(int field) const;
      void               SetVal(int field, const std::string &val);
   };

   // *********************************************************************************************
   class CIDLDataSets final
   {
   private:
      std::vector<CIDLDataSet> m_DataSets;

   public:
      typedef std::vector<CIDLDataSet>::const_iterator const_iterator;

      ~CIDLDataSets();

      void Add(const CIDLDataSet &dataSet);
      void Clear();

      const_iterator begin() const;
      const_iterator end() const;
   };

   // *********************************************************************************************
   bool ParseIDL(CIDLDataSets &result, const std::string &file, const std::vector<int> &cols);

   // *********************************************************************************************
   template<class T>
   const std::string &GetVal(const CIDLDataSet &dataset, T col)
   {
      return dataset[(int)col];
   }
}
