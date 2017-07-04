#include "function.h"

#define PI 3.1415926


int main(int argc, char ** argv) 
{
	printf("Welcome to MSSIM!\n");

	double CBA, CBAl, CBAr;
	CBAl = r2l();
	CBAr = l2r();
	CBA = (CBAl + CBAr)/2;

	printf("\n\nÈ«¾ÖCBAÎª£º\n");
	printf("CBA = %.2lf\n", CBA);

	waitKey();

	return 1;
}