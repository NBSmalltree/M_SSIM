#include "function.h"

#define PI 3.1415926


int main(int argc, char ** argv) 
{
	printf("Welcome to MSSIM!\n");

	////////////////////////////////////1//////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	/*Mat lColor = imread(".\\1.Aloe\\Aloe_VSRS_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\1.Aloe\\Aloe_VSRS_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\1.Aloe\\Aloe_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\1.Aloe\\Aloe_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.6738 * data1[j] + 37.2624;
			data2[j] = 0.6738 * data2[j] + 37.2624;
			data1[j] /= 4;
			data2[j] /= 4;
		}
	}*/
	////////////////////////////////////2//////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	/*Mat lColor = imread(".\\2.Dolls\\Dolls_VSRS_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\2.Dolls\\Dolls_VSRS_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\2.Dolls\\Dolls_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\2.Dolls\\Dolls_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.8599 * data1[j] - 0.5096;
			data2[j] = 0.8599 * data2[j] - 0.5096;
			data1[j] /= 4;
			data2[j] /= 4;
		}
	}*/
	//////////////////////////////////////3////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	/*Mat lColor = imread(".\\3.Reindeer\\Reindeer_VSRS_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\3.Reindeer\\Reindeer_VSRS_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\3.Reindeer\\Reindeer_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\3.Reindeer\\Reindeer_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.7727 * data1[j] + 3.7273;
			data2[j] = 0.7727 * data2[j] + 3.7273;
			data1[j] /= 4;
			data2[j] /= 4;
		}
	}*/
	//////////////////////////////////////4////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	/*Mat lColor = imread(".\\4.Laundry\\Laundry_InterViewConsistency_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\4.Laundry\\Laundry_InterViewConsistency_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\4.Laundry\\Laundry_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\4.Laundry\\Laundry_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.8 * data1[j] + 17.2;
			data2[j] = 0.8 * data2[j] + 17.2;
			data1[j] /= 4;
			data2[j] /= 4;
		}
	}*/
	//////////////////////////////////////5////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	/*Mat lColor = imread(".\\5.Lovebird1\\Lovebird1_InterViewConsistency_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\5.Lovebird1\\Lovebird1_InterViewConsistency_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\5.Lovebird1\\Lovebird1_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\5.Lovebird1\\Lovebird1_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.1358 * data1[j] + 0.5018;
			data2[j] = 0.1358 * data2[j] + 0.5018;
		}
	}*/
	//////////////////////////////////////6////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	/*Mat lColor = imread(".\\6.Newspaper\\Newspaper_VSRS_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\6.Newspaper\\Newspaper_VSRS_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\6.Newspaper\\Newspaper_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\6.Newspaper\\Newspaper_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.1371 * data1[j] + 14.9863;
			data2[j] = 0.1371 * data2[j] + 14.9863;
		}
	}*/
	///////////////////////////////////////7///////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lColor = imread(".\\7.Bookarrival\\Bookarrival_InterViewConsistency_view4.bmp", 2 | 4);
	Mat rColor = imread(".\\7.Bookarrival\\Bookarrival_InterViewConsistency_view3.bmp", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread(".\\7.Bookarrival\\Bookarrival_InterViewConsistency_depth_view4.bmp", 2 | 4);
	Mat rDisp = imread(".\\7.Bookarrival\\Bookarrival_InterViewConsistency_depth_view3.bmp", 2 | 4);

	for (int i = 0; i < lDisp.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);//��ָ��;
		uchar* data2 = rDisp.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lDisp.cols; j++) {
			data1[j] = 0.0764 * data1[j] + 14.6023;
			data2[j] = 0.0764 * data2[j] + 14.6023;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	/*//�����ɫͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lColor = imread("view1.png", 2 | 4);
	Mat rColor = imread("view5.png", 2 | 4);
	//�����Ӳ�ͼ��ע��JUNG���ݿ�����Ŵ�������ӵ㣬���С�������ӵ�;
	Mat lDisp = imread("disp1.png", 2 | 4);
	Mat rDisp = imread("disp5.png", 2 | 4);*/

	double CBA, CBAl, CBAr;
	CBAl = r2l(lColor, rColor, lDisp, rDisp);
	CBAr = l2r(lColor, rColor, lDisp, rDisp);
	CBA = (CBAl + CBAr)/2;

	printf("\n\nȫ��CBAΪ��\n");
	printf("CBA = %.4lf\n", CBA);

	waitKey();

	return 1;
}