#include "header.h"

void epnp_compute_rho(
 double cws[4][3],
 double *rho
)

{

 rho[0] = epnp_dist2(cws[0], cws[1]);
 rho[1] = epnp_dist2(cws[0], cws[2]);
 rho[2] = epnp_dist2(cws[0], cws[3]);
 rho[3] = epnp_dist2(cws[1], cws[2]);
 rho[4] = epnp_dist2(cws[1], cws[3]);
 rho[5] = epnp_dist2(cws[2], cws[3]);

}
