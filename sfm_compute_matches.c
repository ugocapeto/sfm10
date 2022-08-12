#include "header.h"

void sfm_compute_matches(
 sfm_feat_struct *feat1_arr,
 int feat1_nbr,
 sfm_feat_struct *feat2_arr,
 int feat2_nbr,
 match_struct **pmatch_arr,
 int *pmatch_nbr
)

{

 double *descr1_arr;
 double *descr2_arr;
 int feat1_ind;
 int feat2_ind;
 match_struct *match_arr;
 int match_nbr;
 double x1;
 double y1;
 int descr1_nbr;
 int descr2_nbr;
 double x2;
 double y2;
 int match_ind;

 match_arr= 0;
 match_nbr= 0;

 for ( feat1_ind= 0 ;
       feat1_ind< feat1_nbr ;
       feat1_ind++ ) {

    x1= feat1_arr[feat1_ind].x;
    y1= feat1_arr[feat1_ind].y;
    descr1_arr= feat1_arr[feat1_ind].descr_arr;
    descr1_nbr= feat1_arr[feat1_ind].descr_nbr;

    sfm_compute_match(
     feat1_arr,
     feat1_ind,
     feat2_arr,
     feat2_nbr,
     &feat2_ind
    );

    if ( feat2_ind == -1 )
     continue;

    /*
    We have a match
    */

    x2= feat2_arr[feat2_ind].x;
    y2= feat2_arr[feat2_ind].y;
    descr2_arr= feat2_arr[feat2_ind].descr_arr;
    descr2_nbr= feat2_arr[feat2_ind].descr_nbr;

    if ( match_nbr == 0 ) {
       match_arr= (match_struct *)calloc(match_nbr+1,sizeof(match_struct));
    }
    else {
       match_arr= (match_struct *)realloc(match_arr,(match_nbr+1)*sizeof(match_struct));
    }
    match_ind= match_nbr;
    match_nbr++;
    match_arr[match_ind].feat1_ind= feat1_ind;
    match_arr[match_ind].x1= x1;
    match_arr[match_ind].y1= y1;
    match_arr[match_ind].feat2_ind= feat2_ind;
    match_arr[match_ind].x2= x2;
    match_arr[match_ind].y2= y2;
 }

 (*pmatch_arr)= match_arr;
 (*pmatch_nbr)= match_nbr;

}
