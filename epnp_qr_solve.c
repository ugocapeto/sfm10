#include "header.h"

void epnp_qr_solve(
 double *A,
 int nr,
 int nc,
 double *b,
 double *x
)

/*
Solve A*x=b
nr = number of rows for matrix A
nc = number of cols for matrix A
*/

/*
This is assumed to be an over-determined system (nr>=nc)
*/

/*
Works correctly
even if the output x doesn't satisfy Ax=b
x is however the best solution in the least squares sense
Check epnp_normal_equations_solve()
to convince yourself that it gives correct results
*/

{

 double *A1;
 double *A2;
 double *Ax;
 double *pA;
 double *pb;
 int i;
 int j;

 /*
 Copy A into pA
 */

 pA= (double *)calloc(nr*nc,sizeof(double));
 for ( i= 0 ; i< nr ; i++ ) {
    for ( j= 0 ; j< nc ; j++ ) {
       pA[i*nc+j]= A[i*nc+j]; 
    }
 }

 /*
 Copy b into pb
 */

 pb= (double *)calloc(nr,sizeof(double));
 for ( i= 0 ; i< nr ; i++ ) {
    pb[i]= b[i];
 }

 A1= (double *)calloc(nr,sizeof(double));
 A2= (double *)calloc(nr,sizeof(double));

  double * ppAkk = pA;
  for(int k = 0; k < nc; k++) {
    double * ppAik = ppAkk, eta = fabs(*ppAik);
    for(int i = k + 1; i < nr; i++) {
      double elt = fabs(*ppAik);
      if (eta < elt) eta = elt;
      ppAik += nc;
    }

    if (eta == 0) {
      A1[k] = A2[k] = 0.0;
      fprintf(stdout,"God damnit, A is singular, this shouldn't happen.");
      return;
    } else {
      double * ppAik = ppAkk, sum = 0.0, inv_eta = 1. / eta;
      for(int i = k; i < nr; i++) {
        *ppAik *= inv_eta;
        sum += *ppAik * *ppAik;
        ppAik += nc;
      }
      double sigma = sqrt(sum);
      if (*ppAkk < 0)
        sigma = -sigma;
      *ppAkk += sigma;
      A1[k] = sigma * *ppAkk;
      A2[k] = -eta * sigma;
      for(int j = k + 1; j < nc; j++) {
        double * ppAik = ppAkk, sum = 0;
        for(int i = k; i < nr; i++) {
          sum += *ppAik * ppAik[j - k];
          ppAik += nc;
        }
        double tau = sum / A1[k];
        ppAik = ppAkk;
        for(int i = k; i < nr; i++) {
          ppAik[j - k] -= tau * *ppAik;
          ppAik += nc;
        }
      }
    }
    ppAkk += nc + 1;
  }

  /*
  b <- Qt b
  */

  double * ppAjj = pA;
  for(int j = 0; j < nc; j++) {
    double * ppAij = ppAjj, tau = 0;
    for(int i = j; i < nr; i++) {
      tau += *ppAij * pb[i];
      ppAij += nc;
    }
    tau /= A1[j];
    ppAij = ppAjj;
    for(int i = j; i < nr; i++) {
      pb[i] -= tau * *ppAij;
      ppAij += nc;
    }
    ppAjj += nc + 1;
  }

  /*
  X = R-1 b
  */

  x[nc - 1] = pb[nc - 1] / A2[nc - 1];
  for(int i = nc - 2; i >= 0; i--) {
    double * ppAij = pA + i * nc + (i + 1), sum = 0;

    for(int j = i + 1; j < nc; j++) {
      sum += *ppAij * x[j];
      ppAij++;
    }
    x[i] = (pb[i] - sum) / A2[i];
  }

 if ( nr > 0 )
  free(A1);
 if ( nr > 0 )
  free(A2);

 /*
 Free pA
 */

 if ( nr*nc > 0 )
  free(pA);

 /*
 Free pb
 */

 if ( nr > 0 )
  free(pb);

 /*
 Check!
 */

 Ax= (double *)calloc(nr,sizeof(double));

 math_matrix_vector_product(
  A,
  nr,
  nc,
  x,
  nc,
  Ax
 );

 free(Ax);

}
