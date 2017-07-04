#include <iostream>
#include <iomanip>
#include "opencv_cfg.h"
#include <math.h>
#include <stdio.h>

#define PI 3.1415926

using namespace std;
using namespace cv;

double getPSNR(const Mat& I1, const Mat& I2)
{
	Mat s1;
	absdiff(I1, I2, s1);       // |I1 - I2|
	s1.convertTo(s1, CV_32F);  // ������8λ��������ƽ������
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // ����ÿ��ͨ����Ԫ��;

	double sse = s.val[0] + s.val[1] + s.val[2]; // ��������ͨ��;

	if (sse <= 1e-10) // ���ֵ̫С��ֱ�ӵ���0
		return 0;
	else
	{
		double  mse = sse / (double)(I1.channels() * I1.total());
		double psnr = 10.0*log10((255 * 255) / mse);
		return psnr;
	}
}

Scalar getMSSIM(const Mat& i1, const Mat& i2)
{
	const double C1 = 6.5025, C2 = 58.5225;
	/***************************** INITS **********************************/
	int d = CV_32F;

	Mat I1, I2;
	i1.convertTo(I1, d);           // �����ڵ��ֽ������Ͻ��м��㣬��Χ������
	i2.convertTo(I2, d);

	Mat I2_2 = I2.mul(I2);        // I2^2
	Mat I1_2 = I1.mul(I1);        // I1^2
	Mat I1_I2 = I1.mul(I2);        // I1 * I2

								   /***********************�������� ******************************/

	Mat mu1, mu2;   //
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);

	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);

	Mat sigma1_2, sigma2_2, sigma12;

	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	///////////////////////////////// ��ʽ ////////////////////////////////
	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	Mat ssim_map;
	divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

	Scalar mssim = mean(ssim_map); // mssim = ssim_map��ƽ��ֵ
	return mssim;
}

