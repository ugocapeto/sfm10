#include "header.h"

void sfm_merge_duplicate_features(
 char **image_filename_arr,
 int camera_nbr
)

/*
A given camera may have duplicated features
where the (x,y) are the same but
the gradient direction/maginitude is different
*/

/*
Duplicate features are not actually removed from the feat files
They are removed from the matches stored in the good_match files
The new match files are called gooder_match
*/

{

 int camera1_ind;
 int camera2_ind;
 char filename_match[80];
 match_struct *match_arr;
 int match_nbr;
 int match_ind;
 match_struct match;
 int feat1_ind;
 double x1;
 double y1;
 int feat2_ind;
 double x2;
 double y2;
 char filename_gooder_match[80];
 FILE *fp;
 int count;
 int filename_gooder_match_count;
 char filename_feat[80];
 sfm_feat_struct *feat_arr;
 int feat_nbr;
 int **cam_feat2feat_arr;
 int *cam_feat_nbr;
 int camera_ind;
 int *feat2feat_arr;
 int feat_ind;
 double x;
 double y;
 double *descr_arr;
 int descr_nbr;
 int keep_feat_ind2;
 int feat_ind2;
  double *descr_arr2;
 int descr_nbr2;
 int keep_feat1_ind;
 int keep_feat2_ind;

 /*
 Let's see if the gooded matches
 have already been printed on disk
 */

 count= 0;
 filename_gooder_match_count= 0;

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {
    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {
       count++;

       sprintf(filename_gooder_match,"gooder_match_%d_%d.sfm",camera1_ind,camera2_ind);

       fp= fopen(filename_gooder_match,"r");
       if ( fp != 0 ) {

          /*
          File is already there
          */

          fclose(fp);
          filename_gooder_match_count++;
       }
    }
 }

 if ( filename_gooder_match_count == count ) {

    /*
    All the gooder match files are already here  
    No need to do anything
    */

    return;
 }

 /*
 For each camera,
 for any feature that's a duplicate,
 store reference to the feature that will remain
 */

 if ( camera_nbr > 0 ) {
    cam_feat2feat_arr= (int **)calloc(camera_nbr,sizeof(int *));
    cam_feat_nbr= (int *)calloc(camera_nbr,sizeof(int));
 }
 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {

    /*
    Read the features and descriptors
    */

    sprintf(filename_feat,"feat_%d.sfm",camera_ind);

    sfm_read_features_and_descriptors(
     filename_feat,
     &feat_arr,
     &feat_nbr
    );

    if ( feat_nbr > 0 )
     feat2feat_arr= (int *)calloc(feat_nbr,sizeof(int));

    cam_feat2feat_arr[camera_ind]= feat2feat_arr;
    cam_feat_nbr[camera_ind]= feat_nbr;

    for ( feat_ind= 0 ;
          feat_ind< feat_nbr ;
          feat_ind++ ) {

       x= feat_arr[feat_ind].x;
       y= feat_arr[feat_ind].y;
       descr_arr= feat_arr[feat_ind].descr_arr;
       descr_nbr= feat_arr[feat_ind].descr_nbr;

       /*
       Have the feature reference itself
       If the feature references itself,
       it is not a duplicate feature (of another feature)
       */

       feat2feat_arr[feat_ind]= feat_ind;

       /*
       Go back up feat_arr
       to find the first occurence of that location
       */

       /*
       Recall that duplicate features are always consecutive
       */

       keep_feat_ind2=-1;

       for ( feat_ind2= feat_ind-1 ;
             feat_ind2>= 0 ;
             feat_ind2-- ) {

          x2= feat_arr[feat_ind2].x;
          y2= feat_arr[feat_ind2].y;
          descr_arr2= feat_arr[feat_ind2].descr_arr;
          descr_nbr2= feat_arr[feat_ind2].descr_nbr;

          if ( x2 == x &&
               y2 == y ) {
             keep_feat_ind2= feat_ind2;
          }
          else {
             break;
          }
       }

       if ( keep_feat_ind2 == -1 ) {

          /*
          Not a duplicate feature
          */

          continue;
       }

       /*
       If here,
       feat_ind is a duplicate feature of keep_feat_ind2
       */

       feat2feat_arr[feat_ind]= keep_feat_ind2;
    }

    /*
    Free the features
    */

    if ( feat_nbr > 0 )
     free(feat_arr);
 }

 /*
 Matches that involve duplicate features must be updated
 by replacing the duplicate features with the features that remain
 */

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       /*
       Read the matches from disk
       */

       sprintf(filename_match,"good_match_%d_%d.sfm",
        camera1_ind,camera2_ind);

       sfm_read_matches(
        filename_match,
        &match_arr,
        &match_nbr
       );

       for ( match_ind= 0 ;
             match_ind< match_nbr ;
             match_ind++ ) {

          match= match_arr[match_ind];
          feat1_ind= match.feat1_ind;
          x1= match.x1;
          y1= match.y1;
          feat2_ind= match.feat2_ind;
          x2= match.x2;
          y2= match.y2;

          /*
          See if the match has been removed
          */

          if ( feat1_ind == -1 &&
               feat2_ind == -1 )
           continue;

          /*
          Let's get the feature on camera 1 that remains
          */

          keep_feat1_ind= cam_feat2feat_arr[camera1_ind][feat1_ind];

          /*
          Let's get the feature on camera 2 that remains
          */

          keep_feat2_ind= cam_feat2feat_arr[camera2_ind][feat2_ind];

          /*
          Update the match
          */

          match.feat1_ind= keep_feat1_ind;
          match.feat2_ind= keep_feat2_ind;
          match_arr[match_ind]= match;
       }

       sprintf(filename_gooder_match,"gooder_match_%d_%d.sfm",camera1_ind,camera2_ind);

       /*
       Print the gooder matches on disk
       */

       sfm_print_matches(
        filename_gooder_match,
        match_arr,
        match_nbr
       );

       /*
       Free the matches
       */

       if ( match_nbr > 0 )
        free(match_arr);
    }
 }

 /*
 Free cam_feat2feat_arr
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    if ( cam_feat_nbr[camera_ind] > 0 )
     free(cam_feat2feat_arr[camera_ind]);
 }

 if ( camera_nbr > 0 )
  free(cam_feat2feat_arr);

 /*
 Free cam_feat_nbr
 */

 if ( camera_nbr > 0 )
  free(cam_feat_nbr);

}
