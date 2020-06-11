#ifndef GMATH_H
#define GMATH_H

#include <math.h>
/*第一个轴地址，剩下每个+50 */
#define AXISDATA 30126

/*第一旋转轴轴号*/
#define FIRSTAXISOFROTATION 41185
/*第二旋转轴轴号*/
#define SECONDAXISOFROTATION 41188

/*第一旋转轴偏转角矢量*/
#define THEFIRSTROTATIONAXISDEFLECTIONANGLEVECTOR 41130
/*第二旋转轴偏转角矢量*/
#define THESECONDROTATIONAXISDEFLECTIONANGLEVECTOR 41135


#ifdef WIN32
#define CIPATH  "./ci_set.txt"
#else
#define CIPATH "/home/Lynuc/Users/Config/ci_set.txt"
#endif

#define NCPATH "/home/Lynuc/Users/NCFiles/AXISNC.NC"



#define TRAN_PI 3.1415926535897932384626433832795
#define TRAN_DEG2RAD (TRAN_PI / 180.0)
#define TRAN_DEG2DEG (180.0 / TRAN_PI)




void RotVector(double *dRes, double *dSrc, double *dDir, double dAgl, bool bRightHand)
{
    double dCos = cos(dAgl);
    double dSin = sin(dAgl);
    double d1_cfq = (1.0 - dCos) * (dDir[0] * dSrc[0] + dDir[1] * dSrc[1] + dDir[2] * dSrc[2]);
    double dTemp1[3];
    double dTemp2[3];

    if(fabs(dAgl) < 1.0E-8)
    {
        dRes[0] = dSrc[0];
        dRes[1] = dSrc[1];
        dRes[2] = dSrc[2];
        return;
    }

    if(!bRightHand)
    {
        dSin = - dSin;
    }

    dTemp1[0] = (dDir[1] * dSrc[2] - dDir[2] * dSrc[1]) * dSin;
    dTemp1[1] = (dDir[2] * dSrc[0] - dDir[0] * dSrc[2]) * dSin;
    dTemp1[2] = (dDir[0] * dSrc[1] - dDir[1] * dSrc[0]) * dSin;

    dTemp2[0] = dSrc[0] * dCos + dDir[0] * d1_cfq;
    dTemp2[1] = dSrc[1] * dCos + dDir[1] * d1_cfq;
    dTemp2[2] = dSrc[2] * dCos + dDir[2] * d1_cfq;

    dRes[0] = dTemp1[0] + dTemp2[0];
    dRes[1] = dTemp1[1] + dTemp2[1];
    dRes[2] = dTemp1[2] + dTemp2[2];
}
/**
 * 根据高度计算新的XYZ
 * dResDltaMove：新的XYZ
 * dDeltaH：高度
 * dFirstDir：第一旋转轴矢量
 * dSecDir：第二旋转轴矢量
 * dFridtAngle：第一轴角度
 * dSecAngle：第二轴角度
*/

void CalDeltaMoveCoord(double *dResDltaMove, double dDeltaH, double *dFirstDir, double *dSecDir, double dFridtAngle, double dSecAngle)
{
    double dDirW[3] = {0.0, 0.0, 0.0};
    double dTempVec[3] = {0.0, 0.0, 0.0};
    double dToolDir[3] = {0.0, 0.0, -1.0};
    RotVector(dTempVec, dToolDir, dSecDir, dSecAngle * TRAN_DEG2RAD, 1);
    RotVector(dDirW, dTempVec, dFirstDir, dFridtAngle * TRAN_DEG2RAD, 1);
    dResDltaMove[0] = dDirW[0] * dDeltaH;
    dResDltaMove[1] = dDirW[1] * dDeltaH;
    dResDltaMove[2] = dDirW[2] * dDeltaH;
}


#endif // GMATH_H
