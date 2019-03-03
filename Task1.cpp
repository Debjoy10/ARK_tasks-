#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/core/core.hpp"
#include <iostream>
#include <cstdlib>
#include<vector>
#include<math.h>
#include<bits/stdc++.h>  
using namespace cv;
using namespace std;                         
#define ROW 1080
#define COL 1920
RNG rng(12345);
int thresh = 100;

Point2f finds(Mat);

bool isUnBlocked(cv::Mat frameb, int row, int col) 
{ 
    // Returns true if the cell is not blocked else false 
    if (frameb.at<uchar>(row,col) == 255) 
        return (true); 
    else
        return (false); 
}

bool isValid(int row, int col) 
{ 
    // Returns true if row number and column number 
    // is in range 
    return (row >= 0) && (row < ROW) && 
           (col >= 0) && (col < COL); 
}

int check_undone1(cv::Mat& frame, cv::Mat& palette)
{
	int c=1;
	for(int i=0 ; i<ROW ; i++)		
	{
		for(int j=0 ; j<COL ; j++)
		{
			if((palette.at<uchar>(i,j)>0)&&(((frame.at<Vec3b>(i,j)[0]>200)&&(frame.at<Vec3b>(i,j)[1]<15)&&(frame.at<Vec3b>(i,j)[2]<15))))
			{
			c=0;		
			}
		}
	}
return c;
}


void find_path(cv::Mat& frame, cv::Mat& palette, vector<Point> &vec, Point p, int &done)
{	
	int step_size = 35;	
	int row = rand() % ROW;
	int col = (rand() % ROW)+(COL-ROW)/2;
	
	int row_vec, col_vec, row1, col1, d;                //Row_vec Col_vec are the points closest to generated pt.
	int distance_sqrd = ROW*ROW + COL*COL;	
	
	vector<Point>::iterator ptr; 
        for (ptr = vec.begin(); ptr < vec.end(); ptr++) 
        {
		d = ((*ptr).x-row)*((*ptr).x-row) + ((*ptr).y-col)*((*ptr).y-col);
		if(d<distance_sqrd)
		{
			row_vec = (*ptr).x;
			col_vec = (*ptr).y;
			distance_sqrd = d;
		}
	}	
	if(row == row_vec) 
	{
		return;
	}
		
	float m = ((float)(col - col_vec))/(row - row_vec);
	float theta = abs(atan(m)); 

	if((row>row_vec)&&(col>col_vec))
	{
		row1 = (int)(row_vec + step_size*cos(theta));
		col1 = (int)(col_vec + step_size*sin(theta));
	}
	else if((row>row_vec)&&(col<col_vec))
	{
		row1 = (int)(row_vec + step_size*cos(theta));
		col1 = (int)(col_vec - step_size*sin(theta));
	}
	else if((row<row_vec)&&(col>col_vec))
	{
		row1 = (int)(row_vec - step_size*cos(theta));
		col1 = (int)(col_vec + step_size*sin(theta));
	}
	else
	{
		row1 = (int)(row_vec - step_size*cos(theta));
		col1 = (int)(col_vec - step_size*sin(theta));
	}
	/*
	Point intm = Point(row, col) - Point(row_vec, col_vec);
	intm = intm/sqrt(intm.x*intm.x+intm.y*intm.y);
	Point p1 =  Point(row, col) + step_size* intm;
	row1 = p1.x;
	col1 = p1.y; */
	if((isValid(row1, col1))&&(isUnBlocked(frame, row1, col1)))
	{
		//cout<<row_vec<<" "<<col_vec<<" & "<<row<<" "<<col<<"---->"<<row1<<" "<<col1<<endl;
		Mat pal(ROW,COL,CV_8UC1,Scalar(0));
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		if((row1 != row_vec)||(col1 != col_vec))
		{
			line( pal, Point(col_vec, row_vec), Point(col1, row1), Scalar(255), 3);
			if(check_undone1(frame, pal)==0)
			{return;}
			line( palette, Point(col_vec, row_vec), Point(col1, row1), color, 3); 
			vec.push_back(Point(row1, col1));	
		}
		if(sqrt((p.x-row1)*(p.x-row1)+(p.y-col1)*(p.y-col1))<70)
		{
			Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			line( palette, Point(p.y, p.x), Point(col1, row1), color, 3);
			vec.push_back(Point(row1, col1));
			done = 1;
		}	
	}
}

void check_undone(cv::Mat& frame, cv::Mat& palette, vector<Point> &vec, int &done,int row, int col)
{	int c = 0;
	for(int i=0 ; i<ROW ; i++)		
	{
		for(int j=0 ; j<COL ; j++)
		{
			if(((palette.at<Vec3b>(i,j)[0]>0)||(palette.at<Vec3b>(i,j)[1]>0)||(palette.at<Vec3b>(i,j)[2]>0))&&(((frame.at<Vec3b>(i,j)[0]>200)&&(frame.at<Vec3b>(i,j)[1]<15)&&(frame.at<Vec3b>(i,j)[2]<15))))
			{
			c++;		
			}
		}
	}
	if(c!=0)
	{
		done = 0;
		vec.clear();
		vec.push_back(Point(row, col));
		for(int i=0 ; i<ROW ; i++)		
		{
			for(int j=0 ; j<COL ; j++)
			{
				palette.at<Vec3b>(i,j)[0]=0;
				palette.at<Vec3b>(i,j)[1]=0;
				palette.at<Vec3b>(i,j)[2]=0;
			}
		}
		cout<<"Remove";	
	}
}			


