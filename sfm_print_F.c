#include "header.h"

void sfm_print_F(
 char *filename,
 double F[3*3]
)

{

 FILE *fp;
 int i;
 int j;

 fp= fopen(filename,"w");

 for ( i= 0 ; i< 3 ; i++ ) {
    for ( j= 0 ; j< 3 ; j++ ) {
       fprintf(fp,"%g ",
        F[i*3+j]);
    }
 }

 fclose(fp);

}
