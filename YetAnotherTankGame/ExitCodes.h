#pragma once


enum class ExitCode
{
   Success,

   FailedToInitializeGame = -1,
};

#define EXIT(code) (int)code
