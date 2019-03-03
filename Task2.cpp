#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/core/core.hpp"
#include <iostream>
#include <cstdlib>
#include<vector>
#include<math.h>
#include "opencv2/calib3d/calib3d.hpp"
#include<bits/stdc++.h>  
using namespace cv;
using namespace std;

Mat img_left;
Mat img_right;
Mat img_disp;

bool valid(Mat& img, int x, int y) {
  if (x >= 0 && x < img.cols && y >= 0 && y < img.rows)
    return true;
}

int getCorresPoint2f(Point2f p, Mat& img1, Mat& img2, int ndisp) {
  int w = 7;
  long minCost = 1e9;
  int chosen_i = 0;
  for (int i = p.x - ndisp; i <= p.x; i++) {
    long error = 0;
    for (int k = -w; k <= w; k++) {
      for (int j = -w; j <= w; j++) {
        if (valid(img1, p.x+k, p.y+j) && valid(img2, i+k, p.y+j)) {
          error += abs(img1.at<uchar>(p.y+j,p.x+k) - 
                       img2.at<uchar>(p.y+j,i+k));
        }
      }
    }
    if (error < minCost) {
      minCost = error;
      chosen_i = i;
    }
  }
  return chosen_i;
}
void computeDisparityMap(int ndisp) {
  img_disp = Mat(img_left.rows, img_left.cols, CV_8UC1, Scalar(0));
  for (int i = ndisp; i < img_left.cols-ndisp; i++) {
    for (int j = ndisp; j < img_left.rows-ndisp; j++) {
      int right_i = getCorresPoint2f(Point2f(i,j), img_left, img_right, ndisp);
      int disparity = abs(i - right_i);
      img_disp.at<uchar>(j,i) = disparity * (255. / ndisp);
    }
  }
}


int main()
{
img_left = imread("left_image.jpg", 0);
img_right = imread("right_image.jpg", 0);

double FM_RANSAC = 0.1;

int point_count = 41;

vector<Point2f> arr_left(point_count);

arr_left[1] = Point2f(224,55);
arr_left[2] = Point2f(430, 33);
arr_left[3] = Point2f(440, 78);
arr_left[4] = Point2f(420, 303);
arr_left[5] = Point2f(403, 327);
arr_left[6] = Point2f(445, 306);
arr_left[7] = Point2f(449, 273);
arr_left[8] = Point2f(463, 94);
arr_left[9] = Point2f(469, 105);
arr_left[10] = Point2f(572, 117);
arr_left[11] = Point2f(616, 78);
arr_left[12] = Point2f(590, 354);
arr_left[13] = Point2f(546, 393);
arr_left[14] = Point2f(487, 382);
arr_left[15] = Point2f(242, 157);
arr_left[16] = Point2f(399, 170);
arr_left[17] = Point2f(251, 200);
arr_left[18] = Point2f(274, 203);
arr_left[19] = Point2f(284, 204);
arr_left[20] = Point2f(307, 208);
arr_left[21] = Point2f(319, 210);
arr_left[22] = Point2f(344, 211);
arr_left[23] = Point2f(354, 212);
arr_left[24] = Point2f(383, 212);
arr_left[25] = Point2f(263, 341);
arr_left[26] = Point2f(276, 346);
arr_left[27] = Point2f(286, 348);
arr_left[28] = Point2f(300, 350);
arr_left[29] = Point2f(310, 351);
arr_left[30] = Point2f(327, 351);
arr_left[31] = Point2f(338, 353);
arr_left[32] = Point2f(356, 349);
arr_left[33] = Point2f(167, 170);
arr_left[34] = Point2f(163, 193);
arr_left[35] = Point2f(192, 195);
arr_left[36] = Point2f(197, 170);
arr_left[37] = Point2f(135, 171);
arr_left[38] = Point2f(156, 171);
arr_left[39] = Point2f(154, 192);
arr_left[40] = Point2f(117, 62);
arr_left[0] = Point2f(588, 16);

vector<Point2f> arr_right(point_count);

arr_right[1] = Point2f(224, 55);
arr_right[2] =Point2f(430, 35);
arr_right[3] =Point2f(440, 77);
arr_right[4] =Point2f(421, 303);
arr_right[5] =Point2f(403, 329);
arr_right[6] =Point2f(444, 306);
arr_right[7] =Point2f(449, 273);
arr_right[8] =Point2f(462, 93);
arr_right[9] =Point2f(470, 105);
arr_right[10] =Point2f(572, 117);
arr_right[11] =Point2f(616, 77);
arr_right[12] =Point2f(591, 354);
arr_right[13] =Point2f(547, 393);
arr_right[14] =Point2f(487, 383);
arr_right[15] =Point2f(242, 158);
arr_right[16] =Point2f(400, 171);
arr_right[17] =Point2f(251, 200);
arr_right[18] =Point2f(274, 203);
arr_right[19] =Point2f(283, 205);
arr_right[20] =Point2f(307, 208);
arr_right[21] =Point2f(318, 209);
arr_right[22] =Point2f(343, 210);
arr_right[23] =Point2f(354, 210);
arr_right[24] =Point2f(383, 210);
arr_right[25] =Point2f(263, 338);
arr_right[26] =Point2f(275, 345);
arr_right[27] =Point2f(286, 349);
arr_right[28] =Point2f(300, 354);
arr_right[29] =Point2f(310, 356);
arr_right[30] =Point2f(329, 353);
arr_right[31] =Point2f(339, 353);
arr_right[32] =Point2f(355, 350);
arr_right[33] =Point2f(168, 170);
arr_right[34] =Point2f(164, 193);
arr_right[35] =Point2f(192, 195);
arr_right[36] =Point2f(197, 170);
arr_right[37] =Point2f(135, 173);
arr_right[38] =Point2f(156, 172);
arr_right[39] =Point2f(154, 194);
arr_right[40] =Point2f(119, 63);
arr_right[0] =Point2f(587, 17);

Mat fundamental_matrix = findFundamentalMat(arr_right, arr_left, FM_RANSAC, 3, 0.99);

Mat H1;
Mat H2;

//bool T = stereoRectifyUncalibrated(arr_right, arr_left, fundamental_matrix, img_left.size(), H1,H2);

Mat H = findHomography(arr_right, arr_left);
cout << "H:\n" << H << endl;

H1=H;
H2=H;

Mat out_left = img_left;
warpPerspective(img_left, out_left, H2, img_left.size());

Mat out_right = img_right;
warpPerspective(img_right, out_right, H1, img_right.size());

int rows,cols;
rows = img_left.rows;
cols = img_left.cols;
Mat out1(rows, cols,CV_8UC1, Scalar(0));
Mat out2;
resize(out_right, out1, out1.size(), 0, 0);
resize(out_left, out2, out1.size(), 0, 0);
//cv::rotate(out1, out1, cv::ROTATE_90_COUNTERCLOCKWISE);
   // Display images
    //imshow("Source Image", img_right);
    //imshow("Destination Image", img_left);
    //imshow("Warped Source Image1", out_left);
    //imshow("Warped Source Image2", out1);	
 
  img_right = out1;
  img_left = out2;
  computeDisparityMap(20);
  namedWindow("IMG-LEFT", 1);
  namedWindow("IMG-RIGHT", 1);
  imshow("IMG-LEFT", img_left);
  imshow("IMG-RIGHT", img_right);
  imshow("IMG-DISP", img_disp);
  waitKey(0);   
  return 0;
}
