#include "header.h"

void sfm_free_features_and_descriptors(
 sfm_feat_struct *feat_arr,
 int feat_nbr
)

{

 int feat_ind;
 sfm_feat_struct feat;
 double x;
 double y;
 double *descr_arr;
 int descr_nbr;

 for ( feat_ind= 0; feat_ind< feat_nbr ; feat_ind++ ) {
    feat= feat_arr[feat_ind];
    x= feat.x;
    y= feat.y;
    descr_arr= feat.descr_arr;
    descr_nbr= feat.descr_nbr; 

    if ( descr_nbr > 0 )
     free(descr_arr);
 }

 if ( feat_nbr > 0 )
  free(feat_arr);

}
