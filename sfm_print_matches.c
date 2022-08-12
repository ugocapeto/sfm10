#include "header.h"

void sfm_print_matches(
 char *filename,
 match_struct *match_arr,
 int match_nbr
)

{

 FILE *fp;
 int match_ind;
 match_struct match;
 int feat1_ind;
 double x1;
 double y1;
 int feat2_ind;
 double x2;
 double y2;

 fp= fopen(filename,"w");

 for ( match_ind= 0; match_ind< match_nbr ; match_ind++ ) {
    match= match_arr[match_ind];

    feat1_ind= match.feat1_ind; 
    x1= match.x1;
    y1= match.y1;
    feat2_ind= match.feat2_ind; 
    x2= match.x2; 
    y2= match.y2; 

    /*
    Check if the match has been (virtually) removed
    */

    if ( feat1_ind == -1 &&
         feat2_ind == -1 )
     continue;

    /*
    If here, this is a valid match
    */

    fprintf(fp,"%d %g %g %d %g %g ",
     feat1_ind,x1,y1,feat2_ind,x2,y2);

    fprintf(fp,"\n");
 }

 fclose(fp);

}
