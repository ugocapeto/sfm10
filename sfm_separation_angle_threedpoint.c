#include "header.h"

int sfm_separation_angle_threedpoint(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind,
 double min_separation_angle
)

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double xyz[3];
 int rgb[3];
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int feat_ind;
 double x;
 double y;
 int camera_ind;
 double P[3*4];
 double K[3*3];
 double R[3*3];
 double t[3];
 int camera_matrix_ind;
 int err_flag;
 double pi= acos(-1);
 double *vec_arr;
 double Rinv[3*3];
 int i;
 double C[3];
 double vec[3];
 double norm;
 double max_angle;
 int imagepoint1_ind;
 int camera1_ind;
 int feat1_ind;
 double x1;
 double y1;
 int imagepoint2_ind;
 int camera2_ind;
 int feat2_ind;
 double x2;
 double y2;
 double Rinvt[3];
 double norm2;
 double vec1[3];
 double vec2[3];
 double dotp;
 double cosine;
 double angle_rad;
 double angle;

 xyz[0]= threedpoint_arr[threedpoint_ind].xyz[0];
 xyz[1]= threedpoint_arr[threedpoint_ind].xyz[1];
 xyz[2]= threedpoint_arr[threedpoint_ind].xyz[2];
 rgb[0]= threedpoint_arr[threedpoint_ind].rgb[0];
 rgb[1]= threedpoint_arr[threedpoint_ind].rgb[1];
 rgb[2]= threedpoint_arr[threedpoint_ind].rgb[2];
 imagepoint_arr= threedpoint_arr[threedpoint_ind].imagepoint_arr;
 imagepoint_nbr= threedpoint_arr[threedpoint_ind].imagepoint_nbr;

 err_flag= 0;

 /*
 Let's keep track of the normalized vector
 that goes from the 3D point to the camera center
 */

 vec_arr= (double *)calloc(imagepoint_nbr,3*sizeof(double));

 /*
 Loop over the image points the 3D point projects onto
 */

 for ( imagepoint_ind= 0 ;
       imagepoint_ind< imagepoint_nbr ;
       imagepoint_ind++ ) {
    imagepoint= imagepoint_arr[imagepoint_ind];
    camera_ind= imagepoint.camera_ind;
    feat_ind= imagepoint.feat_ind;
    x= imagepoint.x;
    y= imagepoint.y;

    /*
    Get camera matrix for camera
    */

    sfm_get_camera_matrix_ind(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera_ind,
     &camera_matrix_ind
    );

    sfm_get_camera_matrix(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera_matrix_ind,
     &camera_ind,
     P,
     K,
     R,
     t
    );

    /*
    Compute the camera center
    t = -R*C
    */

    math_matrix3x3_inverse(
     R,
     Rinv
    );

    math_matrix_vector_product(
     Rinv,
     3,
     3,
     t,
     3,
     Rinvt 
    );

    for ( i= 0 ; i< 3 ; i++ )
     C[i]=-Rinvt[i];

    /*
    Consider the normalized vector
    that goes from the 3D point to the camera center
    */

    for ( i= 0 ; i< 3 ; i++ )
     vec[i]= C[i]-xyz[i];

    norm2= math_vector_vector_product(
     vec,
     3,
     vec,
     3 
    );
    norm= sqrt(norm2);

    for ( i= 0 ; i< 3 ; i++ )
     vec[i]/= norm;

    /*
    Store the normalized vector
    */

    for ( i= 0 ; i< 3 ; i++ )
     vec_arr[3*imagepoint_ind+i]= vec[i];
 }

 /*
 For each pair of images,
 compute the angle between the rays
 that go from the 3D point to the camera center
 */

 /*
 We are interested in the max angle
 among all the camera pairings
 */

 max_angle=0.0;

 for ( imagepoint1_ind= 0 ;
       imagepoint1_ind< imagepoint_nbr ;
       imagepoint1_ind++ ) {
    imagepoint= imagepoint_arr[imagepoint1_ind];
    camera1_ind= imagepoint.camera_ind;
    feat1_ind= imagepoint.feat_ind;
    x1= imagepoint.x;
    y1= imagepoint.y;

    for ( i= 0 ; i< 3 ; i++ )
     vec1[i]= vec_arr[3*imagepoint1_ind+i];

    for ( imagepoint2_ind= imagepoint1_ind+1 ;
          imagepoint2_ind< imagepoint_nbr ;
          imagepoint2_ind++ ) {
       imagepoint= imagepoint_arr[imagepoint2_ind];
       camera2_ind= imagepoint.camera_ind;
       feat2_ind= imagepoint.feat_ind;
       x2= imagepoint.x;
       y2= imagepoint.y;

       for ( i= 0 ; i< 3 ; i++ )
        vec2[i]= vec_arr[3*imagepoint2_ind+i];

       dotp= math_vector_vector_product(
        vec1,
        3,
        vec2,
        3
       );

       cosine= dotp;

       angle_rad= acos(cosine);

       angle= angle_rad*180/pi;

       if ( angle > max_angle ) {
          max_angle= angle;
       }
    }
 }

 /*
 Check the max angle
 */

 if ( max_angle < min_separation_angle ) {
    err_flag= 1;
 }

 /*
 Free vec_arr
 */

 if ( imagepoint_nbr > 0 )
  free(vec_arr);

 return err_flag;

}
