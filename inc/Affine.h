#ifndef AFFINE_H
#define AFFINE_H

/*
Description:

   The Affine class is used to represent affine transformations. These can be translation, rotation and scale transformations.

   The enum below is used to specify the type of a transformation when creating it.
   Knowing the type of each transformation involved in an operation allows one to reduce the number of operations significantly.

Possible improvements:

   - The Affine class currently encompasses 3 transformation types. This class is well suited to be implemented with an inheritance hierarchy.

   - Matrices are currently stored as 4x4 arrays of floats. Since the last row of an affine transformation is always [0 0 0 1], they could be stored as 3x4 arrays of floats.

   - Inverting mixed matrices is not supported yet. It is not necessary, but it could be useful in the future.
*/

enum TransformationType
{
   TRANSLATION,
   ROTATION,
   SCALE,
   MIXED
};

class Point;
class Vector;
class Normal;

class Affine
{
public:

   // General constructor
   Affine(const float mat[4][4], TransformationType transfType);

   // Constructor for translations and scales
   Affine(float x, float y, float z, TransformationType transfType);

   // Constructor for rotations
   Affine(float x, float y, float z, float degrees);

   ~Affine();

   const Affine invert() const;

   const Point operator*(const Point& rhsPoint) const;
   const Vector operator*(const Vector& rhsVector) const;
   const Normal operator*(const Normal& rhsNormal) const;
   const Affine operator*(const Affine& rhsAffine) const;

private:

   float matrix[4][4];
   TransformationType transfType;
};

#endif

