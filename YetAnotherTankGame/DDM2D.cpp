#include "DDM2D.h"

#include <cmath>
#include <algorithm>
#include "Image.h"
#include "Const.h"
#include "Functions.h"


using namespace std;

// ################################################################################################
CDynamicDamageModel::CDynamicDamageModel(const CImage *pModel, Real stability)
   : m_pModel(pModel),
     m_Stability(MathFun::Normalize(stability, 0.0f, 1.0f)),
     m_ppPhysicalModelPolyDivision(nullptr),
     m_PhysicalModelPolyDivisionSizeX(0),
     m_PhysicalModelPolyDivisionSizeY(0),
     m_ppLogicalModelPolyDivision(nullptr),
     m_LogicalModelPolyDivisionSizeX(0),
     m_LogicalModelPolyDivisionSizeY(0)
{
   CreatePolyDivisions();
}

// ************************************************************************************************
CDynamicDamageModel::~CDynamicDamageModel()
{
   DeletePolyDivisions();
}

// ************************************************************************************************
void CDynamicDamageModel::DeleteLogicalModelPolyDivision()
{
   if (!m_ppLogicalModelPolyDivision)
      return;

   for (size_t currPos = 0; currPos < (m_LogicalModelPolyDivisionSizeX * m_LogicalModelPolyDivisionSizeY); currPos++)
      delete m_ppLogicalModelPolyDivision[currPos];

   delete[] m_ppLogicalModelPolyDivision;

   m_ppLogicalModelPolyDivision    = nullptr;
   m_LogicalModelPolyDivisionSizeX = 0;
   m_LogicalModelPolyDivisionSizeY = 0;
}

// ************************************************************************************************
void CDynamicDamageModel::DeletePhysicalModelPolyDivision()
{
   if (!m_ppPhysicalModelPolyDivision)
      return;

   for (size_t currPos = 0; currPos < (m_PhysicalModelPolyDivisionSizeX * m_PhysicalModelPolyDivisionSizeY); currPos++)
      delete[] m_ppPhysicalModelPolyDivision[currPos];

   delete[] m_ppPhysicalModelPolyDivision;

   m_ppPhysicalModelPolyDivision    = nullptr;
   m_PhysicalModelPolyDivisionSizeX = 0;
   m_PhysicalModelPolyDivisionSizeY = 0;
}

// ************************************************************************************************
size_t CDynamicDamageModel::CalcPhysModelIndex(size_t x, size_t y) const
{
   return x * m_PhysicalModelPolyDivisionSizeX + y;
}

// ************************************************************************************************
size_t CDynamicDamageModel::CalcLogicalModelIndex(size_t x, size_t y) const
{
   return x * m_LogicalModelPolyDivisionSizeX + y;
}

// ************************************************************************************************
void CDynamicDamageModel::CreatePhysicalModelPolyDivision()
{
   DeletePhysicalModelPolyDivision();

   if (!m_pModel)
      return;

   m_PhysicalModelPolyDivisionSizeX = Const::DDM2D_MODEL_RESOLUTION + 1; // bei 5 quads gibt es 6 zu berücksichtigende positionen
   m_PhysicalModelPolyDivisionSizeY = Const::DDM2D_MODEL_RESOLUTION + 1; // "
   m_ppPhysicalModelPolyDivision    = new Real*[m_PhysicalModelPolyDivisionSizeX * m_PhysicalModelPolyDivisionSizeY];

   const CPixelDim modeldim = m_pModel->GetDimensions();

   const Real QUAD_WIDTH_PIXELS    = (Real)(modeldim.m_Width  / (Real)Const::DDM2D_MODEL_RESOLUTION);
   const Real QUAD_HEIGHT_PIXELS   = (Real)(modeldim.m_Height / (Real)Const::DDM2D_MODEL_RESOLUTION);

   const Real QUAD_WIDTH_TEXUNITS  = (Real)(1.0f / (Real)Const::DDM2D_MODEL_RESOLUTION);
   const Real QUAD_HEIGHT_TEXUNITS = (Real)(1.0f / (Real)Const::DDM2D_MODEL_RESOLUTION);

   for (size_t posx = 0; posx < m_PhysicalModelPolyDivisionSizeX; posx++)
   {
      for (size_t posy = 0; posy < m_PhysicalModelPolyDivisionSizeY; posy++)
      {
         const size_t arrayIndex = CalcPhysModelIndex(posx, posy);

         m_ppPhysicalModelPolyDivision[arrayIndex] = new Real[4];

         m_ppPhysicalModelPolyDivision[arrayIndex][XYUV_ARRAY_X_COORD_INDEX] = posx * QUAD_WIDTH_PIXELS;    // x
         m_ppPhysicalModelPolyDivision[arrayIndex][XYUV_ARRAY_Y_COORD_INDEX] = posy * QUAD_HEIGHT_PIXELS;   // y
         m_ppPhysicalModelPolyDivision[arrayIndex][XYUV_ARRAY_U_COORD_INDEX] = posx * QUAD_WIDTH_TEXUNITS;  // u
         m_ppPhysicalModelPolyDivision[arrayIndex][XYUV_ARRAY_V_COORD_INDEX] = posy * QUAD_HEIGHT_TEXUNITS; // v
      }
   }
}

