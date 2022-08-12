#include "header.h"

void compute_rotation_from_quaternion(
 double q[4],
 double M[3*3]
)

/*
Code taken from stuff8/pba_v1.0.5/pba/src/pba/DataInterface.h
*/

{

    /* original code
    template <class Float>     void SetQuaternionRotation(const Float q[4])
    {
        double qq = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
        double qw, qx, qy, qz;
        if(qq>0)
        {
            qw=q[0]/qq;
            qx=q[1]/qq;
            qy=q[2]/qq;
            qz=q[3]/qq;
        }else
        {
            qw = 1;
            qx = qy = qz = 0;
        }
        m[0][0]=float_t(qw*qw + qx*qx- qz*qz- qy*qy );
        m[0][1]=float_t(2*qx*qy -2*qz*qw );
        m[0][2]=float_t(2*qy*qw + 2*qz*qx);
        m[1][0]=float_t(2*qx*qy+ 2*qw*qz);
        m[1][1]=float_t(qy*qy+ qw*qw - qz*qz- qx*qx);
        m[1][2]=float_t(2*qz*qy- 2*qx*qw);
        m[2][0]=float_t(2*qx*qz- 2*qy*qw);
        m[2][1]=float_t(2*qy*qz + 2*qw*qx );
        m[2][2]=float_t(qz*qz+ qw*qw- qy*qy- qx*qx);
    }
    */

    double m[3][3];
    int i;
    int j;

        double qq = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
        double qw, qx, qy, qz;
        if(qq>0)
        {
            qw=q[0]/qq;
            qx=q[1]/qq;
            qy=q[2]/qq;
            qz=q[3]/qq;
        }else
        {
            qw = 1;
            qx = qy = qz = 0;
        }
        m[0][0]=qw*qw + qx*qx- qz*qz- qy*qy;
        m[0][1]=2*qx*qy -2*qz*qw;
        m[0][2]=2*qy*qw + 2*qz*qx;
        m[1][0]=2*qx*qy+ 2*qw*qz;
        m[1][1]=qy*qy+ qw*qw - qz*qz- qx*qx;
        m[1][2]=2*qz*qy- 2*qx*qw;
        m[2][0]=2*qx*qz- 2*qy*qw;
        m[2][1]=2*qy*qz + 2*qw*qx;
        m[2][2]=qz*qz+ qw*qw- qy*qy- qx*qx;

    for ( i= 0 ; i< 3 ; i++ ) {
       for ( j= 0 ; j< 3 ; j++ ) {
          M[i*3+j]= m[i][j];
       }
    }

}
