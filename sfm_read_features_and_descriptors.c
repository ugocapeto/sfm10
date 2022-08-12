#include "header.h"

void sfm_read_features_and_descriptors(
 char *filename,
 sfm_feat_struct **pfeat_arr,
 int *pfeat_nbr
)

{

 int feat_nbr;
 FILE *fp;
 double x;
 double y;
 int descr_nbr;
 sfm_feat_struct *feat_arr;
 int feat_ind;
 double *descr_arr;
 int descr_ind;

 feat_arr= 0;
 feat_nbr= 0;

 fp= fopen(filename,"r");

 while ( fscanf(fp,"%lg %lg %d",
          &x,&y,&descr_nbr) == 3 ) {
    if ( feat_nbr == 0 ) {
       feat_arr= (sfm_feat_struct *)calloc(feat_nbr+1,sizeof(sfm_feat_struct));
    }
    else {
       feat_arr= (sfm_feat_struct *)realloc(feat_arr,(feat_nbr+1)*sizeof(sfm_feat_struct));
    }
    feat_ind= feat_nbr;
    feat_nbr++;
    feat_arr[feat_ind].x= x;
    feat_arr[feat_ind].y= y;
    feat_arr[feat_ind].descr_nbr= descr_nbr;
    if ( descr_nbr > 0 )
     descr_arr= (double *)calloc(descr_nbr,sizeof(double));
    for ( descr_ind= 0 ;
          descr_ind< descr_nbr ;
          descr_ind++ ) {
       fscanf(fp,"%lg",
        &descr_arr[descr_ind]);
    }
    feat_arr[feat_ind].descr_arr= descr_arr;
 }

 fclose(fp);

 (*pfeat_arr)= feat_arr;
 (*pfeat_nbr)= feat_nbr;

}
