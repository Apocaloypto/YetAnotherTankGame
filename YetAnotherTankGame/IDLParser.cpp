#include "IDLParser.h"

#include "Functions.h"
#include "Logging.h"
#include "Typedefs.h"

#include <fstream>
#include <algorithm>


using namespace std;

namespace IDL
{
   // ################################################################################################
   const string CIDLDataSet::DEFAULT_VALUE = "";
   const string CIDLDataSet::IDL_COMMENT = "#";
   const char   CIDLDataSet::IDL_SEPARATOR = ' ';
   const char   CIDLDataSet::IDL_SEPARATOR_ALT = '\t';
   const string CIDLDataSet::IDL_USING = "using";
   const string CIDLDataSet::IDL_AS = "as";

   // ################################################################################################
   CIDLDataSet::CIDLDataSet()
   {
   }

   // ************************************************************************************************
   CIDLDataSet::~CIDLDataSet()
   {
   }

   // ************************************************************************************************
   const std::string &CIDLDataSet::operator[](int col) const
   {
      if (m_FieldValues.count(col) > 0)
         return m_FieldValues.at(col);
      else
         return DEFAULT_VALUE;
   }

   // ************************************************************************************************
   const string &CIDLDataSet::GetVal(int field) const
   {
      return operator[](field);
   }

   // ************************************************************************************************
   void CIDLDataSet::SetVal(int field, const string &val)
   {
      m_FieldValues.insert_or_assign(field, val);
   }

   // ################################################################################################
   CIDLDataSets::~CIDLDataSets()
   {
   }

   // ************************************************************************************************
   void CIDLDataSets::Add(const CIDLDataSet &dataSet)
   {
      m_DataSets.push_back(dataSet);
   }

   // ************************************************************************************************
   void CIDLDataSets::Clear()
   {
      m_DataSets.clear();
   }

   // ************************************************************************************************
   CIDLDataSets::const_iterator CIDLDataSets::begin() const
   {
      return m_DataSets.begin();
   }

   // ************************************************************************************************
   CIDLDataSets::const_iterator CIDLDataSets::end() const
   {
      return m_DataSets.end();
   }

   // ################################################################################################
   bool ParseIDL(CIDLDataSets &result, const string &file, const vector<int> &cols)
   {
      CLogger logger("IDLParser");

      result.Clear();

      ifstream stream(file);
      if (!stream.is_open())
      {
         logger.Log(LogType::Warning, "Unable to open file '" + file + "'");
         return false;
      }

      string              line;
      map<string, string> vars;

      while (getline(stream, line))
      {
         StringFun::Trim(line);

         if (line.empty())
            continue;

         if (StringFun::StartsWith(line, CIDLDataSet::IDL_COMMENT))
            continue;

         replace(line.begin(), line.end(), CIDLDataSet::IDL_SEPARATOR_ALT, CIDLDataSet::IDL_SEPARATOR);

         vector<string> splitted = StringFun::Split(line, CIDLDataSet::IDL_SEPARATOR);
         if (StringFun::StartsWith(line, CIDLDataSet::IDL_USING))
         {
            // Variablendefinition:
            if (splitted.size() >= 4 && splitted[2] == CIDLDataSet::IDL_AS)
            {
               string varname = splitted[1];
               StringFun::Trim(varname);
               string varval = splitted[3];
               StringFun::Trim(varval);

               vars.insert(pair<string, string>(varname, varval));
            }
            else
               logger.Log(LogType::Warning, "Error parsing vardef, syntax: using $varname as varvalue; found: '" + line + "'");
         }
         else
         {
            if (splitted.size() >= cols.size())
            {
               CIDLDataSet add;

               for (size_t colidx = 0; colidx < cols.size(); colidx++)
               {
                  for (const auto &[from, to] : vars)
                     StringFun::ReplaceAll(splitted[colidx], from, to);

                  add.SetVal(cols[colidx], splitted[colidx]);
               }

               result.Add(add);
            }
            else
               logger.Log(LogType::Warning, "Unable to parse line '" + line + "'; columns needed: " + to_string(cols.size()) + ", columns found: " + to_string(splitted.size()));
         }
      }

      stream.close();

      return true;
   }
}
