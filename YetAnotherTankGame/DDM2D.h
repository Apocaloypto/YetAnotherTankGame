#pragma once
#include "Typedefs.h"
#include "TemplateUsings.h"
#include "Point2D.h"


class CImage;

class CDynamicDamageModel
{
private:
   struct CModelQuadPos
   {
      Real &m_X;
      Real &m_Y;
      Real &m_U;
      Real &m_V;

      CModelQuadPos(Real &x, Real &y, Real &u, Real &v)
         : m_X(x), m_Y(y), m_U(u), m_V(v)
      {
      }
   };

   struct CModelQuad
   {
      CModelQuadPos m_UpperLeft;
      CModelQuadPos m_UpperRight;
      CModelQuadPos m_LowerLeft;
      CModelQuadPos m_LowerRight;

      CModelQuad(const CModelQuadPos &ul, const CModelQuadPos &ur, const CModelQuadPos &ll, const CModelQuadPos &lr)
         : m_UpperLeft(ul), m_UpperRight(ur), m_LowerLeft(ll), m_LowerRight(lr)
      {
      }
   };

   static constexpr int XYUV_ARRAY_X_COORD_INDEX = 0;
   static constexpr int XYUV_ARRAY_Y_COORD_INDEX = 1;
   static constexpr int XYUV_ARRAY_U_COORD_INDEX = 2;
   static constexpr int XYUV_ARRAY_V_COORD_INDEX = 3;

   static constexpr int XYUV_ARRAY_LEFT_UPPER_OFFSET = 0;
   static constexpr int XYUV_ARRAY_RIGHT_UPPER_OFFSET = 4;
   static constexpr int XYUV_ARRAY_RIGHT_LOWER_OFFSET = 8;
   static constexpr int XYUV_ARRAY_LEFT_LOWER_OFFSET = 12;

   static constexpr Real CRASH_CHECK_TRIANGLE_HEIGHT = 400;

   const CImage *m_pModel;
   const Real    m_Stability;

   Real **m_ppPhysicalModelPolyDivision;
   size_t        m_PhysicalModelPolyDivisionSizeX;
   size_t        m_PhysicalModelPolyDivisionSizeY;

   CModelQuad **m_ppLogicalModelPolyDivision;
   size_t        m_LogicalModelPolyDivisionSizeX;
   size_t        m_LogicalModelPolyDivisionSizeY;

   void DeletePhysicalModelPolyDivision();
   void DeleteLogicalModelPolyDivision();

   void CreatePhysicalModelPolyDivision();
   void CreateLogicalModelPolyDivision();

   void DeletePolyDivisions();
   void CreatePolyDivisions();

   size_t CalcPhysModelIndex(size_t x, size_t y) const;
   size_t CalcLogicalModelIndex(size_t x, size_t y) const;

public:
   CDynamicDamageModel(const CImage *pModel, Real stability);
   virtual ~CDynamicDamageModel();

   CPixelDim GetDimensions() const;
   CPixelPos GetCenter() const;

   void Draw(const CPixelPos &screen, const CPixelPos &handle, Degrees rot, const CPixelPos &origin, const CShader *pUseShader) const;

   void Crash(CPixelPos offOfCenter, KgTimesMPerS impulse, Pixels width, Degrees rot);

   void Repair();
};
