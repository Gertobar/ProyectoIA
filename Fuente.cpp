
#include <iostream>
#include <cstdlib>
#include<cmath>
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include "Cabecera.hpp"

using namespace std;
using namespace cv;


void trackBarEventHSV(int v, void *p){

}

void CapturaVideo::capturarVideo(){

    VideoCapture video(0);
    if(video.isOpened()){
        Mat frame;
        Mat imgHSV;
        Mat img_threshold;
    	Mat img_gray;
    	Mat img_roi;
        Mat roi;
        Mat objeto;
        namedWindow("Frame",WINDOW_AUTOSIZE);
        namedWindow("Gray_image",WINDOW_AUTOSIZE);
        namedWindow("Thresholded_image",WINDOW_AUTOSIZE);
        namedWindow("ROI1",WINDOW_AUTOSIZE);
        createTrackbar("H-Min","Frame",&minH,180,trackBarEventHSV, NULL);
        createTrackbar("S-Min","Frame",&minS,255,trackBarEventHSV, NULL);
        createTrackbar("V-Min","Frame",&minV,255,trackBarEventHSV, NULL);
        createTrackbar("H-Max","Frame",&maxH,180,trackBarEventHSV, NULL);
        createTrackbar("S-Max","Frame",&maxS,255,trackBarEventHSV, NULL);
        createTrackbar("V-Max","Frame",&maxV,255,trackBarEventHSV, NULL);
        int pixel = 0;
        double huMoments[7];
        Moments momentos;
        double baseDatos[7]={0.1825,0.00552519,2.87566e-05,1.08994e-06,-1.1631e-12,-6.93679e-08,-5.99014e-12};
        double cx =0;
        double cy = 0;
        int count =0;
        char a[40];
       
        

        
        

        while(3==3){
            video >> frame;
            cvtColor(frame,imgHSV,COLOR_BGR2HSV);
            inRange(imgHSV, Scalar(minH,minS,minV),Scalar(maxH,maxS,maxV),roi);
            {   
                objeto = Mat(Size(frame.cols, frame.rows),CV_8UC3,Scalar(255,255,255));
            }
            Rect roi1(340,100,280,280);
            img_roi=frame(roi1);
            cvtColor(img_roi,img_gray,COLOR_RGB2GRAY);
            
            GaussianBlur(img_gray,img_gray,Size(19,19),0.0,0);
            threshold(img_gray,img_threshold,0,255,THRESH_BINARY_INV+THRESH_OTSU);
            
           vector<vector<Point> >contours;
   	   vector<Vec4i>hierarchy;
           findContours(img_threshold,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE,Point());
            if(contours.size()>0){
                size_t indexOfBiggestContour = -1;
	            size_t sizeOfBiggestContour = 0;

	            for (size_t i = 0; i < contours.size(); i++){
		           if(contours[i].size() > sizeOfBiggestContour){
			       sizeOfBiggestContour = contours[i].size();
			       indexOfBiggestContour = i;
		          }
                }
                
             	vector<vector<int> >hull(contours.size());
                vector<vector<Point> >hullPoint(contours.size());
                vector<vector<Vec4i> >defects(contours.size());
                vector<vector<Point> >defectPoint(contours.size());
                vector<vector<Point> >contours_poly(contours.size());
                Point2f rect_point[4];
                vector<RotatedRect>minRect(contours.size());
                vector<Rect> boundRect(contours.size());
                
                for(size_t i=0;i<contours.size();i++){
                if(contourArea(contours[i])>5000){
                	convexHull(contours[i],hull[i],true);
                	convexityDefects(contours[i],hull[i],defects[i]);
                	if(indexOfBiggestContour==i){
                	   minRect[i]=minAreaRect(contours[i]);
                            for(size_t k=0;k<hull[i].size();k++){
                                int ind=hull[i][k];
                                hullPoint[i].push_back(contours[i][ind]);
                            }
                            count =0;
                            
                            for(size_t k=0;k<defects[i].size();k++){
                                if(defects[i][k][3]>13*256){
                                 /*   int p_start=defects[i][k][0];   */
                                    int p_end=defects[i][k][1];
                                    int p_far=defects[i][k][2];
                                    defectPoint[i].push_back(contours[i][p_far]);
                                    circle(img_roi,contours[i][p_end],3,Scalar(0,255,0),2);
                                    count++;
                                }

                            }
                            
                            if(count==1)
                                strcpy(a,"numero 1");
                            else if(count==2)
                                strcpy(a,"numero 2");
                            else if(count==3)
                                strcpy(a," numero 3");
                            else if(count==4)
                                strcpy(a,"numero 4");
                            else if(count==5)
                                strcpy(a,"numero 5");
                            else
                                strcpy(a,"hola !!");
                                
                            putText(frame,a,Point(70,70),FONT_HERSHEY_SIMPLEX,3,Scalar(255,0,0),2,8,false);
                            drawContours(img_threshold, contours, i,Scalar(255,255,0),2, 8, vector<Vec4i>(), 0, Point() );
                            drawContours(img_threshold, hullPoint, i, Scalar(255,255,0),1, 8, vector<Vec4i>(),0, Point());
                            drawContours(img_roi, hullPoint, i, Scalar(0,0,255),2, 8, vector<Vec4i>(),0, Point() );
                            approxPolyDP(contours[i],contours_poly[i],3,false);
                            boundRect[i]=boundingRect(contours_poly[i]);
                            rectangle(img_roi,boundRect[i].tl(),boundRect[i].br(),Scalar(255,0,0),2,8,0);
                            minRect[i].points(rect_point);
                            for(size_t k=0;k<4;k++){
                                line(img_roi,rect_point[k],rect_point[(k+1)%4],Scalar(0,255,0),2,8);
                            }
                	}
                }
                
                    
            }
            }
            momentos = moments(roi,true);
            HuMoments(momentos,huMoments);
            imshow("Frame", frame);
            imshow("roi", roi);
            imshow("Gray_image",img_gray);
            imshow("Thresholded_image",img_threshold);
            imshow("ROI1",img_roi);

            if(waitKey(23)==27)
                break;
        }
        destroyAllWindows();
    }
//
}
