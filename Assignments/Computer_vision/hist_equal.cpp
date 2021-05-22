#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace cv;
using namespace std;

#define HISTOGRAM_SIZE 256

Mat calcHist(Mat srcImage) 
{
	int histogram[256] = { 0, };
	int stackHist[256] = { 0, };
	int normalHist[256] = { 0, };
	int equallHist[256] = { 0, };

	int height = srcImage.rows;
	int width = srcImage.cols;
	int imageSize = srcImage.size().width * srcImage.size().height;
	uchar *imageData = srcImage.data;

	// get histogram
	for (int i = 0; i < imageSize; i++)
		++histogram[imageData[i]];

	// cumulative histogram
	stackHist[0] = histogram[0];
	for (int i = 1; i < HISTOGRAM_SIZE; i++) {
		stackHist[i] = stackHist[i - 1] + histogram[i];
	}

	// normalized
	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
		normalHist[i] = ((float)stackHist[i] / imageSize) * (256 - 1);
	}

	// convert form original image to enhanced image
	Mat dstImage = srcImage.clone();
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			dstImage.at<uchar>(y, x) = normalHist[srcImage.at<uchar>(y, x)];
		}
	}

	return dstImage;
}


// ������׷� �׸���
Mat makeHist(Mat *image) {

	// Initialize parameters
	int histSize = 256; // bin size
	float range[] = { 0, 255 };
	const float *ranges[] = { range };

	// Calculate histogram
	MatND hist;
	calcHist(image, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false);

	// Plot the histogram
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++) {
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}

	return histImage;
}


void showHistogram(int *hist, const char *str) {
	// create canvas image
	Mat base = Mat::zeros(256, 256, CV_8UC1);

	// get max value in array
	int maxValue = 0;
	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
		maxValue = (maxValue < hist[i]) ? hist[i] : maxValue;
	}

	// draw line
	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
		int scaledHeight = ((float)hist[i] / maxValue) * 255;
		line(base, Point(i, 255), Point(i, 255 - scaledHeight), Scalar(255), 1, CV_AA);
	}

	imshow(str, base);
}

void sharpen2D(const Mat &image, Mat &result) {
	// Ŀ�� ���� (��� ���� -1���� �ʱ�ȭ)
	Mat kernel(3, 3, CV_32F, Scalar(-1));
	kernel.at<float>(1, 1) = 9.0;
	/* { -1, -1, -1,
		 -1,  9, -1, 
		 -1, -1, -1}
	*/

	filter2D(image, result, image.depth(), kernel);
}

// G(u,v) = H(u,v)F(u,v)
// cutoff : ���ļ� ��� ���� ����
Mat LPF(int m , int n, float cutoff) {
	Mat H(m, n, CV_32F);

	int x0 = n / 2;
	int y0 = m / 2;

	// ������ ���и� �����Ŵ(1). �������� 0����.
	for (int v = -m / 2; v < m / 2; v++) {
		for (int u = -n / 2; u < n / 2; u++) {
			if ((u)*(u)+(v)*(v) < cutoff * cutoff)
				H.at<float>(v + y0, u + x0) = 1.0;
			else
				H.at<float>(v + y0, u + x0) = 0.0;
		}
	}

	// ����Ʈ���� ����ũ ���ϱ�
	//for (int j = 1; j < dft_image.rows - 1; j++) {
	//	for (int i = 1; i < dft_image.cols - 1; i++) {
	//		G.at<float>(j, i) = H.at<float>(j, i) * dft_image.at<float>(j, i);
	//	}
	//}

	//imshow("G(u,v)", G);
	//imshow("H(u,v)", H);
	return H;

}

Mat HPF(int m, int n, float cutoff) {
	Mat H(m, n, CV_32F);

	int x0 = n / 2;
	int y0 = m / 2;

	// ������ ���и� �����Ŵ(1). �������� 0����.
	for (int v = -m / 2; v < m / 2; v++) {
		for (int u = -n / 2; u < n / 2; u++) {
			if (u*u + v*v > cutoff * cutoff)
				H.at<float>(v + y0, u + x0) = 1.0;
			else
				H.at<float>(v + y0, u + x0) = 0.0;
		}
	}

	// �������
	//for (int j = 1; j < dft_image.rows - 1; j++) {
	//	for (int i = 1; i < dft_image.cols - 1; i++) {
	//		G.at<float>(j, i) = H.at<float>(j, i) * dft_image.at<float>(j, i);
	//	}
	//}

	//imshow("G(u,v)", G);
	imshow("H(u,v)", H);

	return H;
}

