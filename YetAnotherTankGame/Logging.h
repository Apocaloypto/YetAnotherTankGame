#pragma once
#include "Typedefs.h"


// ************************************************************************************************
class CLogger;

// ************************************************************************************************
enum class LogType
{
   Message,
   Warning,
   Error,
};

// ************************************************************************************************
class CLogManager
{
   friend CLogManager &Log();
   friend CLogger;

private:
   void AddLog(const String &msg) const;
};

// ************************************************************************************************
class CLogger
{
private:
   static const String DATETIME_FORMAT;

   const String m_Name;

   String FormatCurrentDateTime() const;

public:
   CLogger(const String &name);

   void Log(LogType logtype, const String &msg) const;
};

// ************************************************************************************************
extern const CLogger GENERAL_LOG;
