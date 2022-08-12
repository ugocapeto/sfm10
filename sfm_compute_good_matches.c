#include "header.h"

void sfm_compute_good_matches(
 int width,
 int height,
 match_struct *match_arr,
 int match_nbr,
 int ntrials,
 match_struct **pgood_match_arr,
 int *pgood_match_nbr,
 double F[3*3]
)

{

 FILE *fp;
 int match_ind;
 int feat1_ind;
 double x1;
 double y1;
 int feat2_ind;
 double x2;
 double y2;
 unsigned long seed;
 int err_flag;
 match_struct *good_match_arr;
 int good_match_nbr;
 int good_match_ind;
 int i;
 int j;

 /*
 Print the matches into the input file
 */

 fp= fopen("match.txt","w");
 for ( match_ind= 0 ; match_ind< match_nbr ; match_ind++ ) {
    feat1_ind= match_arr[match_ind].feat1_ind;
    x1= match_arr[match_ind].x1;
    y1= match_arr[match_ind].y1;
    feat2_ind= match_arr[match_ind].feat2_ind;
    x2= match_arr[match_ind].x2;
    y2= match_arr[match_ind].y2;
    fprintf(fp,"%d %g %g %d %g %g\n",
     feat1_ind,x1,y1,feat2_ind,x2,y2);
 }
 fclose(fp);

 /*
 Use A Contrario RANSAC (ORSA) to reject outliers
 */

 seed = 1;

 err_flag= orsa_main(
  width,
  height,
  "match.txt",
  "good_match.txt",
  "F.txt",
  ntrials,
  seed
 );

 /*
 Initialize good_match_arr
 */

 good_match_arr= 0;
 good_match_nbr= 0;

 /*
 Initialize F
 */
 
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       F[i*3+j]= 0;
    }
 }

 if ( err_flag == 1 ) {
    goto END;
 }

 /*
 Store the good matches from the output file
 */

 fp= fopen("good_match.txt","r");
 while ( fscanf(fp,"%d %lg %lg %d %lg %lg",
          &feat1_ind,&x1,&y1,&feat2_ind,&x2,&y2) == 6 ) {
    if ( good_match_nbr == 0 ) {
       good_match_arr= (match_struct *)calloc(good_match_nbr+1,sizeof(match_struct));
    }
    else {
       good_match_arr= (match_struct *)realloc(good_match_arr,(good_match_nbr+1)*sizeof(match_struct));
    }
    good_match_ind= good_match_nbr;
    good_match_nbr++;
    good_match_arr[good_match_ind].feat1_ind= feat1_ind;
    good_match_arr[good_match_ind].x1= x1;
    good_match_arr[good_match_ind].y1= y1;
    good_match_arr[good_match_ind].feat2_ind= feat2_ind;
    good_match_arr[good_match_ind].x2= x2;
    good_match_arr[good_match_ind].y2= y2;
 }
 fclose(fp);

 /*
 Store F from the output file
 */

 fp= fopen("F.txt","r");
 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fscanf(fp,"%lg",
        &F[i*3+j]);
    }
 }
 fclose(fp);

 END:

 (*pgood_match_arr)= good_match_arr;
 (*pgood_match_nbr)= good_match_nbr;

}
