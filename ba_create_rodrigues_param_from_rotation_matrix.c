#include "header.h"

void ba_create_rodrigues_param_from_rotation_matrix(
 double R[3*3],
 double omega[3]
)

/*
Get the rotation vector from the rotation matrix
using Rodrigues formula
*/

   /*
   template <typename Vec, typename Mat>
   inline void
   createRodriguesParamFromRotationMatrix(Mat const& R, Vec& omega)
   {
      assert(omega.size() == 3);
      assert(R.num_rows() == 3);
      assert(R.num_cols() == 3);

      Vector4d q;
      createQuaternionFromRotationMatrix(R, q);
      omega[0] = q[0];
      omega[1] = q[1];
      omega[2] = q[2];
      normalizeVector(omega);
      scaleVectorIP(2.0*acos(q[3]), omega);
   } // end createRodriguesParamFromRotationMatrix()
   */

{

 double q[4];
 double norm2;
 double norm;
 int i;

 /*
 Get the rotation in quaternion form
 Convention used:
 q[0]=qx
 q[1]=qy
 q[2]=qz
 q[3]=qw
 */

 ba_create_quaternion_from_rotation_matrix(
  R,
  q
 );

 omega[0]= q[0];
 omega[1]= q[1];
 omega[2]= q[2];

 /*
 Normalize omega
 */

 norm2= math_vector_vector_product(
  omega,
  3,
  omega,
  3
 );
 norm= sqrt(norm2);

 if ( norm > 0 ) {
    for ( i= 0 ; i< 3 ; i++ )
     omega[i]/= norm;
 }

 /*
 Scale omega
 */

 for ( i= 0 ; i< 3 ; i++ )
  omega[i]*= 2.0*acos(q[3]);

}
