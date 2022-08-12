#include "header.h"

void sfm_compute_matches_main(
 char **image_filename_arr,
 int camera_nbr
)

{

 int camera1_ind;
 int camera2_ind;
 sfm_feat_struct *feat1_arr;
 int feat1_nbr;
 sfm_feat_struct *feat2_arr;
 int feat2_nbr;
 char filename_feat1[80];
 char filename_feat2[80];
 match_struct *match_arr;
 int match_nbr;
 char filename_match[80];
 FILE *fp;

 /*
 For each pair of cameras,
 compute the feature matches based on nearest-neighbor
 */

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    /*
    Read the features and descriptors
    */

    sprintf(filename_feat1,"feat_%d.sfm",camera1_ind);

    sfm_read_features_and_descriptors(
     filename_feat1,
     &feat1_arr,
     &feat1_nbr
    );

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       /*
       Read the features and descriptors
       */

       sprintf(filename_feat2,"feat_%d.sfm",camera2_ind);

       sfm_read_features_and_descriptors(
        filename_feat2,
        &feat2_arr,
        &feat2_nbr
       );

       /*
       Let's see if the matches
       have already been printed on disk
       */

       sprintf(filename_match,"match_%d_%d.sfm",camera1_ind,camera2_ind);

       fp= fopen(filename_match,"r");
       if ( fp != 0 ) {

          /*
          File is already there
          */

          fclose(fp);

          /*
          Free feat2_arr
          */

          sfm_free_features_and_descriptors(
           feat2_arr,
           feat2_nbr
          );

          continue;
       }

       /*
       Compute the matches
       */

       sfm_compute_matches(
        feat1_arr,
        feat1_nbr,
        feat2_arr,
        feat2_nbr,
        &match_arr,
        &match_nbr
       );

       fprintf(stdout,"Number of matches between images %d and %d = %d\n",
        camera1_ind,camera2_ind,match_nbr);

       /*
       Print the matches on disk
       */

       sfm_print_matches(
        filename_match,
        match_arr,
        match_nbr
       );

       /*
       Free feat2_arr
       */

       sfm_free_features_and_descriptors(
        feat2_arr,
        feat2_nbr
       );

       /*
       Free match_arr
       */

       if ( match_nbr > 0 )
        free(match_arr); 
    }

    /*
    Free feat1_arr
    */

    sfm_free_features_and_descriptors(
     feat1_arr,
     feat1_nbr
    );
 }

}
