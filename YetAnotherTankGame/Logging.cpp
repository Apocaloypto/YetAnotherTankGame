#include "Logging.h"

#include <iostream>
#include <time.h>


using namespace std;

// ################################################################################################
const CLogger GENERAL_LOG("GENERAL");

// ################################################################################################
CLogManager &Log()
{
   static CLogManager inst;
   return inst;
}

// ################################################################################################
void CLogManager::AddLog(const String &msg) const
{
   cout << msg << endl;
}

// ################################################################################################
const String CLogger::DATETIME_FORMAT = "%d.%m.%Y @ %H:%M:%S";

// ************************************************************************************************
CLogger::CLogger(const String &name)
   : m_Name(name)
{
}

// ************************************************************************************************
String CLogger::FormatCurrentDateTime() const
{
   time_t now = time(0);
   struct tm tstruct;
   char buf[80];
   localtime_s(&tstruct, &now);

   strftime(buf, sizeof(buf), DATETIME_FORMAT.c_str(), &tstruct);

   return buf;
}

// ************************************************************************************************
void CLogger::Log(LogType logtype, const string &msg) const 
{
   string fmtmsg = "[" + FormatCurrentDateTime() + "]";

   fmtmsg += "[" + m_Name + "]";

   switch (logtype)
   {
   case LogType::Message:
      fmtmsg += "[ MSG ]";
      break;
   case LogType::Warning:
      fmtmsg += "[ WNG ]";
      break;
   case LogType::Error:
      fmtmsg += "[ERROR]";
      break;
   default:
      fmtmsg += "[UNKNW]";
      break;
   }

   fmtmsg += " " + msg;

   ::Log().AddLog(fmtmsg);
}
