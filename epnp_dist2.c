#include "header.h"

double epnp_dist2(
 double *p1,
 double *p2
)

{

  return
    (p1[0] - p2[0]) * (p1[0] - p2[0]) +
    (p1[1] - p2[1]) * (p1[1] - p2[1]) +
    (p1[2] - p2[2]) * (p1[2] - p2[2]);

}
