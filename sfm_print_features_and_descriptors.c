#include "header.h"

void sfm_print_features_and_descriptors(
 char *filename,
 sfm_feat_struct *feat_arr,
 int feat_nbr
)

{

 FILE *fp;
 int feat_ind;
 sfm_feat_struct feat;
 double x;
 double y;
 double *descr_arr;
 int descr_nbr;
 int descr_ind;

 fp= fopen(filename,"w");

 for ( feat_ind= 0; feat_ind< feat_nbr ; feat_ind++ ) {
    feat= feat_arr[feat_ind];

    x= feat.x;
    y= feat.y;
    descr_arr= feat.descr_arr;
    descr_nbr= feat.descr_nbr;

    fprintf(fp,"%g %g ",x,y);
    fprintf(fp,"%d ",descr_nbr);

    for ( descr_ind= 0 ; descr_ind< descr_nbr ; descr_ind++ )
     fprintf(fp,"%g ",descr_arr[descr_ind]);

    fprintf(fp,"\n");
 }

 fclose(fp);

}