int main(int argc, char ** argv) {

	printf("Welcome to MSSIM!\n");

	//д������;
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	//�����ɫͼ;
	Mat lColor = imread("view1.png", 2 | 4);
	Mat rColor = imread("view5.png", 2 | 4);
	//�����Ӳ�ͼ;
	Mat lDisp = imread("disp1.png", 2 | 4);
	Mat rDisp = imread("disp5.png", 2 | 4);

	//�����»Ҷ�ͼ�ռ�;
	Mat lGray, rGray;
	lGray.create(lColor.rows, lColor.cols, CV_8UC1);//CV_8UC1
	rGray.create(rColor.rows, rColor.cols, CV_8UC1);//CV_8UC1

	//���������ɫͼ(����ͨ��);
	//imshow("ԭʼ���ӵ�ͼ;", lColor);
	//imshow("ԭʼ���ӵ�ͼ;", rColor);

	//���������Ӳ�ͼ(����ͨ��);
	//imshow("ԭʼ���Ӳ�ͼ;", lDisp);
	//imshow("ԭʼ���Ӳ�ͼ;", rDisp);

	//ת���ɻҶ�ͼ;
	cvtColor(lColor, lGray, CV_RGB2GRAY);
	cvtColor(rColor, rGray, CV_RGB2GRAY);

	//����ת���ĻҶ�ͼ(����ͨ��);
	//imshow("���ӵ�Ҷ�ͼ;", lGray);
	//imshow("���ӵ�Ҷ�ͼ;", rGray);

	imwrite("���ӵ�Ҷ�ͼ.png", lGray, compression_params);

	//�����ӵ�Ҷ�ͼͨ���Ӳ�ͼӳ�䵽���ӵ�;
	//����ӳ��������ӵ�ͼ�ռ�;
	Mat rwarpedGray;
	rwarpedGray.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//z-buffer,�����ȱ���һ��λ����ģ;
	//����ӳ��������ӵ�ͼλ����ģ;
	Mat rwarpedLocal, rwarpedLocal_Value;
	rwarpedLocal.create(lGray.rows, lGray.cols, CV_16UC1);//CV_16UC1
	rwarpedLocal_Value.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		ushort* data = rwarpedLocal.ptr<ushort>(i);//��ָ��;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = rwarpedLocal_Value.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//����λ����ģ;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = rDisp.ptr<uchar>(i);//���ӵ��Ӳ�ͼ;
		ushort* data2 = rwarpedLocal.ptr<ushort>(i);//λ��;
		uchar* data3 = rwarpedLocal_Value.ptr<uchar>(i);//ӳ���ȥ���Ӳ�ֵ;
		for (int j = 0, k = 0; j < lGray.cols; j++) {//k����ӳ����Ŀ�����ص�;
			k = j + data1[j];
			if (k >= 0 && k < lGray.cols) {
				if (data3[k] < data1[j]) {//Ŀ�����ص�洢���Ӳ�ֵ<ԭʼ���ص��Ӳ�ֵ;
					data2[k] = j;//�洢λ��;
					data3[k] = data1[j];//�洢������Ӳ�ֵ;
				}
			}
		}
	}

	//ӳ�����;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = rGray.ptr<uchar>(i);
		ushort* data2 = rwarpedLocal.ptr<ushort>(i);
		uchar* data3 = rwarpedGray.ptr<uchar>(i);
		for (int j = 0; j < lGray.cols; j++) {
			if (data2[j] == 0){
				data3[j] = 0;
			}
			else {
				data3[j] = data1[data2[j]];
			}
		}
	}

	//�޸����ƿն�;
	for (int i = 0; i < rwarpedGray.rows; i++) {
		uchar* data = rwarpedGray.ptr<uchar>(i);
		for (int j = 1; j < rwarpedGray.cols - 1; j++) {
			if (data[j] == 0) {//�˴�205ֵΪMat����Ĭ��ֵ
				if (data[j - 1] != 0 && data[j + 1] != 0)//�ÿն���ֻ��һ���հ�����;
					data[j] = (data[j - 1] + data[j + 1]) / 2;
				if (data[j + 1] == 0 && data[j - 1] != 0 && data[j + 2] != 0) {//�ÿն���ֻ�������հ�����;
					data[j] = data[j + 1] = (data[j - 1] + data[j + 2]) / 2;
				}
			}
		}
	}

	imwrite("ӳ��������ӵ�ͼ.png", rwarpedGray, compression_params);

	//��ʾӳ��������ӵ�ͼ;
	//imshow("ӳ��������ӵ�ͼ;", rwarpedGray);

	//������Ҫͳ��Hl����;
	//ͳ����ߵ�ȫ0��������Щȫ0����ΪHl;
	//��Ϊbackward warping;

	//����Hl��ģ�ռ�;
	Mat hlMask;
	hlMask.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = hlMask.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//����Hl;
	for (int i = 0; i < rwarpedGray.rows; i++) {
		uchar* data1 = rwarpedGray.ptr<uchar>(i);
		uchar* data2 = hlMask.ptr<uchar>(i);
		for (int j = 0; j < rwarpedGray.cols; j++) {//����ʼ������������һ����0�ж�ѭ��;
			if (data1[j] == 0) {
				data2[j] = 1;
			}
			else
				break;
		}
	}

	//����Hlͼ(����ͨ��);
	//imshow("Hlͼ;", hlMask);

	//�������ӵ������ӵ�ӳ������ӵ����ͼ�Ŀռ�El;
	Mat lDiff;
	lDiff.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = lDiff.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//���ӵ������ӵ�ӳ������ӵ�����;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = lGray.ptr<uchar>(i);
		uchar* data2 = rwarpedGray.ptr<uchar>(i);
		uchar* data3 = lDiff.ptr<uchar>(i);
		uchar* data4 = hlMask.ptr<uchar>(i);
		for (int j = 0; j < lGray.cols; j++) {//ע������Ҫ��ȥHl�ռ�;
			if (data4[j] != 1) {
				data3[j] = abs(data1[j] - data2[j]);
			}
		}
	}

	//��ʾ���ӵ������ӵ�ӳ������ӵ����ͼ;
	//imshow("���ӵ������ӵ�ӳ������ӵ����ͼ;", lDiff);

	//imwrite("���ӵ������ӵ�ӳ������ӵ����ͼ.png", lDiff, compression_params);


	//////////////////////////////////////////////////////////////////
	//������ֵ���㲿��;
	//�������ӵ���ֵ�ռ�;
	Mat lThre;
	lThre.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = lThre.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//����ÿһ�����ص���ֵ;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);
		uchar* data2 = lDiff.ptr<uchar>(i);
		uchar* data3 = lThre.ptr<uchar>(i);
		for (int j = 0; j < lGray.cols; j++) {
			data3[j] = log(data1[j] * data2[j] + 1) * 5;//�˴�*5��/0.2;
		}
	}

	//��ʾ���ӵ���ֵͼ;
	//imshow("���ӵ���ֵͼ;", lThre);

	///////////////////////////////////////////////////////////////////
	//�ж����ӵ���Ĥ;
	//�������ӵ���Ĥ�ռ�;
	Mat lMask;
	lMask.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = lMask.ptr<uchar>(i);//��ָ��;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//����ÿһ��������ֵ�ж���Ĥ;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = lDiff.ptr<uchar>(i);
		uchar* data2 = lThre.ptr<uchar>(i);
		uchar* data3 = lMask.ptr<uchar>(i);
		for (int j = 0; j < lGray.cols; j++) {
			if (data1[j] > data2[j]) {
				data3[j] = 255;//255
			}
		}
	}

	//��ʾ���ӵ���Ĥͼ;
	//imshow("���ӵ���Ĥͼ;", lMask);
	//д�����ӵ���Ĥͼ;
	imwrite("���ӵ���Ĥͼ.png", lMask, compression_params);

	////////////////////////////////////////////////////
	//����ȫ��MSSIMֵ;
	Scalar mssimV;
	mssimV = getMSSIM(lGray, rwarpedGray);

	printf("\nȫ��MMSIM = %.2lf\n", mssimV.val[0]);

	printf("\n");

	////////////////////////////////////////////////////

	
	//����Բ�ԳƸ�˹Ȩ��ϵ��;
	double sum = 0;
	double gaussWeight[11][11];
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			gaussWeight[i][j] = exp(-(pow(i - 5, 2) + pow(j - 5, 2)) / 4.5);
			sum += gaussWeight[i][j];
		}
	}
	//printf("Բ�ԳƸ�˹Ȩ��ϵ����;\n");
	//�����ܺͣ���һ��;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			gaussWeight[i][j] /= sum;
			//printf("%.2e\t", gaussWeight[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");
	
	//����������Ҫͳ�����йؼ�����SSIM�ܺ�,sum_SSIM,������sum_Num;
	int sum_Num = 0;
	double sum_SSIM = 0;
	
	//���������͹�ϵ����������lGray,rwarpedGray,lMask(0~255),hlMask(0~1)Ϊdouble�������飨1342*1110��;
	int M = lGray.rows, N = lGray.cols;
	//��ջ��С���ⱨ���ʶ�תΪ��άָ�뷽ʽ�Ķѷ���;
	double **lGray_double = new double*[M];
	double **rwarpedGray_double = new double*[M];
	double **lMask_double = new double*[M];
	double **hlMask_double = new double*[M];
	for (int i = 0; i < M; i++){
		lGray_double[i] = new double[N];
		rwarpedGray_double[i] = new double[N];
		lMask_double[i] = new double[N];
		hlMask_double[i] = new double[N];
	}

	//��ʼ��������;
	for (int i = 0; i < M; i++) {
		uchar* data1 = lGray.ptr<uchar>(i);
		uchar* data2 = rwarpedGray.ptr<uchar>(i);
		uchar* data3 = lMask.ptr<uchar>(i);
		uchar* data4 = hlMask.ptr<uchar>(i);
		for (int j = 0; j < N; j++) {
			lGray_double[i][j] = (double)data1[j];
			rwarpedGray_double[i][j] = (double)data2[j];
			lMask_double[i][j] = (double)data3[j];
			hlMask_double[i][j] = (double)data4[j];
		}
	}

	//��д�ؼ�����SSIM;
	//�����uL,uL1(����ul';),oL,oL1(����oL'),oLL1(����oLL');
	for (int i = 5; i < M - 5; i++) {
		for (int j = 5; j < N - 5; j++) {
			if (lMask_double[i][j] == 255.0){//����ǹؼ������;
				//�ؼ��������Ŀ+1;
				sum_Num++;
				//ȷ��11*11���ڵ���ʼ��;
				int l = i - 5;
				int c = j - 5;
				//�����м�ͳ����;
				double uL = 0;
				double uL1 = 0;
				double oL = 0;
				double oL1 = 0;
				double oLL1 = 0;
				//�ȼ���uL��uL1;
				for (int x = l; x < l + 11; x++) {
					for (int y = c; y < c + 11; y++) {
						uL += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * lGray_double[x][y];
						uL1 += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * rwarpedGray_double[x][y];
					}
				}
				//�ټ���oL^2,oL1^2��oLL1;
				for (int x = l; x < l + 11; x++) {
					for (int y = c; y < c + 11; y++) {
						oL += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * pow((lGray_double[x][y] - uL), 2);//�Ѿ���ƽ����;
						oL1 += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * pow((rwarpedGray_double[x][y] - uL1), 2);//�Ѿ���ƽ����;
						oLL1 += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * (lGray_double[x][y] - uL)*(rwarpedGray_double[x][y] - uL1);//�����ɺ���Ҫ��ƽ��;
					}
				}
				
				//�����������������м�ͳ�����ļ���;
				//����ùؼ�������SSIM;
				double SSIM=0, c2 = 6.5, c3 = 58.52;
				double tempv = (pow(uL, 2) + pow(uL1, 2) + c2) * (oL + oL1 + c3);
				SSIM = (2 * uL*uL1 + c2)*(2 * oLL1 + c3) / tempv;

				sum_SSIM += SSIM;

			}
		}
	}
	
	//ɾ����άָ�룬�ͷſռ�;
	for (int i = 0; i < M; i++){
		delete[]	lGray_double[i];
		delete[]	rwarpedGray_double[i];
		delete[]	lMask_double[i];
		delete[]	hlMask_double[i];
	}

	//�м������;
	printf("�ؼ�������; = %d\n", sum_Num);
	//printf("�ؼ���MSSIMֵ�ܺ�; = %.2lf\n", sum_SSIM);

	//CBAl����;
	double CBAl = sum_SSIM / sum_Num;
	printf("CBAl = %.2lf\n", CBAl);
	
	waitKey();

	return 1;
}