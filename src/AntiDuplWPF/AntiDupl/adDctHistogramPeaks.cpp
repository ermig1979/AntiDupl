#include "adPerformance.h"
#include "adDctHistogramPeaks.h"

#include <vector>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include "Simd/SimdLib.hpp"

#ifdef _DEBUG
#pragma comment (lib, "opencv_core2413d.lib")
#pragma comment (lib, "opencv_highgui2413d.lib")
#pragma comment (lib, "libtiffd.lib")
#pragma comment (lib, "libjpegd.lib")
#pragma comment (lib, "libpngd.lib")
#pragma comment (lib, "libjasperd.lib")
#pragma comment (lib, "IlmImfd.lib")
#pragma comment (lib, "zlibd.lib")
#pragma comment (lib, "opencv_imgproc2413d.lib")
#else
#pragma comment (lib, "opencv_core2413.lib")
#pragma comment (lib, "opencv_highgui2413.lib")
#pragma comment (lib, "libtiff.lib")
#pragma comment (lib, "libjpeg.lib")
#pragma comment (lib, "libpng.lib")
#pragma comment (lib, "libjasper.lib")
#pragma comment (lib, "IlmImf.lib")
#pragma comment (lib, "zlib.lib")
#pragma comment (lib, "opencv_imgproc2413.lib")
#endif

using namespace cv;
using namespace std; 

