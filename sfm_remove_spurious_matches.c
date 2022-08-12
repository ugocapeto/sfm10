#include "header.h"

void sfm_remove_spurious_matches(
 char **image_filename_arr,
 int camera_nbr
)

{

 match_struct ***cam_match_arr;
 int **cam_match_nbr;
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
 sfm_temp_struct *temp_arr;
 int temp_nbr;
 char filename_best_match[80];
 int err_flag;
 FILE *fp;
 int count;
 int filename_best_match_count;

 /*
 Let's see if the best matches
 have already been printed on disk
 */

 count= 0;
 filename_best_match_count= 0;

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {
    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {
       count++;

       sprintf(filename_best_match,"best_match_%d_%d.sfm",camera1_ind,camera2_ind);

       fp= fopen(filename_best_match,"r");
       if ( fp != 0 ) {

          /*
          File is already there
          */

          fclose(fp);
          filename_best_match_count++;
       }
    }
 }

 if ( filename_best_match_count == count ) {

    /*
    All the best match files are already here  
    No need to do anything
    */

    return;
 }

 /*
 Retrieve the matches for each pair of cameras
 */

 cam_match_arr= (match_struct ***)calloc(camera_nbr,sizeof(match_struct **));
 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {
    cam_match_arr[camera1_ind]= (match_struct **)calloc(camera_nbr,sizeof(match_struct *));
 }
 cam_match_nbr= (int **)calloc(camera_nbr,sizeof(int *));
 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {
    cam_match_nbr[camera1_ind]= (int *)calloc(camera_nbr,sizeof(int));
 }

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       /*
       Read the matches from disk
       */

       sprintf(filename_match,"gooder_match_%d_%d.sfm",
        camera1_ind,camera2_ind);

       sfm_read_matches(
        filename_match,
        &match_arr,
        &match_nbr
       );

       cam_match_arr[camera1_ind][camera2_ind]= match_arr;
       cam_match_nbr[camera1_ind][camera2_ind]= match_nbr;
    }
 }

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       match_arr= cam_match_arr[camera1_ind][camera2_ind];
       match_nbr= cam_match_nbr[camera1_ind][camera2_ind];

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
          Add to array of features to be processed
          */

          temp_nbr= 2;
          temp_arr= (sfm_temp_struct *)calloc(temp_nbr,sizeof(sfm_temp_struct));
          temp_arr[0].camera_ind= camera1_ind;
          temp_arr[0].feat_ind= feat1_ind;
          temp_arr[0].camera1_ind= camera1_ind;
          temp_arr[0].camera2_ind= camera2_ind;
          temp_arr[0].match_ind= match_ind;
          temp_arr[1].camera_ind= camera2_ind;
          temp_arr[1].feat_ind= feat2_ind;
          temp_arr[1].camera1_ind= camera1_ind;
          temp_arr[1].camera2_ind= camera2_ind;
          temp_arr[1].match_ind= match_ind;

          /*
          Process array of features
          Add any feature that's part of the track
          */

          sfm_remove_spurious_matches_2(
           image_filename_arr,
           camera_nbr,
           cam_match_arr,
           cam_match_nbr,
           &temp_arr,
           &temp_nbr
          );

          /*
          Check the validity of the track
          for 3D reconstruction
          */

          err_flag= sfm_remove_spurious_matches_3(
           image_filename_arr,
           camera_nbr,
           temp_arr,
           temp_nbr
          );

          if ( err_flag == 0 ) {

             /* 
             The track resulting from the match is valid
             */

             if ( temp_nbr > 0 )
              free(temp_arr);

             continue;
          }

          /*
          The track resluting from the match is not valid
          Remove (virtually) all matches involved with the track
          */

          sfm_remove_spurious_matches_4(
           image_filename_arr,
           camera_nbr,
           cam_match_arr,
           cam_match_nbr,
           temp_arr,
           temp_nbr
          );

          if ( temp_nbr > 0 )
           free(temp_arr);
       }
    }
 }

 /*
 Print the remaining matches on disk
 */

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       match_arr= cam_match_arr[camera1_ind][camera2_ind];
       match_nbr= cam_match_nbr[camera1_ind][camera2_ind];

       sprintf(filename_best_match,"best_match_%d_%d.sfm",camera1_ind,camera2_ind);

       /*
       Print the best matches on disk
       */

       sfm_print_matches(
        filename_best_match,
        match_arr,
        match_nbr
       );
    }
 }

 /*
 Free cam_match_arr 
 */

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {

    for ( camera2_ind= camera1_ind+1 ;
          camera2_ind< camera_nbr ;
          camera2_ind++ ) {

       match_arr= cam_match_arr[camera1_ind][camera2_ind];
       match_nbr= cam_match_nbr[camera1_ind][camera2_ind];

       if ( match_nbr > 0 )
        free(match_arr);
    }
 }

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {
    if ( camera_nbr > 0 )
     free(cam_match_arr[camera1_ind]);
 }

 if ( camera_nbr > 0 )
  free(cam_match_arr);

 /*
 Free cam_match_nbr
 */

 for ( camera1_ind= 0 ;
       camera1_ind< camera_nbr ;
       camera1_ind++ ) {
    if ( camera_nbr > 0 )
     free(cam_match_nbr[camera1_ind]);
 }

 if ( camera_nbr > 0 )
  free(cam_match_nbr);

}