int main() 
{ 
	namedWindow("win",0);
 	VideoCapture Video("dynamic_obstacles.mp4");
 	Mat frame;
	Mat start(ROW,COL,CV_8UC1,Scalar(0));
        Mat finish(ROW,COL,CV_8UC1,Scalar(0));
	Mat framebw(ROW,COL,CV_8UC1,Scalar(255));
	Mat palette(ROW,COL,CV_8UC3,Scalar(0,0,0));
	int done =0;
	Video>>frame;
	for(int i=0 ; i<ROW ; i++)		
	{
		for(int j=0 ; j<COL ; j++)
		{
			if((frame.at<Vec3b>(i,j)[0]<200)&&(frame.at<Vec3b>(i,j)[1]<200)&&(frame.at<Vec3b>(i,j)	[2]>100))
			{
				start.at<uchar>(i,j)=255;
				frame.at<Vec3b>(i,j)={255,255,255};
			}	

	
			if((frame.at<Vec3b>(i,j)[0]<200)&&(frame.at<Vec3b>(i,j)[1]>100)&&(frame.at<Vec3b>(i,j)	[2]<200))
			{
				finish.at<uchar>(i,j)=255;
				frame.at<Vec3b>(i,j)={255,255,255};
			}
		}
	}
	Point2f p1=finds(start);
	int row0 = (int)p1.y;
	int col0 = (int)p1.x;
	
	Point2f p2=finds(finish);
	int rowf = (int)p2.y;
	int  colf = (int)p2.x; 
        //cout<<row0<<" "<<col0<<endl<<rowf<<" "<<colf;
	vector<Point> stc;
	stc.push_back(Point(row0, col0));
	
	Point dest = Point(rowf, colf);

	/*
	imshow("win",frame);
 	waitKey(0);
	imshow("win",start);
 	waitKey(0);
	imshow("win",finish);
 	waitKey(0);	*/
	
	while(1)
	{			
		int flag = 0;
		Video>>frame;
		
			
		check_undone(frame, palette, stc, done, row0, col0);

		if(done == 1)
		{
			for(int i=0 ; i<ROW ; i++)		
			{
				for(int j=0 ; j<COL ; j++)
				{
					if((palette.at<Vec3b>(i,j)[0]!=0)||(palette.at<Vec3b>(i,j)[1]!=0)||(palette.at<Vec3b>(i,j)[2]!=0) )
					{
						frame.at<Vec3b>(i,j)=palette.at<Vec3b>(i,j);
					}
				}
			}
		}
		while((flag<8)&&(done==0))
		{
		flag++;
		for(int i=0 ; i<ROW ; i++)		
		{
			for(int j=0 ; j<COL ; j++)
			{
				framebw.at<uchar>(i,j) = 255;
			}
		}

		for(int i=0 ; i<ROW ; i++)		
		{
			for(int j=0 ; j<COL ; j++)
			{
				if((frame.at<Vec3b>(i,j)[0]<10)&&(frame.at<Vec3b>(i,j)[1]<10)&&(frame.at<Vec3b>(i,j)	[2]<10))
				{					
					framebw.at<uchar>(i,j)=0;
				}	
	
		
				if((frame.at<Vec3b>(i,j)[0]>180)&&(frame.at<Vec3b>(i,j)[1]<30)&&(frame.at<Vec3b>(i,j)	[2]<30))
				{
					for(int a=i-2; a<=i+2 ; a++){
					for(int	b=j-2; b<=j+2; b++){				
					framebw.at<uchar>(a,b)=0;}}
				}
			}
		}
		
		find_path(framebw, palette, stc, dest, done);
		for(int i=0 ; i<ROW ; i++)		
		{
			for(int j=0 ; j<COL ; j++)
			{
		if((palette.at<Vec3b>(i,j)[0]!=0)||(palette.at<Vec3b>(i,j)[1]!=0)||(palette.at<Vec3b>(i,j)[2]!=0) )
				{
					frame.at<Vec3b>(i,j)=palette.at<Vec3b>(i,j);
				}
			}
		}
		} 
		
	/*	if(done==1)
		{
			waitKey(0);			
			break;
		}
		vector<Point>::iterator ptr; 
      
		cout << "The vector elements are : "; 
		for (ptr = stc.begin(); ptr < stc.end(); ptr++) 
        	cout << *ptr << " "; 
		cout<<endl;
*/		//cout<<"frame";
	imshow("win",frame);
	waitKey(1);
	} 
		
	return(0); 
}

Point2f finds(Mat img)
{
    Mat canny_output;
    Canny( img, canny_output, thresh, thresh*2 );
    vector<vector<Point> > contours;
    findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Point2f>centers( contours.size() );
    vector<float>radius( contours.size() );
    int max_radius=0,maxR_index;

    for(int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( contours[i], contours_poly[i], 3, true );
        minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
	if(radius[i]>max_radius) 
	{	
		max_radius=radius[i];
		maxR_index=i;
	}
    }
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
	
    
    Scalar color = Scalar(255,255,255);
    circle( drawing, centers[maxR_index], (int)radius[maxR_index], color, 1 );
    
    return centers[maxR_index];
   
}