// High Frequency Emphasis Filter: ������ ���� ����
Mat HFEF(int m, int n, float cutoff, float a, float b) {
	

	Mat H(m, n, CV_32F);
	//Mat G(dft_image.rows, dft_image.cols, dft_image.type());

	int x0 = n / 2;
	int y0 = m / 2;


	// HFE = a + b*HP
	for (int v = -m / 2; v < m / 2; v++) {
		for (int u = -n / 2; u < n / 2; u++) {
			if (u*u + v * v > cutoff * cutoff)
				H.at<float>(v + y0, u + x0) = a + b;
			else
				H.at<float>(v + y0, u + x0) = a + 0;
		}
	}

	// �������
	/*for (int j = 1; j < dft_image.rows - 1; j++) {
		for (int i = 1; i < dft_image.cols - 1; i++) {
			G.at<float>(j, i) = H.at<float>(j, i) * dft_image.at<float>(j, i);
		}
	}*/

	//imshow("G(u,v)", G);
	//imshow("H(u,v)", H);
	return H;
}

void shiftDFT(Mat &planes);

Mat gamma(Mat srcImage, double power)
{
	//double power = 2.2f;

	Mat dstImage;

	srcImage.convertTo(dstImage, CV_32F);
	dstImage /= 255;
	pow(dstImage, power, dstImage);
	dstImage *= 255;
	convertScaleAbs(dstImage, dstImage);
	normalize(dstImage, dstImage, 0, 255, NORM_MINMAX);

	return dstImage;
}

Mat ImageCheck(Mat srcImage)
{
	Mat dstImage;
	int histogram[256] = { 0, };
	int stackHist[256] = { 0, };

	int width = srcImage.cols;
	int imageSize = srcImage.size().width * srcImage.size().height;
	uchar *imageData = srcImage.data;

	// get histogram
	for (int i = 0; i < imageSize; i++)
		++histogram[imageData[i]];

	// cumulative histogram
	stackHist[0] = histogram[0];
	for (int i = 1; i < HISTOGRAM_SIZE; i++) {
		stackHist[i] = stackHist[i - 1] + histogram[i];
	}

	// calculate average
	int averageHist = 0;
	for (int i = 0; i < HISTOGRAM_SIZE; i++) {
		averageHist += stackHist[i];
	}
	averageHist /= 256;


	// noise checking .. .
	if (stackHist[0] >= averageHist || stackHist[255] >= averageHist) {
		medianBlur(srcImage, dstImage, 5);
	}
	else {
		GaussianBlur(srcImage, dstImage, Size(5, 5), 0);
	}

	if (averageHist > 200) {
		dstImage = gamma(dstImage, 2.2f);
	}
	else if (averageHist < 100) {
		dstImage = gamma(dstImage, 0.3f);
	}

	//showHistogram(stackHist, "cumulative histogram");
	
	return dstImage;
}

void findCoin(Mat srcImage, vector<Vec3f> &coins)
{
	HoughCircles(srcImage, coins, CV_HOUGH_GRADIENT, 1.0f, 20, 150, 20, 30, 60);
													// ������ �ػ� (����ũ��/2), �� �� ���� �ּ� �Ÿ�, ĳ�� �ִ� ��谪, ��ǥ �ּ� ����, �ּҿ� �ִ� ������
}

vector<Vec3f> drawCoin(Mat srcImage)
{
	vector<Vec3f> coins;
	/* ���� ��ȯ
	    (�׷��̽�����, �޸� �����, ��� ���(gradient�� ����), ���ش�(1�̸� �̹����� ������ ũ��, 2�� �̹��� ���� ũ��)
		, �ּҰŸ�(���� ������), edge �Ӱ谪(canny edge), �߽� �Ӱ谪(�߽� histogram�� ����), �ּҹ�����, �ִ������)
	*/

	HoughCircles(srcImage, coins, CV_HOUGH_GRADIENT, 2.0f, srcImage.rows / 16, 20, 40, 30, 60);

	cout << "���� ���� : " << coins.size() << endl;

	Mat rgbImage;
	cvtColor(srcImage, rgbImage, CV_GRAY2BGR);

	// ���׶�� �׸���
	for (int i = 0; i < (int)coins.size(); i++) {
		Point center(cvRound(coins[i][0]), cvRound(coins[i][1]));
		int radius = cvRound(coins[i][2]);
		Scalar scalar = Scalar(rand() % 255, rand() % 255, rand() % 255);

		circle(rgbImage, center, 2, scalar, 2);
		circle(rgbImage, center, radius, scalar, 2);
		//putText(rgbImage, to_string(coins[i][2]), center + Point(-10, 20), 1, 1, scalar, 2);
	}

	imshow("coins", rgbImage);

	return coins;
}

double distanceCoins[4] = { 0, };

