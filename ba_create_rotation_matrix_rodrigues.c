#include "header.h"

void ba_create_rotation_matrix_rodrigues(
 double omega[3],
 double R[3*3]
)

/*
Recover the rotation matrix from the rotation vector
using Rodrigues formula
*/

   /*
   template <typename Vec, typename Mat>
   inline void
   createRotationMatrixRodrigues(Vec const& omega, Mat& R)
   {
      assert(omega.size() == 3);
      assert(R.num_rows() == 3);
      assert(R.num_cols() == 3);

      double const theta = norm_L2(omega);
      makeIdentityMatrix(R);
      if (fabs(theta) > 1e-6)
      {
         Matrix3x3d J, J2;
         makeCrossProductMatrix(omega, J);
         multiply_A_B(J, J, J2);
         double const c1 = sin(theta)/theta;
         double const c2 = (1-cos(theta))/(theta*theta);
         for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
               R[i][j] += c1*J[i][j] + c2*J2[i][j];
      }
   } // end createRotationMatrixRodrigues()
   */

{

 double theta2;
 double theta;
 double J[3*3];
 double J2[3*3];
 double c1;
 double c2;
 int i;
 int j;
 double tol= 1.0e-6;

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j]= 0;
    }
 }
 for ( i= 0 ; i< 3 ; i++ ) {
    j= i;
    R[i*3+j]= 1;
 }

 theta2= math_vector_vector_product(
  omega,
  3,
  omega,
  3
 );
 theta= sqrt(theta2);

 if ( theta < tol ) {
    return;
 }

 c1 = sin(theta)/theta;

 math_cross_product_matrix(
  omega,
  J
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j] += c1*J[i*3+j];
    }
 }

 c2 = (1-cos(theta))/(theta*theta);

 math_matrix_matrix_product(
  J,
  3,
  3,
  J,
  3,
  3,
  J2
 );

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       R[i*3+j] += c2*J2[i*3+j];
    }
 }

}
