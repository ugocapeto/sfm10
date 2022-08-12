#include "header.h"

void sfm_check_threedpoints(
 int camera_nbr,
 sfm_threedpoint_struct *threedpoint_arr,
 int threedpoint_nbr
)

/*
Look for image points that are somehow associated
with more than one 3D point
*/

{

 sfm_imagepoint_struct *imagepoint_arr;
 int imagepoint_nbr;
 sfm_feat_struct *feat_arr;
 int **feat2threedpoint_arr;
 int *feat2threedpoint_nbr;
 int camera_ind;
 char filename[80];
 int feat_nbr;
 int feat_ind;
 int threedpoint_ind;
 double xyz[3];
 int rgb[3];
 int imagepoint_ind;
 sfm_imagepoint_struct imagepoint;
 double x;
 double y;
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
 int threedpoint_ind1;
 int threedpoint_ind2;
 sfm_imagepoint_struct *imagepoint_arr1;
 int imagepoint_nbr1;
 sfm_imagepoint_struct *imagepoint_arr2;
 int imagepoint_nbr2;
 sfm_imagepoint_struct imagepoint1;
 sfm_imagepoint_struct imagepoint2;
 int imagepoint_ind1;
 int imagepoint_ind2;
 int camera_ind1;
 int camera_ind2;

 /*
 Initialize feat2threedpoint_arr
 feat2threedpoint_arr[camera_ind][feat_ind]= 3D point
 */

 if ( camera_nbr > 0 ) {
    feat2threedpoint_arr= (int **)calloc(camera_nbr,sizeof(int *));
    feat2threedpoint_nbr= (int *)calloc(camera_nbr,sizeof(int));
 }
 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    sprintf(filename,"feat_%d.sfm",camera_ind);
    sfm_read_features_and_descriptors(
     filename,
     &feat_arr,
     &feat_nbr
    );
    sfm_free_features_and_descriptors(
     feat_arr,
     feat_nbr
    );
    feat2threedpoint_nbr[camera_ind]= feat_nbr;
    if ( feat_nbr > 0 )
     feat2threedpoint_arr[camera_ind]= (int *)calloc(feat_nbr,sizeof(int));
    for ( feat_ind= 0 ;
          feat_ind< feat_nbr ;
          feat_ind++ )
     feat2threedpoint_arr[camera_ind][feat_ind]=-1;
 }

 for ( threedpoint_ind= 0 ;
       threedpoint_ind< threedpoint_nbr ;
       threedpoint_ind++ ) {

    xyz[0]= threedpoint_arr[threedpoint_ind].xyz[0];
    xyz[1]= threedpoint_arr[threedpoint_ind].xyz[1];
    xyz[2]= threedpoint_arr[threedpoint_ind].xyz[2];
    rgb[0]= threedpoint_arr[threedpoint_ind].rgb[0];
    rgb[1]= threedpoint_arr[threedpoint_ind].rgb[1];
    rgb[2]= threedpoint_arr[threedpoint_ind].rgb[2];
    imagepoint_arr= threedpoint_arr[threedpoint_ind].imagepoint_arr;
    imagepoint_nbr= threedpoint_arr[threedpoint_ind].imagepoint_nbr;

    for ( imagepoint_ind= 0 ;
          imagepoint_ind< imagepoint_nbr ;
          imagepoint_ind++ ) {
       imagepoint= imagepoint_arr[imagepoint_ind];
       camera_ind= imagepoint.camera_ind;
       feat_ind= imagepoint.feat_ind;
       x= imagepoint.x;
       y= imagepoint.y;

       /*
       Associate (camera_ind,feat_ind) with 3D point
       */

       if ( feat2threedpoint_arr[camera_ind][feat_ind] != -1 ) {
          sfm10_error_handler("sfm_check_threedpoints");
       }
       feat2threedpoint_arr[camera_ind][feat_ind]= threedpoint_ind;
    }
 }

 /*
 If there is a match between two cameras,
 the 3D point should be unique
 */

 /*
 This is not necessarily true as
 image points and 3D points can be removed
 for whatever reason
 */

 goto END;

 for ( camera_ind1= 0 ;
       camera_ind1< camera_nbr ;
       camera_ind1++ ) {

    for ( camera_ind2= camera_ind1+1 ;
          camera_ind2< camera_nbr ;
          camera_ind2++ ) {

       /*
       Read the matches from disk
       */

       sprintf(filename_match,"best_match_%d_%d.sfm",
        camera_ind1,camera_ind2);

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

          threedpoint_ind1= feat2threedpoint_arr[camera_ind1][feat1_ind];
          threedpoint_ind2= feat2threedpoint_arr[camera_ind2][feat2_ind];

          imagepoint_arr1= threedpoint_arr[threedpoint_ind1].imagepoint_arr;
          imagepoint_nbr1= threedpoint_arr[threedpoint_ind1].imagepoint_nbr;

          imagepoint_arr2= threedpoint_arr[threedpoint_ind2].imagepoint_arr;
          imagepoint_nbr2= threedpoint_arr[threedpoint_ind2].imagepoint_nbr;

          if ( threedpoint_ind1 != -1 &&
               threedpoint_ind2 != -1 ) {
             if ( threedpoint_ind2 != threedpoint_ind1 ) {

                /*
                That's a problem
                */

                /*
                Print some stuff before giving the error message
                */

                fprintf(stdout,"3D point 1 = %d\n",
                 threedpoint_ind1);

                for ( imagepoint_ind1= 0 ;
                      imagepoint_ind1< imagepoint_nbr1 ;
                      imagepoint_ind1++ ) {
                   imagepoint1= imagepoint_arr1[imagepoint_ind1];
                   camera1_ind= imagepoint1.camera_ind;
                   feat1_ind= imagepoint1.feat_ind;
                   x1= imagepoint1.x;
                   y1= imagepoint1.y;
                   fprintf(stdout,"camera1_ind = %d feat1_ind = %d x1 = %g y1 = %g\n",
                    camera1_ind,feat1_ind,x1,y1);
                }

                fprintf(stdout,"3D point 2 = %d\n",
                 threedpoint_ind2);

                for ( imagepoint_ind2= 0 ;
                      imagepoint_ind2< imagepoint_nbr2 ;
                      imagepoint_ind2++ ) {
                   imagepoint2= imagepoint_arr2[imagepoint_ind2];
                   camera2_ind= imagepoint2.camera_ind;
                   feat2_ind= imagepoint2.feat_ind;
                   x2= imagepoint2.x;
                   y2= imagepoint2.y;
                   fprintf(stdout,"camera2_ind = %d feat2_ind = %d x2 = %g y2 = %g\n",
                    camera2_ind,feat2_ind,x2,y2);
                }

                /* 
                Give the error message
                */

                sfm10_error_handler("sfm_check_threedpoints");
             }
          }
       }

       if ( match_nbr > 0 )
        free(match_arr);
    }
 }

 END:

 /*
 Free feat2threedpoint_arr
 */

 for ( camera_ind= 0 ;
       camera_ind< camera_nbr ;
       camera_ind++ ) {
    if ( feat2threedpoint_nbr[camera_ind] > 0 )
     free(feat2threedpoint_arr[camera_ind]);
 }
 if ( camera_nbr > 0 ) {
    free(feat2threedpoint_arr);
    free(feat2threedpoint_nbr);
 }

}
