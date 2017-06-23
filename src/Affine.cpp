#include <algorithm>

#include "Constants.h"
#include "Point.h"
#include "Vector.h"
#include "Normal.h"
#include "Affine.h"

// General constructor
Affine::Affine(const float mat[4][4], TransformationType transfType)
   : transfType(transfType)
{
   std::copy(&mat[0][0], &mat[0][0] + 16, &matrix[0][0]);
}

// Constructor for translations and scales
// It is incorrect to use this constructor to create a rotation or mixed affine transformation
// For this reason, a zero matrix is created when (transfType == ROTATION || transfType == MIXED)
Affine::Affine(float x, float y, float z, TransformationType transfType)
   : transfType(transfType)
{
   memset(matrix, 0, 4 * 4 * sizeof(matrix[0][0]));

   if (transfType == TRANSLATION)
   {
      // Diagonal
      matrix[0][0] = 1;
      matrix[1][1] = 1;
      matrix[2][2] = 1;
      matrix[3][3] = 1;

      // Last column
      matrix[0][3] = x;
      matrix[1][3] = y;
      matrix[2][3] = z;
   }
   else if (transfType == SCALE)
   {
      // Diagonal
      matrix[0][0] = x;
      matrix[1][1] = y;
      matrix[2][2] = z;
      matrix[3][3] = 1;
   }
}

// Constructor for rotations
Affine::Affine(float x, float y, float z, float degrees)
   : transfType(ROTATION)
{
   memset(matrix, 0, 4 * 4 * sizeof(matrix[0][0]));

   // Normalize the axis of rotation unless it is the zero vector
   if (!((x == 0) && (y == 0) && (z == 0)))
   {
      float length = static_cast<float>(sqrt((x * x) + (y * y) + (z * z)));
      x = x / length;
      y = y / length;
      z = z / length;
   }

   float axisTimesAxisTranspose[3][3] = {{x * x, x * y, x * z},
                                         {x * y, y * y, y * z},
                                         {x * z, y * z, z * z}};

   float axisDualMatrix[3][3] = {{ 0, -z,  y},
                                 { z,  0, -x},
                                 {-y,  x,  0}};

   float rotationInRadians = (degrees * Constants::pi) / 180;

   for (size_t row = 0; row < 3; row++)
   {
      for (size_t col = 0; col < 3; col++)
      {
         matrix[row][col] = ((1 - cos(rotationInRadians)) * axisTimesAxisTranspose[row][col]) + (sin(rotationInRadians) * axisDualMatrix[row][col]);
      }
      matrix[row][row] = matrix[row][row] + cos(rotationInRadians);
   }

   matrix[3][3] = 1;
}

Affine::~Affine()
{ }

// Inverting a mixed affine transformation is not supported, so we do not make any modifications when (transfType == MIXED)
const Affine Affine::invert() const
{
   if (transfType == TRANSLATION)
   {
      return Affine(-matrix[0][3], -matrix[1][3], -matrix[2][3], TRANSLATION);
   }
   else if (transfType == SCALE)
   {
      return Affine(1/matrix[0][0], 1/matrix[1][1], 1/matrix[2][2], SCALE);
   }
   else if (transfType == ROTATION)
   {
      // Since it is orthogonal, the inverse of a rotation affine transformation is its transpose
      float result[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,1}};

      for (size_t row = 0; row < 3; row++)
      {
         for (size_t col = 0; col < 3; col++)
         {
            result[row][col] = matrix[col][row];
         }
      }

      return Affine(result, ROTATION);
   }
   else // Mixed
   {
      // Inverting a mixed affine transformation is not supported
      return *this;
   }
}

