#include "header.h"

void ba_create_quaternion_from_rotation_matrix_0(
 double R_[3*3],
 double q[4]
)

/*
Get the quaternion from the rotation matrix
*/

   /*
   template <typename Mat, typename Vec4>
   inline void
   createQuaternionFromRotationMatrix(Mat const& R_, Vec4& q)
   {
      assert(R_.num_rows() == 3);
      assert(R_.num_cols() == 3);
      assert(q.size() == 4);

      Mat R;
      makeTransposedMatrix(R_, R);

      const double tr = R[0][0] + R[1][1] + R[2][2];

      if (tr >= 0.0)
      {
         double s = sqrt(tr + 1);
         q[3] = s * 0.5;
         s = 0.5 / s;

         q[0] = (R[1][2] - R[2][1]) * s;
         q[1] = (R[2][0] - R[0][2]) * s;
         q[2] = (R[0][1] - R[1][0]) * s;
      }
      else
      {
         int i = 0;
         if (R[1][1] > R[0][0]) i = 1;
         if (R[2][2] > R[i][i]) i = 2;

         if (tr > R[i][i])
         {
            // Compute w first:
            q[3] = sqrt(tr + 1)/2.0;
            double const w4 = 4 * q[3];

            // And compute other values:
            q[0] = (R[1][2] - R[2][1]) / w4;
            q[1] = (R[2][0] - R[0][2]) / w4;
            q[2] = (R[0][1] - R[1][0]) / w4;
         }
         else
         {
            // Compute x, y, or z first:
            int j = (i+1) % 3;
            int k = (j+1) % 3;

            // Compute first value:
            q[i] = sqrt(R[i][i] - R[j][j] - R[k][k] + 1) / 2;

            // And the others:
            q[j] = (R[j][i] + R[i][j]) / (4*q[i]);
            q[k] = (R[k][i] + R[i][k]) / (4*q[i]);

            q[3] = (R[j][k]-R[k][j])/(4*q[i]);
         } // end if
      } // end if (tr > 0)
   } // end createQuaternionFromRotationMatrix()
   */

{

 double R[3*3];
 double tr;
 double s;
 int i;
 double w4;
 int j;
 int k;

 math_matrix_transpose(
  R_,
  3,
  3,
  R
 );

 tr = R[0*3+0] + R[1*3+1] + R[2*3+2];

 if ( tr >= 0.0 ) {
    s = sqrt(tr + 1);
    q[3] = s * 0.5;
    s = 0.5 / s;

    q[0] = (R[1*3+2] - R[2*3+1]) * s;
    q[1] = (R[2*3+0] - R[0*3+2]) * s;
    q[2] = (R[0*3+1] - R[1*3+0]) * s;
 }
 else {
    i = 0;
    if ( R[1*3+1] > R[0*3+0] ) i = 1;
    if ( R[2*3+2] > R[i*3+i] ) i = 2;

    if ( tr > R[i*3+i] ) {

       /*
       Compute w first:
       */

       q[3] = sqrt(tr + 1)/2.0;
       w4 = 4 * q[3];

       /*
       And compute other values:
       */

       q[0] = (R[1*3+2] - R[2*3+1]) / w4;
       q[1] = (R[2*3+0] - R[0*3+2]) / w4;
       q[2] = (R[0*3+1] - R[1*3+0]) / w4;
    }
    else {

       /*
       Compute x, y, or z first:
       */ 

       j = (i+1) % 3;
       k = (j+1) % 3;

       /*
       Compute first value:
       */

       q[i] = sqrt(R[i*3+i] - R[j*3+j] - R[k*3+k] + 1) / 2;

       /*
       And the others:
       */

       q[j] = (R[j*3+i] + R[i*3+j]) / (4*q[i]);
       q[k] = (R[k*3+i] + R[i*3+k]) / (4*q[i]);

       q[3] = (R[j*3+k]-R[k*3+j])/(4*q[i]);
    }
 }

}