// ************************************************************************************************
void CDynamicDamageModel::CreateLogicalModelPolyDivision()
{
   DeleteLogicalModelPolyDivision();

   if (!m_pModel)
      return;

   m_LogicalModelPolyDivisionSizeX = Const::DDM2D_MODEL_RESOLUTION;
   m_LogicalModelPolyDivisionSizeY = Const::DDM2D_MODEL_RESOLUTION;
   m_ppLogicalModelPolyDivision    = new CModelQuad*[m_LogicalModelPolyDivisionSizeX * m_LogicalModelPolyDivisionSizeY];

   for (size_t posx = 0; posx < m_LogicalModelPolyDivisionSizeX; posx++)
   {
      for (size_t posy = 0; posy < m_LogicalModelPolyDivisionSizeY; posy++)
      {
         const size_t arrayIndex   = CalcLogicalModelIndex(posx, posy);

         const size_t logicIndexUL = CalcPhysModelIndex(posx,     posy);
         const size_t logicIndexUR = CalcPhysModelIndex(posx + 1, posy);
         const size_t logicIndexLL = CalcPhysModelIndex(posx,     posy + 1);
         const size_t logicIndexLR = CalcPhysModelIndex(posx + 1, posy + 1);

         m_ppLogicalModelPolyDivision[arrayIndex] = new CModelQuad(
            CModelQuadPos(m_ppPhysicalModelPolyDivision[logicIndexUL][XYUV_ARRAY_X_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexUL][XYUV_ARRAY_Y_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexUL][XYUV_ARRAY_U_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexUL][XYUV_ARRAY_V_COORD_INDEX]),
            CModelQuadPos(m_ppPhysicalModelPolyDivision[logicIndexUR][XYUV_ARRAY_X_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexUR][XYUV_ARRAY_Y_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexUR][XYUV_ARRAY_U_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexUR][XYUV_ARRAY_V_COORD_INDEX]),
            CModelQuadPos(m_ppPhysicalModelPolyDivision[logicIndexLL][XYUV_ARRAY_X_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexLL][XYUV_ARRAY_Y_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexLL][XYUV_ARRAY_U_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexLL][XYUV_ARRAY_V_COORD_INDEX]),
            CModelQuadPos(m_ppPhysicalModelPolyDivision[logicIndexLR][XYUV_ARRAY_X_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexLR][XYUV_ARRAY_Y_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexLR][XYUV_ARRAY_U_COORD_INDEX],
                          m_ppPhysicalModelPolyDivision[logicIndexLR][XYUV_ARRAY_V_COORD_INDEX])
         );
      }
   }
}

// ************************************************************************************************
void CDynamicDamageModel::CreatePolyDivisions()
{
   CreatePhysicalModelPolyDivision();
   CreateLogicalModelPolyDivision();
}

// ************************************************************************************************
void CDynamicDamageModel::DeletePolyDivisions()
{
   DeleteLogicalModelPolyDivision();
   DeletePhysicalModelPolyDivision();
}

