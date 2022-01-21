#include "PhysicalData.h"

#include "Settings.h"


// ################################################################################################
KgTimesMPerS CPhysicalData::GetImpulse() const
{
   MPerF magni = m_Speed.GetMagnitude();
   MPerS magni_real = Settings().ToPerSecondValue(magni);

   return magni_real * m_Weight;
}

// ************************************************************************************************
CPhysicalData CPhysicalData::CalcResult(const CPhysicalData &other) const
{
   return *this;
}
