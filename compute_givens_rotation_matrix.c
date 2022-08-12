#include "header.h"

void compute_givens_rotation_matrix(
 double A[3*3],
 int axis,
 double Q[3*3]
)

{

 double a33;
 double a32;
 double c;
 double s;
 double a31;
 double a22;
 double a21;

 if ( axis == (1-1) ) {

    /*
    x axis
    */

    a33= A[(3-1)*3+(3-1)];
    a32= A[(3-1)*3+(2-1)];
    c=-a33/sqrt(a32*a32+a33*a33); 
    s= a32/sqrt(a32*a32+a33*a33);

    Q[0*3+0]= 1;
    Q[0*3+1]= 0;
    Q[0*3+2]= 0;

    Q[1*3+0]= 0;
    Q[1*3+1]= c;
    Q[1*3+2]=-s;

    Q[2*3+0]= 0;
    Q[2*3+1]= s;
    Q[2*3+2]= c;
 }

 if ( axis == (2-1) ) {

    /*
    y axis
    */

    a33= A[(3-1)*3+(3-1)];
    a31= A[(3-1)*3+(1-1)];
    c= a33/sqrt(a31*a31+a33*a33);
    s= a31/sqrt(a31*a31+a33*a33);

    Q[0*3+0]= c;
    Q[0*3+1]= 0;
    Q[0*3+2]= s;

    Q[1*3+0]= 0;
    Q[1*3+1]= 1;
    Q[1*3+2]= 0;

    Q[2*3+0]=-s;
    Q[2*3+1]= 0;
    Q[2*3+2]= c;
 }

 if ( axis == (3-1) ) {

    /*
    z axis
    */

    a22= A[(2-1)*3+(2-1)];
    a21= A[(2-1)*3+(1-1)];
    c=-a22/sqrt(a22*a22+a21*a21);
    s= a21/sqrt(a22*a22+a21*a21);

    Q[0*3+0]= c;
    Q[0*3+1]=-s;
    Q[0*3+2]= 0;

    Q[1*3+0]= s;
    Q[1*3+1]= c;
    Q[1*3+2]= 0;

    Q[2*3+0]= 0;
    Q[2*3+1]= 0;
    Q[2*3+2]= 1;
 }

}
