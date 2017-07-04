#include <iostream>
#include <iomanip>
#include "opencv_cfg.h"
#include <math.h>
#include <stdio.h>

using namespace std;
using namespace cv;

double getPSNR(const Mat& I1, const Mat& I2);
Scalar getMSSIM(const Mat& i1, const Mat& i2);
double r2l();
double l2r();