void Imformation()
{
	int numCoin[4] = { 0, };

	ifstream input;
	input.open("coinstudy.txt");

	double a = 0; int b;
	
	while (!input.eof()) {
		double distance;
		int coin;
		input >> distance >> coin;

		switch (coin)
		{
		case 10:
			coin = 0;
			break;
		case 50:
			coin = 1;
			break;
		case 100:
			coin = 2;
			break;
		default:
			coin = 3;
			break;
		}

		distanceCoins[coin] += distance;
		numCoin[coin]++;
	}


	for (int i = 0; i < 4; i++) {
		distanceCoins[i] /= (double)numCoin[i];
		cout << i << " : "  << distanceCoins[i] << endl;
	}


}

void typeOfCoins(Mat srcImage, vector<Vec3f> coins)
{
	Imformation();

	Mat rgbImage;
	cvtColor(srcImage, rgbImage, CV_GRAY2BGR);

	int CoinValue[4] = { 10, 50, 100, 500 };
	int min = 32767;
	int thisCoin = 0;

	for (int i = 0; i < (int)coins.size(); i++) {

		//Point center = Point(coins[i][0], coins[i][1]);
		float radius = coins[i][2];
		cout << "radius  : " << radius << endl;
		double distance[3] = { 0.0 };
		for (int i = 0; i < 3; i++)
			distance[i] = (distanceCoins[i] - radius) * (distanceCoins[i] - radius);

		for (int i = 0; i < 3; i++) {
			cout << thisCoin << " : now = " << distance[i] << endl;
			if (distance[i] < min) {
				min = distance[i];
				thisCoin = i;
			}
		}
		cout << endl;

		String str;
		Scalar scalar;
		switch (thisCoin)
		{
		case 0:
			scalar = Scalar(051, 102, 051);
			break;
		case 1:
			scalar = Scalar(0,  0, 102);
			break;
		case 2:
			scalar = Scalar(051, 051, 153);
			break;
		default:
			break;
		}

		circle(rgbImage, Point(coins[i][0], coins[i][1]), coins[i][2], scalar, 2);
		putText(rgbImage, to_string(CoinValue[thisCoin]), Point(coins[i][0], coins[i][1]) + Point(-13, 10), 1, 1, scalar, 2);
		min = 32767;
	}

	imshow("What kind of coins", rgbImage);
}


