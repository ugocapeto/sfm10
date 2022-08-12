#include "header.h"

void convert_features_and_descriptors_to_sfm(
 feat_struct *feat_arr,
 int feat_nbr,
 sfm_feat_struct **psfm_feat_arr 
)

{

 feat_struct feat;
 double *descr;
 int feat_ind;
 int octave;
 int interval;
 int i;
 int j;
 double xinterval;
 double xi;
 double xj;
 double pos_x;
 double pos_y;
 double sigma;
 double sigma_octave;
 double ori;
 int descr_nbr;
 int descr_ind;
 sfm_feat_struct *sfm_feat_arr;
 double *sfm_descr_arr;

 sfm_feat_arr= (sfm_feat_struct *)calloc(feat_nbr,sizeof(sfm_feat_struct));

 for ( feat_ind= 0; feat_ind< feat_nbr ; feat_ind++ ) {
    feat= feat_arr[feat_ind];

    octave= feat.octave;
    interval= feat.interval;
    i= feat.i;
    j= feat.j;
    xinterval= feat.xinterval;
    xi= feat.xi;
    xj= feat.xj;
    pos_x= feat.pos_x;
    pos_y= feat.pos_y;
    sigma= feat.sigma;
    sigma_octave= feat.sigma_octave;
    ori= feat.ori;
    descr= feat.descr;
    descr_nbr= feat.descr_nbr;

    sfm_feat_arr[feat_ind].x= pos_x;
    sfm_feat_arr[feat_ind].y= pos_y;
    sfm_feat_arr[feat_ind].descr_nbr= descr_nbr;

    if ( descr_nbr > 0 )
     sfm_descr_arr= (double *)calloc(descr_nbr,sizeof(double));

    for ( descr_ind= 0 ;
          descr_ind< descr_nbr ;
          descr_ind++ ) {
       sfm_descr_arr[descr_ind]= descr[descr_ind];
    }

    sfm_feat_arr[feat_ind].descr_arr= sfm_descr_arr;
 }

 (*psfm_feat_arr)= sfm_feat_arr;

}
