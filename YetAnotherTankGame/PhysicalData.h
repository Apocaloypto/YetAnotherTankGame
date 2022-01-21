#pragma once
#include "Typedefs.h"
#include "TemplateUsings.h"


class CPhysicalData
{
public:
   const CVector2D<Meter> m_Speed;
   const KG m_Weight;

   CPhysicalData()
      : CPhysicalData(CVector2D<Meter>(0, 0), 0)
   {
   }

   CPhysicalData(const CVector2D<Meter> &speed, KG weight)
      : m_Speed(speed), m_Weight(weight)
   {
   }

   KgTimesMPerS GetImpulse(const CPhysicalData &other) const;
};
