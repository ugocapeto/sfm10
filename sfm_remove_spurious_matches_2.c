#include "header.h"

void sfm_remove_spurious_matches_2(
 char **image_filename_arr,
 int camera_nbr,
 match_struct ***cam_match_arr,
 int **cam_match_nbr,
 sfm_temp_struct **ptemp_arr,
 int *ptemp_nbr
)

{

 sfm_temp_struct *temp_arr;
 int temp_nbr;
 int temp_ind;
 int inp_camera_ind;
 int inp_feat_ind;
 int camera_ind;
 int camera1_ind;
 int camera2_ind;
 match_struct *match_arr;
 int match_nbr;
 match_struct match;
 int match_ind;
 int feat1_ind;
 double x1;
 double y1;
 int feat2_ind;
 double x2;
 double y2;
 int match_flag;
 int feat_ind;
 int flag;
 int tind;

 temp_arr= (*ptemp_arr);
 temp_nbr= (*ptemp_nbr);

 for ( temp_ind= 0 ;
       temp_ind< temp_nbr ;
       temp_ind++ ) {
    inp_camera_ind= temp_arr[temp_ind].camera_ind;
    inp_feat_ind= temp_arr[temp_ind].feat_ind;

    for ( camera_ind= 0 ;
          camera_ind< camera_nbr ;
          camera_ind++ ) {

       if ( camera_ind == inp_camera_ind )
        continue;

       if ( camera_ind < inp_camera_ind ) {
          camera1_ind= camera_ind;
          camera2_ind= inp_camera_ind;
       }
       if ( inp_camera_ind < camera_ind ) {
          camera1_ind= inp_camera_ind;
          camera2_ind= camera_ind;
       }

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
          If here,
          this is a valid match
          */

          match_flag= 0;
          if ( camera_ind < inp_camera_ind ) {

             /*
             feat1_ind, x1, y1 correspond to camera_ind
             feat2_ind, x2, y2 correspond to inp_camera_ind
             */

             if ( feat2_ind == inp_feat_ind ) {
                feat_ind= feat1_ind;
                match_flag= 1;
             }
          }

          if ( inp_camera_ind < camera_ind ) {

             /*
             feat1_ind, x1, y1 correspond to inp_camera_ind
             feat2_ind, x2, y2 correspond to camera_ind
             */

             if ( feat1_ind == inp_feat_ind ) {
                feat_ind= feat2_ind;
                match_flag= 1;
             }
          }

          if ( match_flag == 0 )
           continue;

          /*
          Add to array of features to be processed
          unless already there
          */

          flag= 0;
          for ( tind= 0 ;
                tind< temp_nbr ;
                tind++ ) {
             if ( temp_arr[tind].camera_ind == camera_ind &&
                  temp_arr[tind].feat_ind == feat_ind ) {
                flag= 1;
             } 
          }
          if ( flag == 1 )
           continue;
 
          temp_arr= (sfm_temp_struct *)realloc(temp_arr,(temp_nbr+1)*sizeof(sfm_temp_struct));
          temp_arr[temp_nbr].camera_ind= camera_ind;
          temp_arr[temp_nbr].feat_ind= feat_ind;
          temp_arr[temp_nbr].camera1_ind= camera1_ind;
          temp_arr[temp_nbr].camera2_ind= camera2_ind;
          temp_arr[temp_nbr].match_ind= match_ind;
          temp_nbr++;
       }
    }
 }

 (*ptemp_arr)= temp_arr;
 (*ptemp_nbr)= temp_nbr;

}
