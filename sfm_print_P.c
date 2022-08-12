#include "header.h"

void sfm_print_P(
 char *filename,
 double P[3*4]
)

{

 FILE *fp;
 int i;
 int j;

 fp= fopen(filename,"w");

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 4 ; j++ ) {
       fprintf(fp,"%g ",
        P[i*4+j]);
    }
 }

 fclose(fp);

}