namespace ad
{
		int TDctHistogramPeaks::CalcDctHistogramPeaks( const TView & view )
		{
			Mat img(view);

			int dct_rows;
			int dct_cols;
			bool makePad = false;
			if (img.rows %8 != 0) 
			{
				dct_rows = img.rows + 8 - img.rows % 8;
				makePad = true;
			}
			else
				dct_rows = img.rows;

			if (img.cols %8 != 0)
			{
				dct_cols = img.cols + 8 - img.cols % 8;
				makePad = true;
			}
			else
				dct_cols = img.cols;

			Mat padded;  //expand input image to optimal size
			if (makePad)
			{
				copyMakeBorder(img, padded, 0, dct_rows - img.rows, 0, dct_cols - img.cols, BORDER_CONSTANT, Scalar::all(0));
				//cv::FileStorage filePadded("padded.xml", cv::FileStorage::WRITE);
				//filePadded << "matName" << padded;
				//filePadded.release();
			}
			else
				padded = img;

			Mat BGR2YCrCb_image;
			cvtColor(padded, BGR2YCrCb_image, COLOR_BGR2YCrCb);

			vector<Mat> bgr_planes;
			split( BGR2YCrCb_image, bgr_planes );

			Mat y_ = bgr_planes[0];

			 int w = y_.cols;
			 int h = y_.rows;
			 double n = w*h/64;

			 //разбиваем на квадратики 8x8
			 //int sz[] = {h / 8, 8, w / 8, 8};
			//	Mat map(4, sz, CV_8U);

			 //Mat Y = y.reshape(120,8);


				 // Cloning the image to another for visualization later, if you do not want to visualize the result just comment every line related to visualization 
				//cv::Mat maskImg = y.clone();
				// Checking if the clone image was cloned correctly 
				//if(!maskImg.data || maskImg.empty())
				 //   std::cerr << "Problem Loading Image" << std::endl;

			std::vector<cv::Mat> blocks;
			int colDivisor = w / 8;
			int rowDivisor = h / 8;

			
		

			// check if divisors fit to image dimensions
			if (y_.cols % colDivisor == 0 && y_.rows % rowDivisor == 0)
			{
				for(int x = 0; x < y_.rows; x += y_.rows / rowDivisor)
				{
					for (int y = 0; y < y_.cols; y += y_.cols / colDivisor)
					{

						blocks.push_back(y_(cv::Rect(y, x, (y_.cols / colDivisor), (y_.rows / rowDivisor))));
					}
				}
			}
			else
				return -1;


				vector<Mat> qDCT;

				for (int i = 0; i < blocks.size(); i++)
				{
					Mat dct_result;
		

	//If matrix is of type CV_8U then use Mat.at<uchar>(y,x).
	//If matrix is of type CV_8S then use Mat.at<schar>(y,x).
	//If matrix is of type CV_16U then use Mat.at<ushort>(y,x).
	//If matrix is of type CV_16S then use Mat.at<short>(y,x).
	//If matrix is of type CV_32S then use Mat.at<int>(y,x).
	//If matrix is of type CV_32F then use Mat.at<float>(y,x).
	//If matrix is of type CV_64F then use Mat.at<double>(y,x).

					blocks[i].convertTo(blocks[i], CV_32F);			

					dct(blocks[i], dct_result);

					//if(dct_result.empty()) 
					//	throw std::runtime_error("dct_result is empty!");
					//cout << dct_result.type() << endl;
					qDCT.push_back(dct_result);
				}

			Mat row0;
			Mat row1;
			Mat row2;
			Mat row3;
			Mat row4;
			Mat row5;
			Mat row6;
			Mat row7;
			for each (Mat dct in qDCT)
			{
				//a(Range(0, a.rows), Range(a.cols - 2, a.cols)).copyTo(e);
				row0.push_back(dct.row(0));
				row1.push_back(dct.row(1));
				row2.push_back(dct.row(2));
				row3.push_back(dct.row(3));
				row4.push_back(dct.row(4));
				row5.push_back(dct.row(5));
				row6.push_back(dct.row(6));
				row7.push_back(dct.row(7));
			}


			//	Mat meanArray;
			//Scalar outMean = mean(qDCT[0]);
			Mat row_mean;

			cv::Mat meanMat;

			reduce(row0, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row1, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row2, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row3, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row4, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row5, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row6, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
			reduce(row7, row_mean, 0, CV_REDUCE_AVG);
			meanMat.push_back(row_mean);
		

			vector<Mat> subAll;
			Mat sub;
			//subtract(qDCT[0].row(0), row_mean, sub);
			//subtract(qDCT[0], meanMat, sub);
			for each (Mat dct in qDCT)
			{
				subtract(dct, meanMat, sub);
				//Mat conv;
				sub.convertTo(sub, CV_32S);
				subAll.push_back(sub);
			}


			//}
			//catch (cv::Exception& e) 
			//{
			//	cerr << "Error Reason: " << e.msg << endl;
			//	// nothing more we can do
			//	exit(1);
			//}

		
			int peakCount = 0;
		
			//цикл по первым трем числам блоков
			for (int k = 0; k < 4; k++)
			{

				vector<int> data;
				for each (Mat sub in subAll)
				{
					data.push_back(sub.at<int>(0,k));
				}

				int max = *std::max_element(data.begin(), data.end());
				int min = *std::min_element(data.begin(), data.end());

			
				/// Establish the number of bins
				int histSize = max - min;
				/// Set the ranges ( for B,G,R) )
				float range[] = { min, max } ;
				const float* histRange = { range };
				bool uniform = true; 
				bool accumulate = false;
				Mat b_hist;

		//cv::Mat converted(1, data.size(), CV_32S, data);
				Mat converted(data);
				converted.convertTo(converted, CV_32F);
				//cout << converted << endl;

		//&bgr_planes[0]: The source array(s)
		//1: The number of source arrays (in this case we are using 1. We can enter here also a list of arrays )
		//0: The channel (dim) to be measured. In this case it is just the intensity (each array is single-channel) so we just write 0.
		//Mat(): A mask to be used on the source array ( zeros indicating pixels to be ignored ). If not defined it is not used
		//b_hist: The Mat object where the histogram will be stored
		//1: The histogram dimensionality.
		//histSize: The number of bins per each used dimension
		//histRange: The range of values to be measured per each dimension
		//uniform and accumulate: The bin sizes are the same and the histogram is cleared at the beginning.
				calcHist( &converted, 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

				if (b_hist.size().width == 1 && b_hist.size().height == 1)
					return 0;

				double minHist, maxHist;
				cv::minMaxLoc(b_hist, &minHist, &maxHist);
				double s = cv::sum( b_hist )[0];
				//cout << b_hist << endl;


				normalize(b_hist, b_hist, 1, 0, NORM_L1, -1, Mat() );
				//normalize(b_hist, b_hist);
			

				Mat planes2[] = {Mat_<float>(b_hist), Mat::zeros(b_hist.size(), CV_32F)};
				Mat complexI2;
				merge(planes2, 2, complexI2);         // Add to the expanded another plane with zeros
				dft(complexI2, complexI2, DFT_COMPLEX_OUTPUT);   
				split(complexI2, planes2);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
				magnitude(planes2[0], planes2[1], planes2[0]);// planes[0] = magnitude
				Mat magI2 = planes2[0];

			

		
				//try
				//{
					//Mat rotated(b_hist.size(), b_hist.type());
					//Mat rotated(b_hist);
					cv::Mat rotated = cv::Mat::zeros(magI2.size(), magI2.type());
					//int halfWidth = b_hist.rows / 2;
					magI2(cv::Rect(0, 0, magI2.cols, magI2.rows / 2)).copyTo(rotated(cv::Rect(0, magI2.rows / 2, magI2.cols, magI2.rows / 2)));
					magI2(cv::Rect(0,  magI2.rows / 2, magI2.cols, magI2.rows / 2)).copyTo(rotated(cv::Rect(0, 0, magI2.cols, magI2.rows / 2)));
					//cout << rotated << endl;
					//b_hist(cv::Rect(b_hist.cols / 2, 0, b_hist.cols / 2, b_hist.rows)).copyTo(rotated(cv::Rect(0, 0, b_hist.cols / 2, b_hist.rows)));
					//Mat rotated(Range(rowRange), Range(colRange))
					//cv::Mat out = cv::Mat::zeros(b_hist.size(), b_hist.type());
					//b_hist(cv::Rect(0,10, b_hist.cols, b_hist.rows-10)).copyTo(out(cv::Rect(0,0,b_hist.cols,b_hist.rows-10)));
					cv::Mat shifted = cv::Mat::zeros(rotated.size(), rotated.type());
					rotated(cv::Rect(0, 1, magI2.cols, magI2.rows - 1)).copyTo(shifted(cv::Rect(0, 0, magI2.cols, magI2.rows - 1)));
					rotated(cv::Rect(0, magI2.rows - 1, magI2.cols, 1)).copyTo(shifted(cv::Rect(0, magI2.rows - 1, magI2.cols, 1)));

					Mat subtracted;
					subtract(shifted, rotated, subtracted);
					//cout << subtracted << endl;

		
					vector<int> indices;
					for (int y = 0; y < subtracted.rows - 1; y += 1)
					{
						//cout <<  (float)subtracted.at<float>(y,0) << ",";
						if (subtracted.at<float>(y,0) > 0 && subtracted.at<float>(y+1,0) < 0)
						{
							//cout <<  (float)subtracted.at<float>(y,0) << ">" << (float)subtracted.at<float>(y+1,0) << "(" << y << "), ";
							indices.push_back(y + 1);
						}
					}

					for (int i = 0; i < indices.size(); i++)
					{
						if ((float)rotated.at<float>(indices[i],0) > 0.5)
							peakCount++;
					}

				//}
				//catch (cv::Exception& e) 
				//{
				//	cerr << "Error Reason: " << e.msg << endl;
				//	// nothing more we can do
				//	exit(1);
				//}
			}

			return peakCount;

			
		}
}