// ************************************************************************************************
void CDynamicDamageModel::Draw(const CPixelPos &screen, const CPixelPos &handle, Degrees rot, const CPixelPos &origin, const CShader *pUseShader) const
{
   constexpr int XYUV_ARRAY_SIZE = 16;

   if (!m_pModel)
      return;

   static Real xyuv[XYUV_ARRAY_SIZE];

   for (size_t posx = 0; posx < m_LogicalModelPolyDivisionSizeX; posx++)
   {
      for (size_t posy = 0; posy < m_LogicalModelPolyDivisionSizeY; posy++)
      {
         CModelQuad *pModelQuad = m_ppLogicalModelPolyDivision[CalcLogicalModelIndex(posx, posy)];
         if (pModelQuad)
         {
            xyuv[XYUV_ARRAY_LEFT_UPPER_OFFSET + XYUV_ARRAY_X_COORD_INDEX]  = pModelQuad->m_UpperLeft.m_X;
            xyuv[XYUV_ARRAY_LEFT_UPPER_OFFSET + XYUV_ARRAY_Y_COORD_INDEX]  = pModelQuad->m_UpperLeft.m_Y;
            xyuv[XYUV_ARRAY_LEFT_UPPER_OFFSET + XYUV_ARRAY_U_COORD_INDEX]  = pModelQuad->m_UpperLeft.m_U;
            xyuv[XYUV_ARRAY_LEFT_UPPER_OFFSET + XYUV_ARRAY_V_COORD_INDEX]  = pModelQuad->m_UpperLeft.m_V;

            xyuv[XYUV_ARRAY_RIGHT_UPPER_OFFSET + XYUV_ARRAY_X_COORD_INDEX] = pModelQuad->m_UpperRight.m_X;
            xyuv[XYUV_ARRAY_RIGHT_UPPER_OFFSET + XYUV_ARRAY_Y_COORD_INDEX] = pModelQuad->m_UpperRight.m_Y;
            xyuv[XYUV_ARRAY_RIGHT_UPPER_OFFSET + XYUV_ARRAY_U_COORD_INDEX] = pModelQuad->m_UpperRight.m_U;
            xyuv[XYUV_ARRAY_RIGHT_UPPER_OFFSET + XYUV_ARRAY_V_COORD_INDEX] = pModelQuad->m_UpperRight.m_V;

            xyuv[XYUV_ARRAY_RIGHT_LOWER_OFFSET + XYUV_ARRAY_X_COORD_INDEX] = pModelQuad->m_LowerRight.m_X;
            xyuv[XYUV_ARRAY_RIGHT_LOWER_OFFSET + XYUV_ARRAY_Y_COORD_INDEX] = pModelQuad->m_LowerRight.m_Y;
            xyuv[XYUV_ARRAY_RIGHT_LOWER_OFFSET + XYUV_ARRAY_U_COORD_INDEX] = pModelQuad->m_LowerRight.m_U;
            xyuv[XYUV_ARRAY_RIGHT_LOWER_OFFSET + XYUV_ARRAY_V_COORD_INDEX] = pModelQuad->m_LowerRight.m_V;

            xyuv[XYUV_ARRAY_LEFT_LOWER_OFFSET + XYUV_ARRAY_X_COORD_INDEX]  = pModelQuad->m_LowerLeft.m_X;
            xyuv[XYUV_ARRAY_LEFT_LOWER_OFFSET + XYUV_ARRAY_Y_COORD_INDEX]  = pModelQuad->m_LowerLeft.m_Y;
            xyuv[XYUV_ARRAY_LEFT_LOWER_OFFSET + XYUV_ARRAY_U_COORD_INDEX]  = pModelQuad->m_LowerLeft.m_U;
            xyuv[XYUV_ARRAY_LEFT_LOWER_OFFSET + XYUV_ARRAY_V_COORD_INDEX]  = pModelQuad->m_LowerLeft.m_V;

            DrawFun::DrawTexturedPolygon(*m_pModel, xyuv, XYUV_ARRAY_SIZE, screen, handle, origin, rot, pUseShader); 
         }
      }
   }
}

