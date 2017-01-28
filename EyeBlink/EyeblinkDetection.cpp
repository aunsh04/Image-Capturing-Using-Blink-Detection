// EyeblinkDetection.cpp : Defines the entry point for the console application.
//



// OpenCV Sample Application: facedetect.c

// Include header files
#include "cv.h"
#include "highgui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

// Create memory for calculations
static CvMemStorage* storage = 0;

// Create a new Haar classifier
static CvHaarClassifierCascade* cascade = 0;

// Function prototype for detecting and drawing an object from an image
void detect_and_draw( IplImage* image );
void detect_eyes(IplImage* img);

// Create a new Haar classifier
static CvHaarClassifierCascade* cascade1 = 0;



// Create a string that contains the cascade name
const char* cascade_name =
    "haarcascade_frontalface_alt.xml";
/*    "haarcascade_profileface.xml";*/

const char* cascade_name1 =
    "haarcascade_eye.xml";

// Main function, defines the entry point for the program.
int main( int argc, char** argv )
{

    // Structure for getting video from camera or avi
    CvCapture* capture = 0;

    // Images to capture the frame from video or camera or from file
    IplImage *frame, *frame_copy = 0;


    // Load the HaarClassifierCascade
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
	cascade1 = (CvHaarClassifierCascade*)cvLoad( cascade_name1, 0, 0, 0 );

    // Check whether the cascade has loaded successfully. Else report and error and quit
    if( !cascade )
    {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return -1;
    }

    // Allocate the memory storage
    storage = cvCreateMemStorage(0);


        capture = cvCaptureFromCAM(0);


    // Create a new named window with title: result
    cvNamedWindow( "result", 1 );

    // Find if the capture is loaded successfully or not.

    // If loaded succesfully, then:
    if( capture )
    {
        // Capture from the camera.
        for(;;)
        {
            // Capture the frame and load it in IplImage
            if( !cvGrabFrame( capture ))
                break;
            frame = cvRetrieveFrame( capture );

            // If the frame does not exist, quit the loop
            if( !frame )
                break;

            // Allocate framecopy as the same size of the frame
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );

            // Check the origin of image. If top left, copy the image frame to frame_copy.
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            // Else flip and copy the image
            else
                cvFlip( frame, frame_copy, 0 );

            // Call the function to detect and draw the face
            detect_and_draw( frame_copy );

            // Wait for a while before proceeding to the next frame
            if( cvWaitKey( 10 ) >= 0 )
                break;
        }

        // Release the images, and capture memory
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
    }

    // Destroy the window previously created with filename: "result"
    cvDestroyWindow("result");

    // return 0 to indicate successfull execution of the program
    return 0;
}

// Function to detect and draw any faces that is present in an image
void detect_and_draw( IplImage* img )
{
    int scale = 1;

    // Create a new image based on the input image
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );

    // Create two points to represent the face locations
    CvPoint pt1, pt2;
    int i;

    // Clear the memory storage which was used before
    cvClearMemStorage( storage );

    // Find whether the cascade is loaded, to find the faces. If yes, then:
    if( cascade )
    {

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );

	/*	 for( i = 0; i < (faces ? faces->total : 0); i++ ) {
    CvRect* r = (CvRect*)cvGetSeqElem(faces, i);
	   cvRectangle(
      img,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(0, 255, 0),
      2, 8, 0
    );*/
cvShowImage( "result", img );
	cvWaitKey(3);
        // Loop the number of faces found.
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
           // Create a new rectangle for drawing the face
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

			cvSetImageROI(img, *r);
			IplImage* faceSubImageLeft = cvCreateImage( cvSize(r->width, r->height), img->depth, img->nChannels );

			cvCopy(img, faceSubImageLeft);







            detect_eyes(faceSubImageLeft);
            cvResetImageROI(img);
        }
    }

    // Show the image in the window named "result"
    //cvShowImage( "result", img );

    // Release the temp image created.
    cvReleaseImage( &temp );
}


/**
 * Given an image, this function will detect human eyes on the image and
 * draws green boxes around them.
 */
void detect_eyes(IplImage* img)
{
      cvClearMemStorage( storage );
	  CvRect EyeCenter;
	  CvFont font;
	  IplImage* img1;
	  img1 = cvCreateImage( cvSize(img->width,img->height),
                                            IPL_DEPTH_8U, img->nChannels );

    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);

	  CvScalar s,s1,s2,s3;
	  cvCopy(img,img1,0);
 
  /* Load the face detector and create empty memory storage */
  if (cascade1) {
    

  /* Detect objects */
  CvSeq* eyes = cvHaarDetectObjects(
    img,
    cascade1,
    storage,
    1.1,
    3,
    CV_HAAR_DO_CANNY_PRUNING,
    cvSize(40, 20)
  );

  int i,j,max=0;

  /* Draw green boxes around the eyes found */
  for( i = 0; i < (eyes ? eyes->total : 0); i++ ) {
    CvRect* r = (CvRect*)cvGetSeqElem(eyes, i);
	   cvRectangle(
      img,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(0, 255, 0),
      2, 8, 0
    );

	
  }
  for( i = 0; i < (eyes ? eyes->total : 0); i++ ) {
    CvRect* r = (CvRect*)cvGetSeqElem(eyes, i);
	EyeCenter.x = r->x + (r->width/2) - 10;
	EyeCenter.y = r->y + (r->height/2) - 10;

	EyeCenter.width = 20;
	EyeCenter.height = 20;

	//Eye SubImage Creation for Left,
	cvSetImageROI(img,EyeCenter);
	IplImage* EyeSubImageLeft = cvCreateImage( cvSize(EyeCenter.width, EyeCenter.height), img->depth, img->nChannels );
	cvCopy(img,EyeSubImageLeft);

	int width=EyeSubImageLeft->width;
	int height=EyeSubImageLeft->height;
    //Convert RGB to GRAY Image
	IplImage* LeftGray = cvCreateImage( cvSize( width,height ), IPL_DEPTH_8U, 1 );
	cvCvtColor( EyeSubImageLeft, LeftGray, CV_RGB2GRAY );

	for (i = 1; i<width - 1; i++)
	{
		for(j = 1; j<height - 1; j++)
		{
			s=cvGet2D(LeftGray,i,j);
			s1=cvGet2D(LeftGray,i-1,j+1);
			s2=cvGet2D(LeftGray,i,j+1);
			s3=cvGet2D(LeftGray,i+1,j+1);

			s.val[0] = (s.val[0] - (s1.val[0] + s2.val[0] + s3.val[0])/3);

			if (s.val[0] > max)
				max = s.val[0];

		}
	}
printf("intensity=%d\n",max);

if (max>9){
	cvPutText(img1, "Eye Open", cvPoint(10, 130), &font, cvScalar(0, 255, 255, 0));
}
else{
cvPutText(img1, "Eye close", cvPoint(10, 130), &font, cvScalar(0, 255, 255, 0));
}
cvShowImage( "result", img1 );
	cvWaitKey(3);

  }
      
  }
		

}
