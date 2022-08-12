#include "header.h"

void sfm_recompute_threedpoint_b(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int width,
 int height,
 int **all_image_arr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr,
 int threedpoint_ind
)

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 double xyz[3];
 int rgb[3];
 double old_xyz[3];
 int old_rgb[3];
 int cind;
 int new_rgb[3];
 double *P_arr;
 double *x_arr;
 double *y_arr;
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 int feat_ind;
 double x;
 double y;
 int camera_ind;
 int *image_arr;
 int i;
 int j;
 double P[3*4];
 double K[3*3];
 double R[3*3];
 double t[3];
 double val_dbl;
 int pixel;
 int camera_matrix_ind;
 int val_int;
 double X4[4];
 double new_xyz[3];
 double K_inv[3*3];
 double x3[3];
 double normalized_x3[3];
 double normalized_x;
 double normalized_y;
 double normalized_P[3*4];

 xyz[0]= threedpoint_arr[threedpoint_ind].xyz[0];
 xyz[1]= threedpoint_arr[threedpoint_ind].xyz[1];
 xyz[2]= threedpoint_arr[threedpoint_ind].xyz[2];
 rgb[0]= threedpoint_arr[threedpoint_ind].rgb[0];
 rgb[1]= threedpoint_arr[threedpoint_ind].rgb[1];
 rgb[2]= threedpoint_arr[threedpoint_ind].rgb[2];
 imagepoint_arr= threedpoint_arr[threedpoint_ind].imagepoint_arr;
 imagepoint_nbr= threedpoint_arr[threedpoint_ind].imagepoint_nbr;

 /*
 Keep track of old coordinates and color
 */

 old_xyz[0]= xyz[0];
 old_xyz[1]= xyz[1];
 old_xyz[2]= xyz[2];
 old_rgb[0]= rgb[0];
 old_rgb[1]= rgb[1];
 old_rgb[2]= rgb[2];

 /*
 Initialize new color
 */

 for ( cind= 0 ; cind< 3 ; cind++ ) {
    new_rgb[cind]= 0;
 }

 /*
 Allocate P_arr
 */

 P_arr= (double *)calloc(imagepoint_nbr*(3*4),sizeof(double));

 /*
 Allocate x_arr
 */

 x_arr= (double *)calloc(imagepoint_nbr,sizeof(double));

 /*
 Allocate y_arr
 */

 y_arr= (double *)calloc(imagepoint_nbr,sizeof(double));

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
    Get image for that camera
    */

    image_arr= all_image_arr[camera_ind];

    /*
    Get color for image point
    */

    j= (int)(x + 0.5);

    if ( j < 0 )
     j= 0;
    if ( j > width-1 )
     j= width-1;

    i= (int)(y + 0.5);

    if ( i < 0 )
     i= 0;
    if ( i > height-1 )
     i= height-1;

    pixel= i*width+j;

    for ( cind= 0 ; cind< 3 ; cind++ ) {
       rgb[cind]= image_arr[pixel*3+cind];
    }

    /*
    Add to new color
    */

    for ( cind= 0 ; cind< 3 ; cind++ ) {
       new_rgb[cind]+= rgb[cind];
    }

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
    Invert K
    */

    math_matrix3x3_inverse(
     K,
     K_inv
    );

    /*
    Get the normalized coordinates
    */

    x3[0]= x;
    x3[1]= y;
    x3[2]= 1;
    math_matrix_vector_product(
     K_inv,
     3,
     3,
     x3,
     3,
     normalized_x3
    );
    normalized_x= normalized_x3[0]/normalized_x3[2];
    normalized_y= normalized_x3[1]/normalized_x3[2];

    /*
    Compute the normalized P
    */

    math_matrix_matrix_product(
     K_inv,
     3,
     3,
     P,
     3,
     4,
     normalized_P
    );

    /*
    Add P to P_arr
    */

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 4 ; j++ ) {
          P_arr[imagepoint_ind*(3*4)+i*4+j]= normalized_P[i*4+j];
       }
    }

    /*
    Add x to x_arr
    */

    x_arr[imagepoint_ind]= normalized_x;

    /*
    Add y to y_arr
    */

    y_arr[imagepoint_ind]= normalized_y;
 }

 /*
 Compute the new color
 */

 for ( cind= 0 ; cind< 3 ; cind++ ) {
    val_dbl= (double)new_rgb[cind];
    val_dbl/= (double)imagepoint_nbr;
    val_int= (int)(val_dbl+0.5);
    if ( val_int < 0 )
     val_int= 0;
    if ( val_int > 255 )
     val_int= 255;
    new_rgb[cind]= val_int;
 }

 /*
 Compute the (new) location of the 3D point
 using linear triangulation
 */

 compute_linear_triangulation(
  P_arr,
  x_arr,
  y_arr,
  imagepoint_nbr,
  X4
 );

 /*
 Go from homegeneous 3d coords to inhomogeneous
 */

 new_xyz[0]= X4[0]/X4[3];
 new_xyz[1]= X4[1]/X4[3];
 new_xyz[2]= X4[2]/X4[3];

 /*
 Update threedpoint_arr
 */

 threedpoint_arr[threedpoint_ind].xyz[0]= new_xyz[0];
 threedpoint_arr[threedpoint_ind].xyz[1]= new_xyz[1];
 threedpoint_arr[threedpoint_ind].xyz[2]= new_xyz[2];
 threedpoint_arr[threedpoint_ind].rgb[0]= new_rgb[0];
 threedpoint_arr[threedpoint_ind].rgb[1]= new_rgb[1];
 threedpoint_arr[threedpoint_ind].rgb[2]= new_rgb[2];

 /*
 Free P_arr
 */

 if ( imagepoint_nbr > 0 )
  free(P_arr);

 /*
 Free x_arr
 */

 if ( imagepoint_nbr > 0 )
  free(x_arr);

 /*
 Free y_arr
 */

 if ( imagepoint_nbr > 0 )
  free(y_arr);

}
