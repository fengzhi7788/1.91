#include "stdafx.h" 
#include <cstdio>  
#include <cstdlib>  
#include <Windows.h>  
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\video\background_segm.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	
	const char *pstrImageName ="qq.jpg";
	//IplImage *pSrcImage =cvLoadImage(pstrImageName,0);
	IplImage *img = cvLoadImage(pstrImageName, CV_LOAD_IMAGE_COLOR);
	cvFlip(img,NULL,1); 
	//Mat picture(pSrcImage);
	if(!img)
	{
		printf("can't load ");
		return -1;
	}
	const char *pstrCascadeFileName ="haarcascade_frontalface_alt.xml";
	const char *pstrCascadeproface="haarcascade_profileface.xml";
	const char *pstrCascadeyes="haarcascade_eye.xml";
	if(!pstrCascadeFileName)
	{
		printf("error");
		return -1;
	}
	if(!pstrCascadeproface)
	{
		printf("error");
		return -1;
	}
	CvHaarClassifierCascade *pHaarCascade = NULL;
	CvHaarClassifierCascade *pHaarCascadepro=NULL;
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeFileName);
	if (pHaarCascade != NULL)
	{	
		CvMemStorage* storage = 0;
		storage = cvCreateMemStorage(0) ;
		CvHaarClassifierCascade* cascade_e = 0;


		DWORD dwTimeBegin, dwTimeEnd;  
		dwTimeBegin = GetTickCount(); 
		CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
		CvSeq *pcvSeqFaces = cvHaarDetectObjects(img, pHaarCascade, pcvMStorage);
		dwTimeEnd = GetTickCount();
		printf("人脸个数: %d   识别用时: %d ms\n", pcvSeqFaces->total, dwTimeEnd - dwTimeBegin);  
		for(int i = 0; i <pcvSeqFaces->total; i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);	
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width * 0.5));
			center.y = cvRound((r->y + r->height * 0.5));
			radius = cvRound((r->width + r->height) * 0.25);
			printf("Found a face at (%d, %d)\n", center.x, center.y);
			cvCircle(img,center,radius, cvScalar(255,255,255));
			//ellipse(picture, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(0, 0, 255),5, 2, 0);
		}
	}
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeyes);
	if(pHaarCascade != NULL)
	{
		CvMemStorage *cvMstorage = cvCreateMemStorage(0);
		cvClearMemStorage(cvMstorage);
		CvSeq *cvSeqFace = cvHaarDetectObjects(img, pHaarCascade, cvMstorage);
		for(int i = 0; i < cvSeqFace->total; i++)
		{
			CvRect *r = (CvRect*)cvGetSeqElem(cvSeqFace, i);
			CvPoint center;
			int radius;
			center.x = cvRound(r->x + r->width * 0.5);
			center.y = cvRound(r->y + r->height * 0.5);
			radius = cvRound((r->width + r->height)*0.25);			
			cvCircle( img, center, radius,  cvScalar(255,255,255));
		}
		cvReleaseMemStorage(&cvMstorage);
	}
	pHaarCascadepro = (CvHaarClassifierCascade*)cvLoad(pstrCascadeproface);
	if(pHaarCascadepro!=NULL)
	{
	}
	cvNamedWindow("face detection", CV_WINDOW_AUTOSIZE);
	cvShowImage("face detection", img);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&img);	
}