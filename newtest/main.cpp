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
	s1.convertTo(s1, CV_32F);  // 不能在8位矩阵上做平方运算
	s1 = s1.mul(s1);           // |I1 - I2|^2

	Scalar s = sum(s1);         // 叠加每个通道的元素;

	double sse = s.val[0] + s.val[1] + s.val[2]; // 叠加所有通道;

	if (sse <= 1e-10) // 如果值太小就直接等于0
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
	i1.convertTo(I1, d);           // 不能在单字节像素上进行计算，范围不够。
	i2.convertTo(I2, d);

	Mat I2_2 = I2.mul(I2);        // I2^2
	Mat I1_2 = I1.mul(I1);        // I1^2
	Mat I1_I2 = I1.mul(I2);        // I1 * I2

								   /***********************初步计算 ******************************/

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

	///////////////////////////////// 公式 ////////////////////////////////
	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

	Mat ssim_map;
	divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

	Scalar mssim = mean(ssim_map); // mssim = ssim_map的平均值
	return mssim;
}

int main(int argc, char ** argv) {

	printf("Welcome to MSSIM!\n");

	//写入配置;
	vector<int>compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);

	//输入彩色图;
	Mat lColor = imread("view1.png", 2 | 4);
	Mat rColor = imread("view5.png", 2 | 4);
	//输入视差图;
	Mat lDisp = imread("disp1.png", 2 | 4);
	Mat rDisp = imread("disp5.png", 2 | 4);

	//申明新灰度图空间;
	Mat lGray, rGray;
	lGray.create(lColor.rows, lColor.cols, CV_8UC1);//CV_8UC1
	rGray.create(rColor.rows, rColor.cols, CV_8UC1);//CV_8UC1

	//测试输入彩色图(测试通过);
	//imshow("原始左视点图;", lColor);
	//imshow("原始右视点图;", rColor);

	//测试输入视差图(测试通过);
	//imshow("原始左视差图;", lDisp);
	//imshow("原始右视差图;", rDisp);

	//转换成灰度图;
	cvtColor(lColor, lGray, CV_RGB2GRAY);
	cvtColor(rColor, rGray, CV_RGB2GRAY);

	//测试转换的灰度图(测试通过);
	//imshow("左视点灰度图;", lGray);
	//imshow("右视点灰度图;", rGray);

	imwrite("左视点灰度图.png", lGray, compression_params);

	//将右视点灰度图通过视差图映射到左视点;
	//申明映射的新左视点图空间;
	Mat rwarpedGray;
	rwarpedGray.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//z-buffer,打算先保存一个位置掩模;
	//申明映射的新左视点图位置掩模;
	Mat rwarpedLocal, rwarpedLocal_Value;
	rwarpedLocal.create(lGray.rows, lGray.cols, CV_16UC1);//CV_16UC1
	rwarpedLocal_Value.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		ushort* data = rwarpedLocal.ptr<ushort>(i);//行指针;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = rwarpedLocal_Value.ptr<uchar>(i);//行指针;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//生成位置掩模;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = rDisp.ptr<uchar>(i);//右视点视差图;
		ushort* data2 = rwarpedLocal.ptr<ushort>(i);//位置;
		uchar* data3 = rwarpedLocal_Value.ptr<uchar>(i);//映射过去的视差值;
		for (int j = 0, k = 0; j < lGray.cols; j++) {//k代表映射后的目标像素点;
			k = j + data1[j];
			if (k >= 0 && k < lGray.cols) {
				if (data3[k] < data1[j]) {//目标像素点存储的视差值<原始像素点视差值;
					data2[k] = j;//存储位置;
					data3[k] = data1[j];//存储更大的视差值;
				}
			}
		}
	}

	//映射过程;
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

	//修复裂纹空洞;
	for (int i = 0; i < rwarpedGray.rows; i++) {
		uchar* data = rwarpedGray.ptr<uchar>(i);
		for (int j = 1; j < rwarpedGray.cols - 1; j++) {
			if (data[j] == 0) {//此处205值为Mat类型默认值
				if (data[j - 1] != 0 && data[j + 1] != 0)//该空洞中只有一个空白像素;
					data[j] = (data[j - 1] + data[j + 1]) / 2;
				if (data[j + 1] == 0 && data[j - 1] != 0 && data[j + 2] != 0) {//该空洞中只有两个空白像素;
					data[j] = data[j + 1] = (data[j - 1] + data[j + 2]) / 2;
				}
			}
		}
	}

	imwrite("映射的新左视点图.png", rwarpedGray, compression_params);

	//显示映射的新左视点图;
	//imshow("映射的新左视点图;", rwarpedGray);

	//接下来要统计Hl区域;
	//统计左边的全0区域，则这些全0区域即为Hl;
	//即为backward warping;

	//申明Hl掩模空间;
	Mat hlMask;
	hlMask.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = hlMask.ptr<uchar>(i);//行指针;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//计算Hl;
	for (int i = 0; i < rwarpedGray.rows; i++) {
		uchar* data1 = rwarpedGray.ptr<uchar>(i);
		uchar* data2 = hlMask.ptr<uchar>(i);
		for (int j = 0; j < rwarpedGray.cols; j++) {//从左开始遍历，遇到第一个非0中断循环;
			if (data1[j] == 0) {
				data2[j] = 1;
			}
			else
				break;
		}
	}

	//测试Hl图(测试通过);
	//imshow("Hl图;", hlMask);

	//申明左视点与右视点映射的左视点差异图的空间El;
	Mat lDiff;
	lDiff.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = lDiff.ptr<uchar>(i);//行指针;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//左视点与右视点映射的左视点作差;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = lGray.ptr<uchar>(i);
		uchar* data2 = rwarpedGray.ptr<uchar>(i);
		uchar* data3 = lDiff.ptr<uchar>(i);
		uchar* data4 = hlMask.ptr<uchar>(i);
		for (int j = 0; j < lGray.cols; j++) {//注意这里要除去Hl空间;
			if (data4[j] != 1) {
				data3[j] = abs(data1[j] - data2[j]);
			}
		}
	}

	//显示左视点与右视点映射的左视点差异图;
	//imshow("左视点与右视点映射的左视点差异图;", lDiff);

	//imwrite("左视点与右视点映射的左视点差异图.png", lDiff, compression_params);


	//////////////////////////////////////////////////////////////////
	//进入阈值计算部分;
	//申明左视点阈值空间;
	Mat lThre;
	lThre.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = lThre.ptr<uchar>(i);//行指针;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//计算每一个像素的阈值;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data1 = lDisp.ptr<uchar>(i);
		uchar* data2 = lDiff.ptr<uchar>(i);
		uchar* data3 = lThre.ptr<uchar>(i);
		for (int j = 0; j < lGray.cols; j++) {
			data3[j] = log(data1[j] * data2[j] + 1) * 5;//此处*5即/0.2;
		}
	}

	//显示左视点阈值图;
	//imshow("左视点阈值图;", lThre);

	///////////////////////////////////////////////////////////////////
	//判断左视点掩膜;
	//申明左视点掩膜空间;
	Mat lMask;
	lMask.create(lGray.rows, lGray.cols, CV_8UC1);//CV_8UC1

	//init to 0;
	for (int i = 0; i < lGray.rows; i++) {
		uchar* data = lMask.ptr<uchar>(i);//行指针;
		for (int j = 0; j < lGray.cols; j++)
			data[j] = 0;
	}

	//根据每一个像素阈值判断掩膜;
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

	//显示左视点掩膜图;
	//imshow("左视点掩膜图;", lMask);
	//写入左视点掩膜图;
	imwrite("左视点掩膜图.png", lMask, compression_params);

	////////////////////////////////////////////////////
	//计算全局MSSIM值;
	Scalar mssimV;
	mssimV = getMSSIM(lGray, rwarpedGray);

	printf("\n全局MMSIM = %.2lf\n", mssimV.val[0]);

	printf("\n");

	////////////////////////////////////////////////////

	
	//计算圆对称高斯权重系数;
	double sum = 0;
	double gaussWeight[11][11];
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			gaussWeight[i][j] = exp(-(pow(i - 5, 2) + pow(j - 5, 2)) / 4.5);
			sum += gaussWeight[i][j];
		}
	}
	//printf("圆对称高斯权重系数表;\n");
	//除以总和，归一化;
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			gaussWeight[i][j] /= sum;
			//printf("%.2e\t", gaussWeight[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");
	
	//下述计算需要统计所有关键区域SSIM总和,sum_SSIM,点总数sum_Num;
	int sum_Num = 0;
	double sum_SSIM = 0;
	
	//因数据类型关系，重新申明lGray,rwarpedGray,lMask(0~255),hlMask(0~1)为double类型数组（1342*1110）;
	int M = lGray.rows, N = lGray.cols;
	//因栈大小问题报错，故而转为二维指针方式的堆方法;
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

	//初始化，复制;
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

	//重写关键区域SSIM;
	//输出：uL,uL1(代表ul';),oL,oL1(代表oL'),oLL1(代表oLL');
	for (int i = 5; i < M - 5; i++) {
		for (int j = 5; j < N - 5; j++) {
			if (lMask_double[i][j] == 255.0){//如果是关键区域点;
				//关键区域点数目+1;
				sum_Num++;
				//确定11*11窗口的起始点;
				int l = i - 5;
				int c = j - 5;
				//申明中间统计量;
				double uL = 0;
				double uL1 = 0;
				double oL = 0;
				double oL1 = 0;
				double oLL1 = 0;
				//先计算uL和uL1;
				for (int x = l; x < l + 11; x++) {
					for (int y = c; y < c + 11; y++) {
						uL += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * lGray_double[x][y];
						uL1 += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * rwarpedGray_double[x][y];
					}
				}
				//再计算oL^2,oL1^2和oLL1;
				for (int x = l; x < l + 11; x++) {
					for (int y = c; y < c + 11; y++) {
						oL += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * pow((lGray_double[x][y] - uL), 2);//已经是平方了;
						oL1 += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * pow((rwarpedGray_double[x][y] - uL1), 2);//已经是平方了;
						oLL1 += /*hlMask_double[x][y] **/ gaussWeight[x - l][y - c] * (lGray_double[x][y] - uL)*(rwarpedGray_double[x][y] - uL1);//求和完成后不需要开平方;
					}
				}
				
				//上述步骤完成了五个中间统计量的计算;
				//计算该关键区域点的SSIM;
				double SSIM=0, c2 = 6.5, c3 = 58.52;
				double tempv = (pow(uL, 2) + pow(uL1, 2) + c2) * (oL + oL1 + c3);
				SSIM = (2 * uL*uL1 + c2)*(2 * oLL1 + c3) / tempv;

				sum_SSIM += SSIM;

			}
		}
	}
	
	//删除二维指针，释放空间;
	for (int i = 0; i < M; i++){
		delete[]	lGray_double[i];
		delete[]	rwarpedGray_double[i];
		delete[]	lMask_double[i];
		delete[]	hlMask_double[i];
	}

	//中间量检测;
	printf("关键点总数; = %d\n", sum_Num);
	//printf("关键点MSSIM值总和; = %.2lf\n", sum_SSIM);

	//CBAl计算;
	double CBAl = sum_SSIM / sum_Num;
	printf("CBAl = %.2lf\n", CBAl);
	
	waitKey();

	return 1;
}