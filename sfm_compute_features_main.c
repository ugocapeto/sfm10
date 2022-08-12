#include "header.h"

void sfm_compute_features_main(
 char **image_filename_arr,
 int camera_nbr
)

/*
Note that the feature data structure in er9b
is different from the feature data structure in sfm
*/

{

 char image_filename[80];
 feat_struct *feat_arr;
 sfm_feat_struct *sfm_feat_arr;
 int feat_nbr;
 int camera_ind;
 double sigma;
 int err_flag;
 char filename[80];
 FILE *fp;

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    strcpy(image_filename,image_filename_arr[camera_ind]);

    /*
    Let's see if the features and the descriptors
    have already been printed on disk
    */

    sprintf(filename,"feat_%d.sfm",camera_ind);

    fp= fopen(filename,"r");
    if ( fp != 0 ) {

       /*
       File is already there
       */

       fclose(fp);
       continue;
    }

    /*
    Get the features in the image
    */

    sigma= 1.6;
    err_flag= feature_main(
     image_filename,
     sigma,
     &feat_arr,
     &feat_nbr
    );

    fprintf(stdout,"Number of features for image %d = %d\n",
     camera_ind,feat_nbr);

    /*
    Convert from feat_struct to sfm_feat_struct
    */

    convert_features_and_descriptors_to_sfm(
     feat_arr,
     feat_nbr,
     &sfm_feat_arr
    );

    /*
    Free feat_arr
    */

    free_features_and_descriptors(
     feat_arr,
     feat_nbr
    );

    /*
    Print the features and the descriptors on disk
    */

    sfm_print_features_and_descriptors(
     filename,
     sfm_feat_arr,
     feat_nbr
    );

    /*
    Free sfm_feat_arr
    */

    sfm_free_features_and_descriptors(
     sfm_feat_arr,
     feat_nbr
    );
 }

}
