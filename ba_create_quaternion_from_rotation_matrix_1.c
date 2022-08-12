#include "header.h"

double _copysign(
 double x,
 double y
)

{

 if ( y < 0 )
  return -fabs(x);
 else
  return fabs(x);

}

double _max(
 double x,
 double y
) 

{

 if ( x > y )
  return x;
 else
  return y;

}

void ba_create_quaternion_from_rotation_matrix_1(
 double R[3*3],
 double q[4]
)

/*
Get the quaternion from the rotation matrix
*/

   /*
   inline double _copysign(double x, double y)
   {
      if (y < 0)
         return -fabs(x);
      else
         return fabs(x);
   }

   template <typename Mat, typename Vec4>
   inline void
   createQuaternionFromRotationMatrix(Mat const& R, Vec4& q)
   {
      assert(R.num_rows() == 3);
      assert(R.num_cols() == 3);
      assert(q.size() == 4);

      double const m00 = R[0][0]; double const m01 = R[0][1]; double const m02 = R[0][2];
      double const m10 = R[1][0]; double const m11 = R[1][1]; double const m12 = R[1][2];
      double const m20 = R[2][0]; double const m21 = R[2][1]; double const m22 = R[2][2];

      q[3] = sqrt(std::max(0.0, 1.0 + m00 + m11 + m22)) / 2;
      q[0] = sqrt(std::max(0.0, 1.0 + m00 - m11 - m22)) / 2;
      q[1] = sqrt(std::max(0.0, 1.0 - m00 + m11 - m22)) / 2;
      q[2] = sqrt(std::max(0.0, 1.0 - m00 - m11 + m22)) / 2;

      q[0] = _copysign(q[0], m21 - m12);
      q[1] = _copysign(q[1], m02 - m20);
      q[2] = _copysign(q[2], m10 - m01);
   } // end createQuaternionFromRotationMatrix()
   */

{

 double m00;
 double m01;
 double m02;
 double m10;
 double m11;
 double m12;
 double m20;
 double m21;
 double m22;

 m00 = R[0*3+0];
 m01 = R[0*3+1];
 m02 = R[0*3+2];
 m10 = R[1*3+0];
 m11 = R[1*3+1];
 m12 = R[1*3+2];
 m20 = R[2*3+0];
 m21 = R[2*3+1];
 m22 = R[2*3+2];

 q[3] = sqrt(_max(0.0, 1.0 + m00 + m11 + m22)) / 2;
 q[0] = sqrt(_max(0.0, 1.0 + m00 - m11 - m22)) / 2;
 q[1] = sqrt(_max(0.0, 1.0 - m00 + m11 - m22)) / 2;
 q[2] = sqrt(_max(0.0, 1.0 - m00 - m11 + m22)) / 2;

 q[0] = _copysign(q[0], m21 - m12);
 q[1] = _copysign(q[1], m02 - m20);
 q[2] = _copysign(q[2], m10 - m01);

}
