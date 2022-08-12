#include "header.h"

void sfm_compute_good_matches_main(
 char **image_filename_arr,
 int camera_nbr,
 int ntrials
)

{

 int camera1_ind;
 int camera2_ind;
 char filename_match[80];
 char filename_good_match[80];
 match_struct *match_arr;
 int match_nbr;
 match_struct *good_match_arr;
 int good_match_nbr;
 FILE *fp;
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
 double F[3*3];
 char filename_F[80];

 /*
 For each pair of cameras,
 compute the good feature matches
 */

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
       sfm10_error_handler("sfm_compute_good_matches_main");
    }

    /*
    Free image1_arr
    */

    free(image1_arr);

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

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
          sfm10_error_handler("sfm_compute_good_matches_main");
       }

       /*
       Free image2_arr 
       */

       free(image2_arr);

       /*
       Check that the dimensions of the 2 images are the same
       */

       if ( width2 != width1 ) {
          sfm10_error_handler("sfm_compute_good_matches_main");
       }
       if ( height2 != height1 ) {
          sfm10_error_handler("sfm_compute_good_matches_main");
       }

       /*
       Read the matches
       */

       sprintf(filename_match,"match_%d_%d.sfm",camera1_ind,camera2_ind);

       sfm_read_matches(
        filename_match,
        &match_arr,
        &match_nbr
       );

       /*
       Let's see if the good matches
       have already been printed on disk
       */

       sprintf(filename_good_match,"good_match_%d_%d.sfm",camera1_ind,camera2_ind);

       fp= fopen(filename_good_match,"r");
       if ( fp != 0 ) {

          /*
          File is already there
          */

          fclose(fp);

          /*
          Free match_arr
          */

          if ( match_nbr > 0 )
           free(match_arr);

          continue;
       }

       fprintf(stdout,"Number of matches between images %d and %d = %d\n",
        camera1_ind,camera2_ind,match_nbr);

       /*
       Compute the good matches
       */

       sfm_compute_good_matches(
        width1,
        height1,
        match_arr,
        match_nbr,
        ntrials,
        &good_match_arr,
        &good_match_nbr,
        F
       );

       fprintf(stdout,"Number of good matches between images %d and %d = %d\n",
        camera1_ind,camera2_ind,good_match_nbr);

       /*
       Print the good matches on disk
       */

       sfm_print_matches(
        filename_good_match,
        good_match_arr,
        good_match_nbr
       );

       /*
       Print F on disk
       */

       sprintf(filename_F,"F_%d_%d.sfm",camera1_ind,camera2_ind);

       sfm_print_F(
        filename_F,
        F 
       );

       /*
       Free match_arr
       */

       if ( match_nbr > 0 )
        free(match_arr);

       /*
       Free good_match_arr
       */

       if ( good_match_nbr > 0 )
        free(good_match_arr);
    }
 }

}