const Point Affine::operator*(const Point& rhsPoint) const
{
   if (transfType == TRANSLATION)
   {
      // Only worry about the last column of the matrix
      return Point(matrix[0][3] + rhsPoint.x, matrix[1][3] + rhsPoint.y, matrix[2][3] + rhsPoint.z);
   }
   else if (transfType == SCALE)
   {
      // Only worry about the diagonal of the matrix
      return Point(matrix[0][0] * rhsPoint.x, matrix[1][1] * rhsPoint.y, matrix[2][2] * rhsPoint.z);
   }
   else if (transfType == ROTATION)
   {
      Point result;

      // Only worry about upper-left 3x3
      result.x = (matrix[0][0] * rhsPoint.x) + (matrix[0][1] * rhsPoint.y) + (matrix[0][2] * rhsPoint.z);
      result.y = (matrix[1][0] * rhsPoint.x) + (matrix[1][1] * rhsPoint.y) + (matrix[1][2] * rhsPoint.z);
      result.z = (matrix[2][0] * rhsPoint.x) + (matrix[2][1] * rhsPoint.y) + (matrix[2][2] * rhsPoint.z);

      return result;
   }
   else // Mixed
   {
      Point result;

      // Points are affected by translations, so we take into account the last column
      result.x = (matrix[0][0] * rhsPoint.x) + (matrix[0][1] * rhsPoint.y) + (matrix[0][2] * rhsPoint.z) + matrix[0][3];
      result.y = (matrix[1][0] * rhsPoint.x) + (matrix[1][1] * rhsPoint.y) + (matrix[1][2] * rhsPoint.z) + matrix[1][3];
      result.z = (matrix[2][0] * rhsPoint.x) + (matrix[2][1] * rhsPoint.y) + (matrix[2][2] * rhsPoint.z) + matrix[2][3];

      return result;
   }
}

// Vectors are not affected by translations, so we do not make any modifications when (transfType == TRANSLATION)
const Vector Affine::operator*(const Vector& rhsVector) const
{
   if (transfType == TRANSLATION)
   {
      // Vectors are not affected by translations
      return rhsVector;
   }
   else if (transfType == SCALE)
   {
      // Only worry about the diagonal of the matrix
      return Vector(matrix[0][0] * rhsVector.x, matrix[1][1] * rhsVector.y, matrix[2][2] * rhsVector.z);
   }
   else // Rotation or mixed
   {
      Vector result;

      // Only worry about upper-left 3x3
      result.x = (matrix[0][0] * rhsVector.x) + (matrix[0][1] * rhsVector.y) + (matrix[0][2] * rhsVector.z);
      result.y = (matrix[1][0] * rhsVector.x) + (matrix[1][1] * rhsVector.y) + (matrix[1][2] * rhsVector.z);
      result.z = (matrix[2][0] * rhsVector.x) + (matrix[2][1] * rhsVector.y) + (matrix[2][2] * rhsVector.z);

      return result;
   }
}

// A normal is transformed by (M^-1)^T
// The lhs affine transformation must have been inverted before calling this function
// The transpose is performed by the function before transforming the normal
// Note that normals are not affected by translations, so we do not make any modifications when (transfType == TRANSLATION)
const Normal Affine::operator*(const Normal& rhsNormal) const
{
   if (transfType == TRANSLATION)
   {
      // Normals are not affected by translations
      return rhsNormal;
   }
   else if (transfType == SCALE)
   {
      // No need to transpose since scale matrices are symmetric
      return Normal(matrix[0][0] * rhsNormal.x, matrix[1][1] * rhsNormal.y, matrix[2][2] * rhsNormal.z).normalize();
   }
   else // Rotation or mixed
   {
      Normal result;

      // In the case of a rotation affine transformation, the inverse and the transpose cancel out since rotation matrices are orthogonal
      // In the case of a mixed affine transformation, we only consider the upper-left 3x3 matrix since translations do not affect normals
      result.x = (matrix[0][0] * rhsNormal.x) + (matrix[1][0] * rhsNormal.y) + (matrix[2][0] * rhsNormal.z);
      result.y = (matrix[0][1] * rhsNormal.x) + (matrix[1][1] * rhsNormal.y) + (matrix[2][1] * rhsNormal.z);
      result.z = (matrix[0][2] * rhsNormal.x) + (matrix[1][2] * rhsNormal.y) + (matrix[2][2] * rhsNormal.z);

      return result.normalize();
   }
}

