#include "header.h"

void sfm_add_camera(
 char **image_filename_arr,
 int camera_nbr,
 sfm_camera_matrix_struct *camera_matrix_arr,
 int camera_matrix_nbr,
 int inp_camera_ind,
 int **feat2threedpoint_arr,
 int *feat2threedpoint_nbr,
 sfm_threedpoint_struct **pthreedpoint_arr,
 int *pthreedpoint_nbr
)

/*
Add a camera to the 3D reconstruction
*/

{

 sfm_threedpoint_struct *threedpoint_arr;
 int threedpoint_nbr;
 int camera_ind;
 int camera1_ind;
 int camera2_ind;
 char filename_match[80];
 match_struct *match_arr;
 int match_nbr;
 char image_filename1[80];
 char image_filename2[80];
 int err_flag1;
 int err_flag2;
 int *image1_arr;
 int *image2_arr;
 int width1;
 int width2;
 int height1;
 int height2;
 int width;
 int height;
 double P1[3*4];
 double P2[3*4];
 int camera_matrix_ind1;
 double K1[3*3];
 double R1[3*3];
 double t1[3];
 int camera_matrix_ind2;
 double K2[3*3];
 double R2[3*3];
 double t2[3];
 int camera_matrix_ind;
 double K[3*3];
 double R[3*3];
 double t[3];
 double P[3*4];

 threedpoint_arr= (*pthreedpoint_arr);
 threedpoint_nbr= (*pthreedpoint_nbr);

 for ( camera_matrix_ind= 0 ;
       camera_matrix_ind< camera_matrix_nbr ;
       camera_matrix_ind++ ) {

    /*
    Get camera matrix
    */ 

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
    Check if camera_ind is inp_camera_ind
    */

    if ( camera_ind == inp_camera_ind ) {
       if ( camera_matrix_ind != camera_matrix_nbr-1 ) {
          sfm10_error_handler("sfm_add_camera");
       }
       continue;
    }

    /*
    Sort (camera_ind,inp_camera_ind)
    in ascending order
    */

    if ( camera_ind < inp_camera_ind ) {
       camera1_ind= camera_ind;
       camera2_ind= inp_camera_ind;
    }
    if ( inp_camera_ind < camera_ind ) {
       camera1_ind= inp_camera_ind;
       camera2_ind= camera_ind;
    }

    sprintf(filename_match,"best_match_%d_%d.sfm",
     camera1_ind,camera2_ind);

    sfm_read_matches(
     filename_match,
     &match_arr,
     &match_nbr
    );

    /*
    Get filename for image 1
    */

    strcpy(image_filename1,image_filename_arr[camera1_ind]);

    /*
    Load image 1
    */

    err_flag1= load_rgb_image(
     image_filename1,
     &image1_arr,
     &width1,
     &height1
    );

    /*
    Get filename for image 2
    */

    strcpy(image_filename2,image_filename_arr[camera2_ind]);

    /*
    Load image 2
    */

    err_flag2= load_rgb_image(
     image_filename2,
     &image2_arr,
     &width2,
     &height2
    );

    /*
    Check that the 2 images have the same width and height
    */

    if ( width2 != width1 ) {
       sfm10_error_handler("sfm_reconstruct_3d_from_2cameras");
    }
    if ( height2 != height1 ) {
       sfm10_error_handler("sfm_reconstruct_3d_from_2cameras");
    }

    /*
    Set the common width and height
    */

    width= width1;
    height= height1;

    /*
    Get camera matrix for camera 1
    */

    sfm_get_camera_matrix_ind(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera1_ind,
     &camera_matrix_ind1
    );

    sfm_get_camera_matrix(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera_matrix_ind1,
     &camera1_ind,
     P1,
     K1,
     R1,
     t1
    );

    /*
    Get camera matrix for camera 2
    */

    sfm_get_camera_matrix_ind(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera2_ind,
     &camera_matrix_ind2
    );

    sfm_get_camera_matrix(
     camera_matrix_arr,
     camera_matrix_nbr,
     camera_matrix_ind2,
     &camera2_ind,
     P2,
     K2,
     R2,
     t2
    );

    /*
    Compute the 3d points
    */

    sfm_compute_threedpoints(
     width,
     height,
     camera1_ind,
     camera2_ind,
     image1_arr,
     image2_arr,
     P1,
     P2,
     match_arr,
     match_nbr,
     feat2threedpoint_arr,
     feat2threedpoint_nbr,
     &threedpoint_arr,
     &threedpoint_nbr
    );

    /*
    Free match_arr
    */

    if ( match_nbr > 0 )
     free(match_arr);

    /*
    Free image1_arr
    */

    free(image1_arr);

    /*
    Free image2_arr
    */

    free(image2_arr);
 }

 (*pthreedpoint_arr)= threedpoint_arr;
 (*pthreedpoint_nbr)= threedpoint_nbr;

}
