#include "header.h"

void sfm_read_matches(
 char *filename,
 match_struct **pmatch_arr,
 int *pmatch_nbr
)

{

 match_struct *match_arr;
 int match_nbr;
 FILE *fp;
 int feat1_ind;
 double x1;
 double y1;
 int feat2_ind;
 double x2;
 double y2;
 int match_ind;

 match_arr= 0;
 match_nbr= 0;

 fp= fopen(filename,"r");

 while ( fscanf(fp,"%d %lg %lg %d %lg %lg",
          &feat1_ind,&x1,&y1,&feat2_ind,&x2,&y2) == 6 ) {
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

 fclose(fp);

 (*pmatch_arr)= match_arr;
 (*pmatch_nbr)= match_nbr;

}