// All of the cases below assume that the lhs affine transformation is a mixed affine transformation
// A higher level of optimization could be achieved if we discarded this assumption and included additional cases,
// but it is better to implement an inheritance hierarchy before moving in that direction
const Affine Affine::operator*(const Affine& rhsAffine) const
{
   if (rhsAffine.transfType == TRANSLATION)
   {
      float result[4][4] = {{matrix[0][0], matrix[0][1], matrix[0][2], 0},
                            {matrix[1][0], matrix[1][1], matrix[1][2], 0},
                            {matrix[2][0], matrix[2][1], matrix[2][2], 0},
                            {           0,            0,            0, 1}};

      // 9 additions and 9 multiplications

      result[0][3] = (matrix[0][0] * rhsAffine.matrix[0][3]) + (matrix[0][1] * rhsAffine.matrix[1][3]) + (matrix[0][2] * rhsAffine.matrix[2][3]) + matrix[0][3];
      result[1][3] = (matrix[1][0] * rhsAffine.matrix[0][3]) + (matrix[1][1] * rhsAffine.matrix[1][3]) + (matrix[1][2] * rhsAffine.matrix[2][3]) + matrix[1][3];
      result[2][3] = (matrix[2][0] * rhsAffine.matrix[0][3]) + (matrix[2][1] * rhsAffine.matrix[1][3]) + (matrix[2][2] * rhsAffine.matrix[2][3]) + matrix[2][3];

      return Affine(result, MIXED);
   }
   else if (rhsAffine.transfType == SCALE)
   {
      // 9 multiplications

      float result[4][4] = {{matrix[0][0] * rhsAffine.matrix[0][0], matrix[0][1] * rhsAffine.matrix[1][1], matrix[0][2] * rhsAffine.matrix[2][2], matrix[0][3]},
                            {matrix[1][0] * rhsAffine.matrix[0][0], matrix[1][1] * rhsAffine.matrix[1][1], matrix[1][2] * rhsAffine.matrix[2][2], matrix[1][3]},
                            {matrix[2][0] * rhsAffine.matrix[0][0], matrix[2][1] * rhsAffine.matrix[1][1], matrix[2][2] * rhsAffine.matrix[2][2], matrix[2][3]},
                            {                                    0,                                     0,                                     0,            1}};

      return Affine(result, MIXED);
   }
   else if (rhsAffine.transfType == ROTATION)
   {
      float result[4][4] = {{0, 0, 0, matrix[0][3]},
                            {0, 0, 0, matrix[1][3]},
                            {0, 0, 0, matrix[2][3]},
                            {0, 0, 0,            1}};

      // 18 additions and 27 multiplications

      // First row
      result[0][0] = (matrix[0][0] * rhsAffine.matrix[0][0]) + (matrix[0][1] * rhsAffine.matrix[1][0]) + (matrix[0][2] * rhsAffine.matrix[2][0]);
      result[0][1] = (matrix[0][0] * rhsAffine.matrix[0][1]) + (matrix[0][1] * rhsAffine.matrix[1][1]) + (matrix[0][2] * rhsAffine.matrix[2][1]);
      result[0][2] = (matrix[0][0] * rhsAffine.matrix[0][2]) + (matrix[0][1] * rhsAffine.matrix[1][2]) + (matrix[0][2] * rhsAffine.matrix[2][2]);

      // Second row
      result[1][0] = (matrix[1][0] * rhsAffine.matrix[0][0]) + (matrix[1][1] * rhsAffine.matrix[1][0]) + (matrix[1][2] * rhsAffine.matrix[2][0]);
      result[1][1] = (matrix[1][0] * rhsAffine.matrix[0][1]) + (matrix[1][1] * rhsAffine.matrix[1][1]) + (matrix[1][2] * rhsAffine.matrix[2][1]);
      result[1][2] = (matrix[1][0] * rhsAffine.matrix[0][2]) + (matrix[1][1] * rhsAffine.matrix[1][2]) + (matrix[1][2] * rhsAffine.matrix[2][2]);

      // Third row
      result[2][0] = (matrix[2][0] * rhsAffine.matrix[0][0]) + (matrix[2][1] * rhsAffine.matrix[1][0]) + (matrix[2][2] * rhsAffine.matrix[2][0]);
      result[2][1] = (matrix[2][0] * rhsAffine.matrix[0][1]) + (matrix[2][1] * rhsAffine.matrix[1][1]) + (matrix[2][2] * rhsAffine.matrix[2][1]);
      result[2][2] = (matrix[2][0] * rhsAffine.matrix[0][2]) + (matrix[2][1] * rhsAffine.matrix[1][2]) + (matrix[2][2] * rhsAffine.matrix[2][2]);

      return Affine(result, MIXED);
   }
   else // Mixed
   {
      float result[4][4] = {{0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 1}};

      // 27 additions and 36 multiplications

      // First row
      result[0][0] = (matrix[0][0] * rhsAffine.matrix[0][0]) + (matrix[0][1] * rhsAffine.matrix[1][0]) + (matrix[0][2] * rhsAffine.matrix[2][0]);
      result[0][1] = (matrix[0][0] * rhsAffine.matrix[0][1]) + (matrix[0][1] * rhsAffine.matrix[1][1]) + (matrix[0][2] * rhsAffine.matrix[2][1]);
      result[0][2] = (matrix[0][0] * rhsAffine.matrix[0][2]) + (matrix[0][1] * rhsAffine.matrix[1][2]) + (matrix[0][2] * rhsAffine.matrix[2][2]);
      result[0][3] = (matrix[0][0] * rhsAffine.matrix[0][3]) + (matrix[0][1] * rhsAffine.matrix[1][3]) + (matrix[0][2] * rhsAffine.matrix[2][3]) + matrix[0][3];

      // Second row
      result[1][0] = (matrix[1][0] * rhsAffine.matrix[0][0]) + (matrix[1][1] * rhsAffine.matrix[1][0]) + (matrix[1][2] * rhsAffine.matrix[2][0]);
      result[1][1] = (matrix[1][0] * rhsAffine.matrix[0][1]) + (matrix[1][1] * rhsAffine.matrix[1][1]) + (matrix[1][2] * rhsAffine.matrix[2][1]);
      result[1][2] = (matrix[1][0] * rhsAffine.matrix[0][2]) + (matrix[1][1] * rhsAffine.matrix[1][2]) + (matrix[1][2] * rhsAffine.matrix[2][2]);
      result[1][3] = (matrix[1][0] * rhsAffine.matrix[0][3]) + (matrix[1][1] * rhsAffine.matrix[1][3]) + (matrix[1][2] * rhsAffine.matrix[2][3]) + matrix[1][3];

      // Third row
      result[2][0] = (matrix[2][0] * rhsAffine.matrix[0][0]) + (matrix[2][1] * rhsAffine.matrix[1][0]) + (matrix[2][2] * rhsAffine.matrix[2][0]);
      result[2][1] = (matrix[2][0] * rhsAffine.matrix[0][1]) + (matrix[2][1] * rhsAffine.matrix[1][1]) + (matrix[2][2] * rhsAffine.matrix[2][1]);
      result[2][2] = (matrix[2][0] * rhsAffine.matrix[0][2]) + (matrix[2][1] * rhsAffine.matrix[1][2]) + (matrix[2][2] * rhsAffine.matrix[2][2]);
      result[2][3] = (matrix[2][0] * rhsAffine.matrix[0][3]) + (matrix[2][1] * rhsAffine.matrix[1][3]) + (matrix[2][2] * rhsAffine.matrix[2][3]) + matrix[2][3];

      return Affine(result, MIXED);
   }
}

