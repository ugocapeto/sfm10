#include "header.h"

void sfm_compute_focal_length_main(
 char **image_filename_arr,
 int camera_nbr,
 double *pfocal_length
)

{

 int camera1_ind;
 char image_filename1[80];
 int *image1_arr;
 int width1;
 int height1;
 int err_flag1;
 int camera2_ind;
 char image_filename2[80];
 int *image2_arr;
 int width2;
 int height2;
 int err_flag2;
 char filename_F[80];
 double F[3*3];
 double focal_length;
 double avg_focal_length;
 int count;

 avg_focal_length= 0.0;
 count= 0;

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    /*
    Get corresponding image file name
    */

    strcpy(image_filename1,image_filename_arr[camera1_ind]);

    /*
    Load the corresponding image to get width and height
    */

    err_flag1= load_rgb_image(
     image_filename1,
     &image1_arr,
     &width1,
     &height1
    );

    if ( err_flag1 == 1 ) {
       sfm10_error_handler("sfm_get_initial_camera_pair");
    }

    /*
    Free image1_arr
    */

    free(image1_arr);

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       fprintf(stdout,"Processing camera %d and %d\n",
        camera1_ind,camera2_ind);

       /*
       Get corresponding image file name
       */

       strcpy(image_filename2,image_filename_arr[camera2_ind]);

       /*
       Load the corresponding image to get width and height
       */

       err_flag2= load_rgb_image(
        image_filename2,
        &image2_arr,
        &width2,
        &height2
       );

       if ( err_flag2 == 1 ) {
          sfm10_error_handler("sfm_get_initial_camera_pair");
       }

       /*
       Free image2_arr
       */

       free(image2_arr);

       /*
       Check that the dimensions of the 2 images are the same
       */

       if ( width2 != width1 ) {
          sfm10_error_handler("sfm_get_initial_camera_pair");
       }
       if ( height2 != height1 ) {
          sfm10_error_handler("sfm_get_initial_camera_pair");
       }

       /*
       Read the fundamental matrix F
       */

       sprintf(filename_F,"F_%d_%d.sfm",
        camera1_ind,camera2_ind);

       sfm_read_F(
        filename_F,
        F
       );

       /*
       Compute the focal length for this image pair
       */

       sfm_compute_focal_length(
        F,
        width1,
        height1,
        &focal_length
       );

       if ( focal_length == 1.0e32 ) {
          fprintf(stdout,"focal_length could not be determined for camera pair\n");
       }
       else {
          fprintf(stdout,"focal_length = %g\n",focal_length);
          avg_focal_length+= focal_length; 
          count++;
       }
    }
 }

 if ( count == 0 ) {
    fprintf(stdout,"focal_length could not be determined for any camera pair\n");
 }
 else {
    avg_focal_length/= (double)count; 
    fprintf(stdout,"average focal_length = %g\n",avg_focal_length);
 }

}
