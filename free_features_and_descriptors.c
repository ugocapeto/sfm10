#include "header.h"

void free_features_and_descriptors(
 feat_struct *feat_arr,
 int feat_nbr
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

    if ( descr_nbr > 0 )
     free(descr);
 }

 if ( feat_nbr > 0 )
  free(feat_arr);

}