// ************************************************************************************************
void CDynamicDamageModel::Crash(CPixelPos offOfCenter, KgTimesMPerS impulse, Pixels width, Degrees rot)
{
   struct CCollisionInfoHelper
   {
      Real *m_pPointArray;
      Real  m_DistanceFromCenter;

      CCollisionInfoHelper(Real *pPointArray, Real distFromCenter)
         : m_pPointArray(pPointArray),
           m_DistanceFromCenter(distFromCenter)
      {
      }
   };

   if (!m_pModel || !m_ppPhysicalModelPolyDivision)
      return;

   if (impulse <= 0.0f)
      return;

   if (m_Stability >= 1.0f)
      return;

   const CPixelPos CENTER_OF_MODEL = m_pModel->GetCenter();

   offOfCenter += CENTER_OF_MODEL;

   const Real DISTANCE_OF_COLL_TO_CENTER = offOfCenter.GetDistanceTo(CENTER_OF_MODEL);
   if (DISTANCE_OF_COLL_TO_CENTER <= 0)
      return;

   const Degrees ANGLE_OF_COLLISION = MathFun::NormalizeAngle(MathFun::GetRotationBetween(CENTER_OF_MODEL, offOfCenter) - rot);

   const Real HALF_WIDTH_SCALED_TO_TRI_HEIGHT = (CRASH_CHECK_TRIANGLE_HEIGHT * (width / 2.0f)) / DISTANCE_OF_COLL_TO_CENTER;

   // Hier wird nun ein Dreieck zusammengestellt, alle x/y-Koordinaten,
   // die dort enthalten sind, sind vom Crash betroffen:
   CPixelPos leftPointOfTriangle  = CENTER_OF_MODEL + CPixelPos(-HALF_WIDTH_SCALED_TO_TRI_HEIGHT, CRASH_CHECK_TRIANGLE_HEIGHT);
   CPixelPos rightPointOfTriangle = CENTER_OF_MODEL + CPixelPos(HALF_WIDTH_SCALED_TO_TRI_HEIGHT, CRASH_CHECK_TRIANGLE_HEIGHT);

   leftPointOfTriangle  = MathFun::RotateAround(leftPointOfTriangle, CENTER_OF_MODEL, ANGLE_OF_COLLISION);
   rightPointOfTriangle = MathFun::RotateAround(rightPointOfTriangle, CENTER_OF_MODEL, ANGLE_OF_COLLISION);

   // Jetzt schauen wir, welche Punkte alles in dem collision-Triangle liegen:
   vector<CCollisionInfoHelper> pointsInTriangle;

   for (size_t posx = 0; posx < m_PhysicalModelPolyDivisionSizeX; posx++)
   {
      for (size_t posy = 0; posy < m_PhysicalModelPolyDivisionSizeY; posy++)
      {
         Real *pPointArray = m_ppPhysicalModelPolyDivision[CalcPhysModelIndex(posx, posy)];

         CPoint2D<Real> pointCoord = CPoint2D<Real>((Real)pPointArray[XYUV_ARRAY_X_COORD_INDEX], (Real)pPointArray[XYUV_ARRAY_Y_COORD_INDEX]);

         if (MathFun::IsPointInTriangle(CENTER_OF_MODEL, leftPointOfTriangle, rightPointOfTriangle, pointCoord))
         {
            Real distanceFromCenter = pointCoord.GetDistanceTo(CENTER_OF_MODEL);
            if (distanceFromCenter <= 0)
               continue;

            pointsInTriangle.push_back(CCollisionInfoHelper(pPointArray, distanceFromCenter));
         }
      }
   }

   if (pointsInTriangle.empty())
      return;

   // Nachdem wir nun alle betroffenen Punkte haben, sortieren wir diese nach der
   // Entfernung zum Mittelpunkt:
   sort(pointsInTriangle.begin(), pointsInTriangle.end(), [](const CCollisionInfoHelper &coll1, const CCollisionInfoHelper &coll2) -> bool
      {
         return coll2.m_DistanceFromCenter < coll1.m_DistanceFromCenter;
      });

   // Und jetzt passiert die Action:
   const Real DISTANCE_FAREST_POINT = pointsInTriangle[0].m_DistanceFromCenter;
   if (DISTANCE_FAREST_POINT == 0)
      return;

   const Degrees ANGLE_OF_DISPLACEMENT = MathFun::NormalizeAngle(ANGLE_OF_COLLISION + 180);

   const Real MAX_DISPLACEMENT_LENGTH = impulse * ((Real)1.0 - m_Stability) * Const::DDM2D_IMPULSE_TO_DISPLACEMENT_MULT;

   for (const CCollisionInfoHelper &collInfo : pointsInTriangle)
   {
      Real propOfImpulse      = collInfo.m_DistanceFromCenter / DISTANCE_FAREST_POINT;
      Real displacementLength = (propOfImpulse * impulse) * ((Real)1.0 - m_Stability) * Const::DDM2D_IMPULSE_TO_DISPLACEMENT_MULT;

      Real randomDisplacement = MathFun::GetRandomRealBetween(0, Const::DDM2D_RANDOM_DISPLACEMENT);

      displacementLength += randomDisplacement * (displacementLength / MAX_DISPLACEMENT_LENGTH);

      CPoint2D<Real> translated = MathFun::Move(CPoint2D<Real>(collInfo.m_pPointArray[XYUV_ARRAY_X_COORD_INDEX], collInfo.m_pPointArray[XYUV_ARRAY_Y_COORD_INDEX]), ANGLE_OF_DISPLACEMENT, displacementLength);

      collInfo.m_pPointArray[XYUV_ARRAY_X_COORD_INDEX] = translated.m_X;
      collInfo.m_pPointArray[XYUV_ARRAY_Y_COORD_INDEX] = translated.m_Y;
   }
}

// ************************************************************************************************
void CDynamicDamageModel::Repair()
{
   CreatePolyDivisions();
}

// ************************************************************************************************
CPixelDim CDynamicDamageModel::GetDimensions() const
{
   return m_pModel ? m_pModel->GetDimensions() : CPixelDim();
}

// ************************************************************************************************
CPixelPos CDynamicDamageModel::GetCenter() const
{
   return m_pModel ? m_pModel->GetCenter() : CPixelPos();
}
