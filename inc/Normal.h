#ifndef NORMAL_H
#define NORMAL_H

/*
Description:

   The Normal class is used to represent normals in 3 dimensions.
   It provides useful functions to perform operations with normals.

   Note that normals are automatically normalized during construction and after any operations are performed on them.

Possible improvements:

   - Multiplying a normal with a scalar returns a vector. This behaviour is unusual, but it was implemented
     this way because returning a normal would remove the increase in magnitude through the automatic normalization.
*/

class Normal
{
public:

   Normal();
   Normal(float x, float y, float z);
   Normal(const Vector& vec);
   ~Normal();

   const Normal normalize() const;

   const Normal operator+(const Normal& rhsNormal) const;
   const Normal operator-(const Normal& rhsNormal) const;

   float x, y, z;
};

const Vector operator*(float scalar, const Normal& rhsNormal);

#endif