int main() {

	string Image;
	getline(cin, Image);

	// �̹���(����) �ε�
	Mat srcImage = imread(Image, IMREAD_GRAYSCALE);
	if (srcImage.empty())
		return -1;

	// ������ üũ
	Mat checkImage = ImageCheck(srcImage);


	resize(srcImage, srcImage, Size(srcImage.cols / 1.3, srcImage.rows / 1.3), 0, 0, CV_INTER_NN);
	resize(checkImage, checkImage, Size(checkImage.cols / 1.3, checkImage.rows / 1.3), 0, 0, CV_INTER_NN);
	imshow("source image", srcImage);
	imshow("noise check", checkImage);

	// ������׷� ��Ȱȭ
	Mat equalImage = calcHist(checkImage);

	//resize(equalImage, equalImage, Size(equalImage.cols / 1.3, equalImage.rows / 1.3), 0, 0, CV_INTER_NN);
	//imshow("histogram equaliztion", equalImage);


	// ����ȭ
	threshold(equalImage, equalImage, 50, 255, CV_THRESH_BINARY);
	imshow("threshold", equalImage);

	// opening (ħ�� -> ��â)
	dilate(equalImage, equalImage, Mat(), Point(-1, -1), 5);
	imshow("dilate", equalImage);
	erode(equalImage, equalImage, Mat(), Point(-1, -1), 5);
	imshow("erode", equalImage);

	GaussianBlur(equalImage, equalImage, Size(5, 5), 0);

	// ���� ã�� �׸��� �׸���
	vector<Vec3f> coins = drawCoin(equalImage);

	typeOfCoins(checkImage, coins);


	//// ������ ������� ���� Ȯ��
	//Mat padded;
	//int m = getOptimalDFTSize(srcImage.rows);
	//int n = getOptimalDFTSize(srcImage.cols);

	// �ѷ��� 0 �߰��ϱ�
	//copyMakeBorder(srcImage, padded, 0, m - srcImage.rows, 0, n - srcImage.cols, 
	//				BORDER_CONSTANT, Scalar::all(0));

	// Make place for both the complex and the real values.
	/* Ǫ���� ��ȯ�� ����� �����ϴ�.
	   ����� �� ���� ���� ������ ���´�. (one per component)
	   �Է� �̹����� float type���� ��ȯ�ϰ� �ٸ� ä�η� Ȯ���ؼ� ������ ���� �����Ѵ�.*/
	//Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	//Mat complexI;
	//merge(planes, 2, complexI);	// 0���� Ȯ��� �ٸ� ��鿡 �߰�


	//// Discrete Fourier Transform
	//dft(complexI, complexI);		


	// ũ�⸦ ����ϰ� �α� �����Ϸ� ��ȯ
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))

	// ũ�� ���
	//split(complexI, planes);                      // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
	//magnitude(planes[0], planes[1], planes[0]);   // planes[0] = magnitude
	//Mat magI = planes[0];

	//// log(0) �� ������ �ʵ��� +1
	//magI += Scalar::all(1);          
	//// �α� �����Ϸ� ��ȯ
	//log(magI, magI);


	///* �ڸ��� �� ��迭 */
	//
	//// ���� Ȧ�����, ����Ʈ�� �ڸ���
	//magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	///* ������ �̹��� �߾ӿ� ������ Fourier ������ ��и��� �ٽ� ���� */
	//// �̹����� �߽� ��ǥ
	//int cx = magI.cols / 2;
	//int cy = magI.rows / 2;

	//Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - ��2��и�
	//Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right - ��1��и�
	//Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left - ��3��и�
	//Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right - ��4��и�


	//// �����ϱ�
	//Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	//q0.copyTo(tmp);
	//q3.copyTo(q0);
	//tmp.copyTo(q3);

	//q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	//q2.copyTo(q1);
	//tmp.copyTo(q2);

	//// 0�� 1���� ������ ����ȭ�ϱ�
	//normalize(magI, magI, 0, 1, CV_MINMAX);

	//shiftDFT(planes[0]);
	//shiftDFT(planes[1]);


	//HPF(magI, 100.0);
	//HFEF(magI, 100.0, 0.5, 0.75);

	//int height = m;
	//int width = n;

	//Point centre = Point(width / 2, height / 2);

	//Mat Mask(height, width, CV_32F);

	//float a = 0.5;
	//float b = 0.75;

	//int cutoff = 100;
	//int alpha = 10;
	//for (int j = 0; j < height; j++) {
	//	for (int i = 0; i < width; i++) {
	//		double radius = (double)sqrt(pow((i - centre.x), 2.0) + pow((double)(j - centre.y), 2.0));
	//		Mask.at<float>(j, i) = a + b * (float)(1 / (1 + pow((double)(cutoff / radius), (double)(2 * alpha))));
	//	}
	//}


	//Mask = HFEF(m, n, 20, 0.5, 0.75);
	//Mat HPE(magI.rows, magI.cols, magI.type());
	//// �������
	//for (int j = 1; j < magI.rows - 1; j++) {
	//	for (int i = 1; i < magI.cols - 1; i++) {
	//		HPE.at<float>(j, i) = Mask.at<float>(j, i) * magI.at<float>(j, i);
	//	}
	//}

	////���ϱ��� ��Ƽ��
	//planes[0] = planes[0].mul(Mask);
	//planes[1] = planes[1].mul(Mask);

	//shiftDFT(planes[0]);
	//shiftDFT(planes[1]);

	//Mat inverseFFT;
	//merge(planes, 2, inverseFFT);

	////inverseFFT
	//dft(inverseFFT, inverseFFT, DFT_INVERSE | DFT_REAL_OUTPUT);
	//normalize(inverseFFT, inverseFFT, 0, 1, CV_MINMAX);
	//convertScaleAbs(inverseFFT, inverseFFT, 255, 0);

	//imshow("Input Image", srcImage);    // Show the result
	//imshow("spectrum magnitude", magI);
	//imshow("mask", Mask);
	//imshow("HPE", inverseFFT);
	//waitKey();

	//Mat dstImage, dstImage_;
	

	// gamma ����
	//srcImage.convertTo(dstImage, CV_32F);
	//dstImage /= 255;
	//pow(dstImage, power, dstImage);
	//dstImage *= 255;
	//convertScaleAbs(dstImage, dstImage);
	//normalize(dstImage, dstImage, 0, 255, NORM_MINMAX);

	// ������
	/*sharpen2D(dstImage, dstImage_);*/


	// Mat hist_srcImage = makeHist(&srcImage);

	//resize(srcImage, srcImage, Size(srcImage.cols / 1.3, srcImage.rows / 1.3), 0, 0, CV_INTER_NN);
	//resize(dstImage, dstImage, Size(dstImage.cols / 1.3, dstImage.rows / 1.3), 0, 0, CV_INTER_NN);
	//resize(dstImage_, dstImage_, Size(dstImage_.cols / 1.3, dstImage_.rows / 1.3), 0, 0, CV_INTER_NN);

	//imshow("srcImage", srcImage);
	//imshow("gamma", dstImage);
	//imshow("sharpening", dstImage_);
	//imshow("histogram scrImage", hist_srcImage);


	waitKey(0);
	return 0;
}



void shiftDFT(Mat &planes) {
	planes = planes(Rect(0, 0, planes.cols & -2, planes.rows & -2));

	int cx = planes.cols / 2;
	int cy = planes.rows / 2;

	Mat q0(planes, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(planes, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(planes, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(planes, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

}