#pragma once

#include "geste.h"
/**************/
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <msclr\marshal_cppstd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>
#include <cmath>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <opencv/cv.h>
#include <vector>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
/*******************************************/


namespace pfe_gui {


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	/// <summary>
	/// Summary for simul
	/// </summary>
	public ref class simul : public System::Windows::Forms::Form
	{
	public:


 
		simul(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
				

		/********************************************************/
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~simul()
		{
			if (components)
			{
				delete components;
			}
		}

/****************************************/



		//Global variables
		String^ img;
		int directionpouce;
		bool Globalpouce;
	public: static int stop= -1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Label^  label3;


	private: 

	public: 



	public: 

	public: 
		int nb_alea; 

		//structure du tableau
value struct Guide{
int etat; // 1 si occupé et 0 free
int num; //numéro de  l'img pr trouver l'ordre
String^ nom_img;
System::Windows::Forms::PictureBox^  box;
};


 string concatene_nom_img(int nb_alea,string nom)
 {
 string res;
 System::String^ managedString = nb_alea.ToString();
    msclr::interop::marshal_context context;
    string numImg = context.marshal_as<string>(managedString);
	res=nom+numImg+".jpg";
	return res;
 }


		//-----------------------------------------------------------------------------------------------------------

/**la fontion qui calcule les valeurs de y cb et cr d'un pixel**/
void calculYCbCr(cv::Vec3b p,double *yVal,double *cbVal,double *crVal )
{
   *yVal=(double)p.val[2]*65.738/256+(double)p.val[1]*129.057/256+(double)p.val[0]*25.064/256+16;
   *cbVal=(double)-p.val[2]*34.945/256-(double)p.val[1]*74.494/256+(double)p.val[0]*112.439/256+128;
   *crVal=(double)p.val[2]*112.439/256-(double)p.val[1]*94.154/256-(double)p.val[0]*18.258/256+128;
}
//--------------------------------------------------------------------------------------------------------------
/**la fonction qui retourne le max entre 3 nombres**/
double Max(double a,double b, double c){
if(a>b)
    if(a>c) return a;
       else return c;
else
    if(b>c)
    return b;
else return c;

}
//---------------------------------------------------------------------------------------------------------------
/**la fonction qui retourne le min entre 3 nombres**/
double Min(double a,double b, double c){
if(a<b)
    if(a<c) return a;
       else return c;
else
    if(b<c)
    return b;
else return c;
}
//-------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
/**la fontion qui calcule les valeurs HSV d'un pixel a partir des valeurs RGB**/
void calcul_Hsv(cv::Vec3b p,double *H,double *S,double *V)
{
    double Cmax,Cmin,R,G,B;
    double delta; 
    B=(double)p.val[0]/255;
    G=(double)p.val[1]/255;
    R=(double)p.val[2]/255;
    Cmax=Max(R,G,B);
    Cmin=Min(R,G,B);
    delta=Cmax - Cmin;
    //calculer H
if(delta==0)
     *H=0;
     else {
        if(Cmax=R)
        {
            *H= (G - B) /delta;
        }
        else if (Cmax=G)
        {
            *H= 2 + ( B - R ) / delta;
        }
        else {
           *H=  4 + ( R - G ) / delta;
        }
    *H *= 60;
     if( *H < 0)
 *H += 360;

     }
    //calculer S
     if(Cmax==0)
    *S=0;
    else *S=delta/Cmax;
    //calculer V
   *V=Cmax;
}

//-------------------------------------------------------------------
/*******************/

int verif_pixel_HSV(cv::Vec3b pixel)
{
				// 0<H<0.25  -   0.15<S<0.9    -    0.2<V<0.95
if( (pixel[0]>5) && (pixel[0] < 17) && (pixel[1]>38) && (pixel[1]<250) && (pixel[2]>51) && (pixel[2]<242) )
				return 0;
else return 1;
		}

//----------------------------------------------------------------------------------------------------------------
int verif_pixel_YCbCr2(cv::Vec3b pix)
{
if( (int) pix.val[0] >14 && (int) pix.val[0] <200) //y
{

  if((int) pix.val[2] <140 && (int) pix.val[2] >75){ //cb

    if( (int) pix.val[1] <180 && (int) pix.val[1] >138) //cr
    {

    return 0;
	}									}
}

return 1;
}
//---------------------------------------------------------------------------------------------------------------
int verif_pixel_YCbCr3(cv::Vec3b pix)
{
if( (int) pix.val[0] >14 && (int) pix.val[0] <255)
{

  if((int) pix.val[2] <134 && (int) pix.val[2] >84){

    if( (int) pix.val[1] <184 && (int) pix.val[1] >134) //cout << "dans le 3eme if" << endl;
    {

    return 0;
	}									}
}

return 1;
}


//---------------------------------------------------------------------------------------------------------------
void face_eliminer(cv::Mat img_src,cv::Mat *img_seg,int nb_alea)
{
     cv::CascadeClassifier face_cascade;
     std::vector<cv::Rect> faces;
     cv::Mat frameGray;
     cv::Vec3b p;
	 cv::Mat src=img_src.clone();
    if (!face_cascade.load("lbpcascade_frontalface.xml"))
    {
//    ltest->Text="Couldn't load face_cascade";
    exit(-1);
    }

	cv::cvtColor(img_src,frameGray,CV_BGR2GRAY);
	face_cascade.cv::CascadeClassifier::detectMultiScale(frameGray, faces, 1.1 , 2 , 0|CV_HAAR_DO_CANNY_PRUNING , cv::Size(30,30));

	int seuil;
	for(int i=0; i < faces.size();i++)
   {  if(faces[i].y < 10 && faces[i].x <10 )
	   seuil= 10;
       else seuil=0;

       for(int x=faces[i].x -seuil ; x<faces[i].x+faces[i].width+20; ++x)
       {
        for(int y=faces[i].y -seuil ; y<faces[i].y+faces[i].height+20; ++y)
        {
		   (*img_seg).at<cv::Vec3b>(y,x)[0]=0;
		   (*img_seg).at<cv::Vec3b>(y,x)[1]=0;
		    (*img_seg).at<cv::Vec3b>(y,x)[2]=0;
  
        }
       }
  cv::Point center(faces[i].x +faces[i].width*0.5 ,faces[i].y + faces[i].height*0.5);
 cv::ellipse(src,center,cv::Size(faces[i].width*0.5 , faces[i].height*0.5),0,0,360,cv::Scalar(255,0,255),4,8,0);
   }

	
	string nom_noface;
	nom_noface=concatene_nom_img(nb_alea,"No_face");
    cv::imwrite(nom_noface,src);
	
	
}

//---------------------------------------------------------------------------------------------------------------
// marche bien 
void detecterContours(cv::Mat RGB,cv::Mat img ,cv::Rect *maxRect,std::vector<cv::Point> *contour)
{
  cv::Mat resultatCanny;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchie;
  cv::Mat imgGray,imgGauss;

  // image bgr en image en niveau de gris 
  cv::cvtColor(img,imgGray,CV_BGR2GRAY);

  //appliquer le filtre gaussien sur l'image en niveau de gris
  cv::GaussianBlur(imgGray,imgGauss,cv::Size(5,5),1.5);

  /// Détecter les bords de la main avec Canny
  // Canny( img, resultatCanny, 100, 200, 3 );
   cv::Canny(img, resultatCanny, 80, 60);

  /// Trouver les contours de la main
  cv::findContours( resultatCanny, contours, hierarchie, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
   

  /// get bounding rects
  std::vector<cv::Rect> boundRect( contours.size() );



  /// Trouver les points convexes (convex hull)
  /// et aussi les points concaves (defects points)
  std::vector<std::vector<cv::Point> > pointsConvexe( contours.size() );
  std::vector<std::vector<int> > pointsConvexeI(contours.size());
  std::vector<std::vector<cv::Vec4i> > convdefect(contours.size());

  for( int i = 0; i < contours.size(); i++ )
  {
    cv::convexHull( cv::Mat(contours[i]), pointsConvexe[i], false);
    cv::convexHull( cv::Mat(contours[i]), pointsConvexeI[i], false);
    if(pointsConvexeI[i].size() > 3 )
      cv::convexityDefects(contours[i],pointsConvexeI[i],convdefect[i]);

  }


    for( int i = 0; i < contours.size(); i++ )
     { //approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true ); //refermer le contour
       boundRect[i] = cv::boundingRect( cv::Mat(contours[i]) );
       if( (*maxRect).size().area() < boundRect[i].size().area())
       {*maxRect=boundRect[i];
	    // *contour= contours[i];
	   }
	   }



  /// Dessiner les contours( en bleu) et les convex hull (en rouge)
  cv::Mat dessiner = cv::Mat::zeros( resultatCanny.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
  {
    size_t count = contours[i].size();
    if( count <300 )
      continue;
//if(contours[i].size()> 70 )
    cv::drawContours( dessiner, contours, i, cv::Scalar(255,0,0), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point()); //dessiner conteur
    //drawContours( dessiner, pointsConvexe, i, cv::Scalar(0,0,255), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());//dessiner la coque convexe
    if(*maxRect==boundRect[i]) //s'assurer qu'on dessine qu'autour la main
      cv::rectangle( RGB, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0,255,0), 2, 8, 0 );//dessine la bounding box autour de la main
  }


  }

  
////////*************************************************************************//////////////////////////////

// nous renvoie la liste des points concaves/convexes to use it in fraction_....

void detecterContours_modifier(cv::Mat RGB,cv::Mat img ,cv::Rect *maxRect,std::vector<cv::Point> *contour,int numImg)
{
  cv::Mat resultatCanny;
  std::vector<std::vector<cv::Point> > contours;
  std::vector<cv::Vec4i> hierarchie;
  cv::Mat imgGray,imgGauss;

  // image bgr en image en niveau de gris
  cv::cvtColor(img,imgGray,CV_BGR2GRAY);

  //appliquer le filtre gaussien sur l'image en niveau de gris
  cv::GaussianBlur(imgGray,imgGauss,cv::Size(5,5),1.5);

  /// Détecter les bords de la main avec Canny
  // Canny( img, resultatCanny, 100, 200, 3 );
   cv::Canny(img, resultatCanny, 80, 60);

  /// Trouver les contours de la main
  cv::findContours( resultatCanny, contours, hierarchie, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );


  /// get bounding rects
  std::vector<cv::Rect> boundRect( contours.size() );


    for( int i = 0; i < contours.size(); i++ )
     { //approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true ); //refermer le contour

       boundRect[i] = cv::boundingRect( cv::Mat(contours[i]) );
       if( (*maxRect).size().area() < boundRect[i].size().area()&&(contours[i].size()>70))
       {*maxRect=boundRect[i];
	    *contour= contours[i];
	   }
	   }


  /// Dessiner les contours( en bleu) et les convex hull (en rouge)
 // cv::Mat dessiner = cv::Mat::zeros( resultatCanny.size(), CV_8UC3 );
  for( int i = 0; i< contours.size(); i++ )
  {
    size_t count = contours[i].size();
    if( count <300 )
      continue;
//if(contours[i].size()> 70 )
    cv::drawContours( RGB, contours, i, cv::Scalar(255,0,0), 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point()); //dessiner conteur
    //drawContours( dessiner, pointsConvexe, i, cv::Scalar(0,0,255), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());//dessiner la coque convexe
    if(*maxRect==boundRect[i]) //s'assurer qu'on dessine qu'autour la main
      cv::rectangle( RGB, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0,255,0), 2, 8, 0 );//dessine la bounding box autour de la main
  }

  string nom_contours;
	nom_contours=concatene_nom_img(nb_alea,"contours");
    cv::imwrite(nom_contours,RGB);
//imshow("dessiner contour",RGB);
//cvWaitKey(0);
}

/****************************************************************************************/

/****************************************************************************************/
int max_sub_mat( int **M,int L,int C,int *max_i,int *max_j)
{
    int i=0,j=0;
    int **T;
	int max_of_s=0;

	T = new int*[L];
	for(int i = 0; i < L; i++)
      T[i] = new int[C];

    
    /****** Set first column of S[][] *******/
  for(i = 0; i < L; i++)
     T[i][0] = M[i][0];

  /***** Set first row of S[][] *****/
  for(j = 0; j < C; j++)
     T[0][j] = M[0][j];

  /***** Construct other entries of S[][] *****/
  for(i = 1; i < L; i++)
  {
    for(j = 1; j < C; j++)
    {
      if(M[i][j] == 1)
	  { int minim= (int) Min(T[i][j-1], T[i-1][j], T[i-1][j-1]);
		  T[i][j] = minim + 1;
	  }
      else
        T[i][j] = 0;
    }
  }



 /****** Find the maximum entry, and indexes of maximum entry in S[][] ********/
  max_of_s = T[0][0];
  *max_i = 0;
  *max_j = 0;

  for(i = 0; i < L; i++)
  {
    for(j = 0; j < C; j++)
    {
      if(max_of_s < T[i][j])
      {
         max_of_s = T[i][j];
         *max_i = i;
         *max_j = j;
      }
    }
  }


return max_of_s;
}


/********************************************************************/

int construct_sous_Mat(cv::Rect MaxRect,cv::Mat src,int *max_ipaum,int *max_jpaum)
{

 int start_x=MaxRect.tl().x;
 int start_y=MaxRect.tl().y;
  int init=start_x;
  int lign= MaxRect.height,col=MaxRect.width ;

 
//Créer Matrice,pour extraire la matrice binaire M[ligne][col]qui a la taille de la Bounding box
  int** M = new int*[lign];
for(int i = 0; i <lign; i++)
	M[i] = new int[col];


 //remplir la matrice de la bounding box
 //remplir la matrice de la bounding box
 for(int i=0;i<lign;i++)
 {
     for(int j=0;j<col;j++)
     {
		// Vec3b  pixel=src.at<Vec3b>(start_y,start_x);
		 //cout <<"valeur pixel:	" << pixel <<endl;
         if( src.at<cv::Vec3b>(start_y,start_x)[0] == 255)
            {
                M[i][j]=1;
            }
         else { if(src.at<cv::Vec3b>(start_y,start_x)[0] == 0)
                M[i][j]=0;
              }

         start_x ++ ; ///passer au prochain pixel sur la meme ligne

     }
      start_x=init; ///démarrer du meme indice de la bounding box sur la prochaine ligne
      start_y ++; ///passer à la prochaine ligne de pixels
 }
// recupérer le max de la matrice et ses coordonnées
 int max_i,max_j;
 int res=max_sub_mat( M,lign,col,&max_i,&max_j);
 //cout << "dans la fonction" << endl;
 *max_ipaum=max_i; *max_jpaum=max_j;
//  cout <<"max i " << *max_ipaum<< endl <<"j " << *max_jpaum << endl;
           return(res);

}

////////*************************************************************************//////////////////////////////
////////*************************************************************************//////////////////////////////
cv::Point draw_paume_main(cv::Mat src,cv::Mat RGB,int max_i,int max_j,int taille,cv::Rect bound,int *br_paume_y,int numImg)
{
		 //cout << "dans la fontion draw ppaume"<<endl;
		 //cout <<"max i " << max_i<< endl <<"max j " << max_j<<endl <<"max taille"<< taille<< endl;
		cv::Point BR_src,TL_src;
		//calculer top left du rect à dessiner
		BR_src.y=bound.tl().y+max_i;
		BR_src.x=bound.tl().x+max_j;
		//calculer bottom right du rect à dessiner
		TL_src.y=BR_src.y-taille;
		TL_src.x=BR_src.x-taille;
		cv::Mat imgpaume= cv::Mat::zeros(src.size(),CV_8UC3);
		imgpaume = src.clone();
		cv::rectangle( RGB,TL_src,BR_src, cv::Scalar(0,255,0), 2, 8, 0 );
	//cv::imshow("paume",imgpaume);
	  //  cv::waitKey(0); 
		string nom_paume;
	    nom_paume=concatene_nom_img(nb_alea,"paume");
	    cv::imwrite(nom_paume,RGB);
    
		*br_paume_y=BR_src.y;

		return TL_src;
}

   ///*******************************************************************************************************************/////
/////*********************** INTRODUCTION FONCTION QUI UTILISE LA KINECT *****************************/////////////////

/********************************************************************/
cv::Mat acquisitionImageReference()
{
//declaration des variables;
cv::Mat tab[100];
int i,j,k;
unsigned long somme;
cv::VideoCapture sensor1;
sensor1.open(CV_CAP_OPENNI);
//les instruction ;
///l'acquisition des frames (depth frame)
for(i=0;i<100;i++)
{
sensor1.grab();
sensor1.retrieve(tab[i], CV_CAP_OPENNI_DEPTH_MAP );
 if( cv::waitKey(30) >= 0 )
        break;
}
///calcul de l'image reference
for(i=0;i<tab[0].rows;i++)
{
  for(j=0;j<tab[0].cols;j++)
   {
    somme=0;
    for(k=0;k<100;k++)
     {
      somme=somme+tab[k].data[j + i*tab[0].cols];
     }
      tab[0].data[j + i*tab[0].cols]=somme/100;
    }

}

return tab[0];
}
/********************************************************************/
void acquisitionImageGeste(cv::Mat *RGB,cv::Mat *depth)
{
cv::Mat show,resultat,res;

cv::VideoCapture sensor1;
sensor1.open(CV_CAP_OPENNI);
for(int i=0;i<30;++i)
       {
		   sensor1.grab();
    if( sensor1.retrieve(*depth,CV_CAP_OPENNI_DEPTH_MAP) )
        sensor1.retrieve(*RGB,CV_CAP_OPENNI_BGR_IMAGE );
         //imshow("depth",*depth);
		    if( cv::waitKey( 30 )==27 )   break;
       }

}
/********************************************************************/

cv::Mat findShadow(cv::Mat depth)//elle retourne une image avec l'ombre en blanc
{
 cv::Mat show,resultat;
 cv::Mat depth2=depth;
    depth2.convertTo(show,CV_8UC1,100.0/2048.0);
    cv::threshold(show,resultat,20,255,CV_THRESH_BINARY_INV);
return resultat;
}

/********************************************************************/
///segmentation HSV+YCBCR avec la suppression du background
//segmentation en IplImage
/*cv::Mat segmentationVS(cv::Mat imgres,array< HSV >^ tab1,array< ycbcr >^ tab2)
{
	int i,j,k=0;
    CvScalar p,p1,p2;

 /*IplImage copyD = depthgest;
 IplImage* imgD = &copyD; //résultat dans imgD

  IplImage copyB = depthbackg;
  IplImage* imgB = &copyB; //résultat dans imgB
 
 */
 /*IplImage copySH = imgres;
 IplImage* imgSH = &copySH; //résultat dans imgSH	

 
cv::Mat dst(imgres.rows,imgres.cols,CV_8UC3,cv::Scalar(0,0,0));
for(i=0;i<imgres.rows;i++)
{
  for(j=0;j<imgres.cols;j++)
    {

   if( (verif_pixel_HSV(tab1[k])==0) || (verif_pixel_YCbCr(tab2[k])==0))
		{
    	
		p2= cvGet2D (imgSH,i,j);


		 if((double)p2.val[0]==255)
		 { dst.at<cv::Vec3b>(i,j)[0]=255;
		   dst.at<cv::Vec3b>(i,j)[1]=255; 
		   dst.at<cv::Vec3b>(i,j)[2]=255;
		 }
   }
   else
   {
	  dst.at<cv::Vec3b>(i,j)[0]=0;
	  dst.at<cv::Vec3b>(i,j)[1]=0;
	  dst.at<cv::Vec3b>(i,j)[2]=0;
   }
     k++;

    }
}
return dst;
}*/

/*********************************/
cv::Mat segmentationDirecte(cv::Mat imgres,cv::Mat imgRGB)
{

	 CvScalar p;

 IplImage imgresD = imgres;
 IplImage* imgD = &imgresD; //résultat dans imgD
	cv::Mat dst(imgres.rows,imgres.cols,CV_8UC3,cv::Scalar(0,0,0));
	cv::Mat imgycbcr,result,imghsv;
	cvtColor(imgRGB,imgycbcr, CV_BGR2YCrCb);
	cvtColor(imgRGB,imghsv, CV_BGR2HSV);
	cv::Vec3b pixel1,pixel2;
	int res=0;
	for(int i=0;i<imgres.rows;i++)
	{	for(int j=0; j<imgres.cols;j++)
		{
		pixel1=imgycbcr.at<cv::Vec3b>(i,j); 
		pixel2=imghsv.at<cv::Vec3b>(i,j);
		p= cvGet2D (imgD,i,j);

		 if((verif_pixel_YCbCr3(pixel1) == 0)  || ( verif_pixel_HSV(pixel2) == 0) )
		 {
			 if( (double)p.val[0]==255) {
			 dst.at<cv::Vec3b>(i,j).val[0]=255;
		     dst.at<cv::Vec3b>(i,j).val[1]=255; 
			 dst.at<cv::Vec3b>(i,j).val[2]=255; }
		 }
		   else
		   {	
		  dst.at<cv::Vec3b>(i,j).val[0]=0;
		  dst.at<cv::Vec3b>(i,j).val[1]=0;
		  dst.at<cv::Vec3b>(i,j).val[2]=0; 
		
		   }
		}
	    
	}
		/*cv::imshow("dans beug",dst);
		cv::waitKey(0);
	/*cv::Mat kernel5x5 = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    morphologyEx(dst, dst, cv::MORPH_OPEN, kernel5x5);*/
	     IplImage *img =NULL, *img_res=NULL;
         img = cvCreateImage(cvSize(dst.cols ,dst.rows),8,3);
		 IplImage ipltemp=dst;
		 cvCopy(&ipltemp,img);

         cvDilate(img,img,0,2);
         
		

         /**** dilate  and display the eroded/dilated image  ****/

       cvErode(img,img,0,2);
      // cvShowImage("dans segm aprés dilaterose", img);
       //cv::waitKey(0);

	   /**** Trouver les contours / main et la mettre dans un rectangle ****/

       cv::Mat dst2= cv::Mat(img, false);
	   string nom_dilateros;
	   nom_dilateros=concatene_nom_img(nb_alea,"dilaterose");
	   cv::imwrite(nom_dilateros,dst2);
		return dst2;
}


/*************************************************************/
	 // CHETVERECOV 
	/*
	/* The line drawn by the user. */
//var Line = new Array();
/* For each point on the line, measure the sharpness of the turn there. */
//var sharpness = new Array();
/* The detected corners are stored in the following array. */
//var corners = new Array();
/* Variables for the algorithm: see

/*var d_min = 10;
var d_min_sq = d_min*d_min;
var d_max = 2*d_min;
var d_max_sq = d_max*d_max;
var cos_min = ‐0.75;
*/

//Calculate the sharpness at each point of the curve.
value struct sharp{
	int indplus;
	int indmoins;
	float angle;
	int empty; //si vide 0, 2 fail lower,1 fail upper, 3 angle fail,4 OK
};
 /*****************************************************************************/

	//calculer la distance entre p_succ et p_pred
	double calculer_distance ( cv::Point p_succ,cv::Point p_pred)
	{
	double dist=0,dx=0,dy=0;
	dx = p_pred.x - p_succ.x;
	dy = p_pred.y - p_succ.y;
	dist = sqrt(dx*dx + dy*dy);
	return dist;
	}


	/************************************************************/

	/************************************************************/

void dessiner_points_forte_courbure(cv::Mat dst,std::vector<int> list_indic,std::vector<cv::Point> handcount,int length,string numImg)
	{
	
    /// Dessiner les points (en (255,0,255))

   
	cv::Point to_draw;
	for(int i=0; i<length;i++)
    {
        to_draw=handcount[list_indic[i]];
        cv::circle( dst,to_draw,  4, cv::Scalar(164,28,164), 2 );
		
    }
      cv::imshow("chet new dessiner",dst);
	  cv::waitKey(0);
	  string nom_chetvere;
	  nom_chetvere=concatene_nom_img(nb_alea,"chetvere");
	  cv::imwrite(nom_chetvere,dst);
	}
	
/********************************************************************************************************/
 
	void dessiner_points_concave_convexe_courbure(cv::Mat img,vector<int> lconcave,
		vector<int> lconvexe,vector<cv::Point> handcount,int length1,int length2,int concavepouce,int convexepouce)
	{
	//Mat dessiner = Mat::zeros( (*img).size(), CV_8UC3 );
    /// Dessiner les points (en (255,0,255))
    int var;
	cv::Point to_draw;
	for(int i=0; i<length1;i++)
    {  var=lconcave[i];
        to_draw=handcount[var];
        cv::circle( img,to_draw,  4, cv::Scalar(0,255,0), 2 ); //concave vert

    }
    
	for(int i=0; i<length2;i++)
    {  var=lconvexe[i];
        to_draw=handcount[var];
        cv::circle( img,to_draw,  4, cv::Scalar(255,0,0), 2 ); //convexe bleu
    }

	if(concavepouce != -1 )
	{
	 to_draw=handcount[concavepouce];
     cv::circle( img,to_draw,  4, cv::Scalar(0,0,255), 2 );
	}

	if(convexepouce != -1 )
	{
	 to_draw=handcount[convexepouce];
     cv::circle( img,to_draw,  4, cv::Scalar(255,0,255), 2 );
	}

	string nom_pointfin;
	nom_pointfin=concatene_nom_img(nb_alea,"pointsfinals");
	cv::imwrite(nom_pointfin,img);

	}
	/************************************************************/
	
	double calculer_angle_alpha(double a,double b,double c)
	{
	double alpha=0;
	alpha= acos((a*a + b*b - c*c)/(2*a*b));
	return alpha;
	}

	/************************************************************/
	//retourne 1 si point convexe et 0 si point concave
	int concave_convexe(cv::Point p_succ,cv::Point p_pred)
	{
	double x=0;
	x=(p_succ.x * p_pred.y) - (p_succ.y * p_pred.x);
	if(x < 0)
		return 0;
	else if (x> 0)
		return 1;
	
	}


    /***************************************************************************/
  int taille_vect(int x)
  {
   int fac=0;
   if(x == 0 ) return 1;
   
   for(int i=1;i <= x; i++)
	   fac= fac + i ;
  return fac;
  }

    /***************************************************************************/

  
  /*********************************************************************/
 
 
  /**********************************************************************/
  void delete_arm(cv::Mat *img,int taillePaume,int brPaume)
  {
   int seuil_y= brPaume + (taillePaume/3);
   for(int i=seuil_y; i< (*img).rows ;i++)
	   for(int j=0;j< (*img).cols; j++)
	   {

		   (*img).at<cv::Vec3b>(i,j)[0]=0;
		   (*img).at<cv::Vec3b>(i,j)[1]=0;
		   (*img).at<cv::Vec3b>(i,j)[2]=0;

	   }

  }
  /*******************************************************************/

 
bool point_contour(vector<cv::Point> contourHand,cv::Point p,cv::Point mc,cv::Point pouce)
{ 
	int i=0; 
	bool trouve=false;
	while(i < contourHand.size() &&  (trouve==false))
	{
	if((contourHand[i] == p) && (contourHand[i].x != pouce.x && contourHand[i].y != pouce.y) && (contourHand[i] != mc))
		trouve=true;
		i++;
	}
	
	return trouve;
}
		/*************************************************/
  cv::Point find_concave_pouce(cv::Point2f mc,cv::Point pouce,vector<cv::Point> contourHand,cv::Mat img)
  {
  bool trouve=false;
  cv::Point p(0,0),pt(0,0);
  cv::LineIterator it(img,pouce,mc,8);
  int i=0;
	while(i< it.count && (trouve==false) )
		{
		pt=it.pos();
		trouve=point_contour(contourHand,pt,mc,pouce);
		i++;
		it++;
		
	    }
	if(trouve == true) p=pt;
  return p;
  }
  /***************************************************************/
  //on lui donne l'image aprés suppression du bras et détection contour
  cv::Point2f find_centre_gravit(cv::Mat img,vector<cv::Point> contourHand,int numImg)
  {
   /// Get the moments
  cv::Moments mu;
   mu = moments( contourHand, false );


 ///  Get the mass centers:
  cv::Point2f mc;
  mc = cv::Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 ); 

   
  //dessiner le centre de gravité sur une image à part et l'enregistrer pr l'afficher dans l'interface later  
    cv::Mat imgcentregrav= cv::Mat::zeros( img.size(), CV_8UC3 );
    imgcentregrav= img.clone();
    cv::circle( imgcentregrav , mc, 4, cv::Scalar(255, 69,37), -1, 8, 0 );
	string nom_centre;
	nom_centre=concatene_nom_img(numImg,"centregrav");
    cv::imwrite(nom_centre,imgcentregrav);
	return mc;

/*	std::vector<cv::Point > storage;
	std::vector<cv::Point > poly ;
	cv::approxPolyDP( cv::Mat(contourHand),poly, 3, true );*/
	
		//cvApproxPoly(contourHand, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 2,1);
	// cvCvtSeqToArray(poly, points, CV_WHOLE_SEQ);
	// find_convexe(mc,poly,img,numImg);
	 // cvLine(frame, center_of_mass, fingers[i], CV_RGB(255,255,0), 1, CV_AA, 0);
  }
  /******************************************************************************/
   /*------------------------------------------------------------------------------------------------*/
 	int indice_p_dans_contour(vector<cv::Point> contourHand,cv::Point p)
	{
	 cout<<"Debut fonction indice"<<endl;
		int i=0,indic=-1;
		bool trouve=false;
		while(i < contourHand.size() && trouve==false)
		{
		cout<<"Debut while"<<endl;
		if(p.x== contourHand[i].x && p.y== contourHand[i].y )
		{
		cout<<"dans if"<<endl;
			indic=i;
			trouve=true;
		}
		i++;
		}
		return indic;
	}
 /*-------------------------------------------------------------------------------------------------*/
//cette fonction calcule l'angle d'un point donné en utilisant son successeur et son predecesseur
 double calculAngle(cv::Point p,cv::Point pPlus,cv::Point pMoins)
 {
 double a,b,c;
 a= calculer_distance(p,pPlus);//a
 b= calculer_distance(p,pMoins);//b
 c= calculer_distance(pPlus,pMoins);
 double alpha=0;
	alpha= acos((a*a + b*b - c*c)/(2*a*b));
	cout<<alpha<<endl;
	return alpha;
}
  /*********************************************************************************/
   void EliminerConcaveAngle(vector<cv::Point> ContourHand,vector<cv::Point> *concave,int *size,cv::Mat *img )
 {
 cout<<"Debut"<<endl;
double Angle;
int pas=5;
vector<cv::Point> resultat(*size);
 cout<<"Apres allocation"<<endl;
int i,indice,indicePlus,indiceMoins,j=0;
for(i=0;i<(*size);i++)
{
 cout<<"Dans le boucle"<<endl;
indice=indice_p_dans_contour(ContourHand,(*concave)[i]);
 cout<<"Apres indice"<<endl;
//---------------calcul de p+ et p- ----------------------------
if(indice+pas >= ContourHand.size())
	 {indicePlus=indice+pas-ContourHand.size();}
	 else
		 {indicePlus=indice+pas;}
	 if(indice-pas <0)
	 {indiceMoins=ContourHand.size()-pas+indice; }//ici ça beug sometimes car ind -pas est en dehors du contour
	 else
		{ indiceMoins=indice-pas;}
//---------------------------------------------------

Angle=calculAngle(ContourHand[indice],ContourHand[indicePlus],ContourHand[indiceMoins]);
if(Angle<1.75)
{
resultat[j]=ContourHand[indice];
j++;
}

}
for(int i=0;i<j;i++)
	 {

	 cv::circle(*img, (resultat)[i], 4,CV_RGB(30,60,80),3);
	// cv::line(*img, mc, (resultat)[i], CV_RGB(255,48,152), 1, CV_AA, 0);
	 }
*concave=resultat;
*size=j;
 }

  /***************************************************************************/
   
  /**************************************************************************************/
void supp_concave_distance_seuil(vector<cv::Point> *concaves,int *size,cv::Point mc)
{
int seuil=5,j=0;
double dist=0;
vector<cv::Point> res(*size);
for(int i=0;i<(*size);i++)
{
dist=calculer_distance((*concaves)[i],mc);
if(dist < seuil)
{
res[j]=(*concaves)[i];
j++;
}
}
(*concaves)=res;
*size=j;

}
/***************************************************************************/
  // retourne la liste des points filtrés concaves sans le pouce, to use to draw the curve
  vector<cv::Point> liste_sans_point_donne(vector<cv::Point> list,int *listsize,cv::Point pouce)
  { int j=0;
	 vector<cv::Point> list_courbe(*listsize - 1);
     for(int i=0;i<*listsize;i++)
	 {
		  if(list[i] == pouce)
			  continue;
     list_courbe[j]=list[i];
	 j++;
	 }
     *listsize=j;
	 return list_courbe;
  }


  /**************************************************************************************/
void point_convexe_geste(vector<cv::Point> *concaves,int *size,cv::Point mc,cv::Mat *img)
{
int seuil=50,j=0;
double dist=0;
vector<cv::Point> res(*size);
for(int i=0;i<(*size);i++)
{
dist=calculer_distance((*concaves)[i],mc);
if(dist > seuil)
{
res[j]=(*concaves)[i];
j++;
}
}
(*concaves)=res;
*size=j;
for(int i=0;i<j;i++){
cv::circle(*img, res[i], 4, cv::Scalar(255, 0, 0), 2); 
cv::line(*img, mc, res[i], CV_RGB(0,255,0), 1, CV_AA, 0);
}
}

/********************************************************************/
 /****************************************************************************/

  void trouver_points_courbures(cv::Mat *img,vector<cv::Point> contourHand,int nb_alea,cv::Point2f mc,cv::Rect boundRect1,int *taille)
  {
	  std::vector<cv::Point > storage;
	std::vector<cv::Point > poly ;
	int sizeConvxelimin=0;
	int ressize=0;
	cv::approxPolyDP( cv::Mat(contourHand),poly, 3, true );
	int num_fingers=0,num_concaves=0;
	vector<cv::Point> concaves;
	vector<cv::Point> fingers;

	/** trouver les points convexes ********/
    find_convexe(mc,poly,img,nb_alea,&num_fingers,&fingers);
	vector<cv::Point> res(num_fingers);
	eliminer_pics(fingers,num_fingers,&res,&sizeConvxelimin,img,mc, nb_alea); //elimine p convexes se trouvant dans le meme région

  //  point_convexe_geste(&res,&sizeConvxelimin,mc,img);//choisir les p convexes significatifs
	

	/** trouver les points concaves ***********/
	vector<cv::Point> concavesm(10);
	int sizeconcave=0;
    

  // find_concave_masse(contourHand,mc,&concavesm,&sizeconcave,img,nb_alea);
   find_concave(mc,poly,img,nb_alea,&num_concaves,&concaves);
	vector<cv::Point> listfiltrer(num_concaves);

	/*** trouver le pouce ******/
	cv::Point minconcave= min_concave_y(concaves,num_concaves);
	cv::Point pouce=chercher_pouce(res,sizeConvxelimin, minconcave,mc);
	/****** dessiner le pouce	**********/
	if(pouce != cv::Point(0,0))
	{
	cv::circle(*img, pouce, 4,CV_RGB(0,255,0),2);
	directionpouce=1;
	res=liste_sans_point_donne(res,&sizeConvxelimin,pouce);
	}
	*taille=sizeConvxelimin;
		string nom_pointfin;
	nom_pointfin=concatene_nom_img(nb_alea,"pointsfinals");
	cv::imwrite(nom_pointfin,*img);

	/**********  dessiner la courbe de jonction ********************************/
	vector<cv::Point> resconcaves(3);
	int sizeres=0;
	filtrage_concave_courbe(concaves,num_concaves,&listfiltrer,&ressize); //elimine les p concaves qui ont le meme x
	eliminer_vallees(listfiltrer,ressize,&listfiltrer,&ressize,img,mc,nb_alea); //elimine les points concaves qui se trouvent dans la meme région
	EliminerConcaveAngle(contourHand,&listfiltrer,&ressize,img); //elimine les p concaves qui ont un grand angle 
	supp_concave_distance_seuil(&listfiltrer,&ressize,mc); //elimine les p concaves qui se trouvent sur les doigts (entre cocanves/convexes)
	//chercher concave pouce
	cv::Point concavepouce=chercher_pouce_concave(listfiltrer,ressize);
   if(concavepouce != cv::Point(0,0))
	 cv::circle(*img,concavepouce, 4,CV_RGB(0,255,0),2);
   if(concavepouce != cv::Point(0,0) || pouce != cv::Point(0,0))
	   Globalpouce=true;
   else Globalpouce=false;
	
  }


  /***************************************************************************/
  //calculer l'équation de la courbe de jonction

void courbe_jonction(vector<int> listconcave,vector<cv::Point> contourHand,cv::Mat dst2,int numImg,cv::Rect boundRect1)
{
	if(listconcave.size() >=3) //3 points ou plus pr dessiner la courbe de jonction
	{
    double Matrice[3][4],stockcase;
    double Calcul[3][4];
    int i,j,k;
    double x,y,z;
    //remplissage de la matrice
	//valeur des x2              
    Matrice[0][0]=(contourHand[listconcave[0]].x) * (contourHand[listconcave[0]].x);  
    Matrice[1][0]=(contourHand[listconcave[1]].x) * (contourHand[listconcave[1]].x);							   
    Matrice[2][0]=(contourHand[listconcave[2]].x) * (contourHand[listconcave[2]].x);                               
    
	//valeur de x 
	Matrice[0][1]=contourHand[listconcave[0]].x;
	Matrice[1][1]=contourHand[listconcave[1]].x;           
    Matrice[2][1]=contourHand[listconcave[2]].x;
	
	 // les 1 
	 Matrice[0][2]=1;
     Matrice[1][2]=1;  
     Matrice[2][2]=1;     							 

	  //valeur de y
	 Matrice[0][3]=contourHand[listconcave[0]].y;
     Matrice[1][3]=contourHand[listconcave[1]].y;
     Matrice[2][3]=contourHand[listconcave[2]].y;
	
	 // l'application de la méthode
     cout <<"Avant le calcule "<<endl;
    for(i=0;i<3;i++)
    {
     for(j=0;j<4;j++)
      {
      cout << Matrice[i][j]<<"  ";
      }
      cout<<endl;
    }
    //remplissage de la 1 ligne;
    //remplissage des lignes restantes avec la methode
    for( i=0;i<2;i++)
    {
    for(j=i+1;j<3;j++)
    {
    stockcase=Matrice[j][i];
    for(k=0;k<4;k++)
    {
    Matrice[j][k]= Matrice[j][k]- stockcase/Matrice[i][i]*Matrice[i][k];
    }
    }
    }
    //l'affichage de la matrice
     cout <<"Apres le calcule le calcule "<<endl;
    for(i=0;i<3;i++)
    {
     for(j=0;j<4;j++)
      {
      cout << Matrice[i][j]<<"  ";
      }
      cout<<endl;
    }
 z=Matrice[2][3]/Matrice[2][2];
 y=(Matrice[1][3]-Matrice[1][2]*z)/Matrice[1][1];
 x=(Matrice[0][3]-Matrice[0][2]*z-Matrice[0][1]*y)/Matrice[0][0];
 
// ltest->Text="Equation courbe de jonction:	"+x+" x²+	"+y+" x+	"+z;
	
	//dessiner la courbe de jonction
	dessiner_courbe_jonction(dst2,contourHand,listconcave,listconcave.size(),x,y,z,numImg,boundRect1);
	
	}

}
/*****************************************************************************/
void filtrage_concave_courbe(vector<cv::Point> concaves,int size,vector<cv::Point> *listfiltrer,int *ressize)
{
	int i=0,j=0;
	bool elimin=false;
	int k=0;
	
	
		for(i=0;i<size - 1;i++)
		{
		j=i+1;
		elimin=false;
		while( j< size && elimin== false)
		{
		if(concaves[i].x == concaves[j].x )
			elimin=true;
	
			j++;
		}

			if(elimin == false) //insérer le point convexe
			{
			(*listfiltrer)[k]=concaves[i];
			k=k+1;
			}
        }

		//insérer le dernier point convexe
		if(size!=0){
		(*listfiltrer)[k]=concaves[i];
			k=k+1;
		}

*ressize=k;

 /*for(int i=0;i<k;i++)
	 {
	
	 cv::circle(*img, (*res)[i], 4,CV_RGB(255,0,0),0.1);
	 cv::line(*img, mc, (*res)[i], CV_RGB(60,48,152), 1, CV_AA, 0);
	 cv::imshow("Points convexes Masse",*img);
	 cv::waitKey(0);
	 }*/

}

 /****************************************************************************/
void courbe_jonction_vectconcave(vector<cv::Point> listconcave,int num_concaves,vector<cv::Point> contourHand,cv::Mat dst2,int numImg,cv::Rect boundRect1)
{

	if(num_concaves >=3) //3 points ou plus pr dessiner la courbe de jonction
	{
    double Matrice[3][4],stockcase;
    double Calcul[3][4];
    int i,j,k;
    double x,y,z;
    //remplissage de la matrice
	//valeur des x2              
    Matrice[0][0]=(listconcave[0].x) * (listconcave[0].x);  
    Matrice[1][0]=(listconcave[1].x) * (listconcave[1].x);							   
    Matrice[2][0]=(listconcave[2].x) * (listconcave[2].x);                               
    
	//valeur de x 
	Matrice[0][1]=listconcave[0].x;
	Matrice[1][1]=listconcave[1].x;           
    Matrice[2][1]=listconcave[2].x;
	
	 // les 1 
	 Matrice[0][2]=1;
     Matrice[1][2]=1;  
     Matrice[2][2]=1;     							 

	  //valeur de y
	 Matrice[0][3]=listconcave[0].y;
     Matrice[1][3]=listconcave[1].y;
     Matrice[2][3]=listconcave[2].y;
	
	
    //remplissage de la 1 ligne;
    //remplissage des lignes restantes avec la methode
    for( i=0;i<2;i++)
    {
    for(j=i+1;j<3;j++)
    {
    stockcase=Matrice[j][i];
    for(k=0;k<4;k++)
    {
    Matrice[j][k]= Matrice[j][k]- stockcase/Matrice[i][i]*Matrice[i][k];
    }
    }
    }
 
 z=Matrice[2][3]/Matrice[2][2];
 y=(Matrice[1][3]-Matrice[1][2]*z)/Matrice[1][1];
 x=(Matrice[0][3]-Matrice[0][2]*z-Matrice[0][1]*y)/Matrice[0][0];
 
// ltest->Text="Equation courbe de jonction:	"+x+" x²+	"+y+" x+	"+z;
	
	//dessiner la courbe de jonction
	dessiner_courbe_jonction2(dst2,contourHand,listconcave,listconcave.size(),x,y,z,numImg,boundRect1);
	
	}

}

/************************************************************************************/
void courbe_jonction_pouce(vector<cv::Point> listconcave,int num_concaves,vector<cv::Point> contourHand,cv::Mat dst2,int numImg,cv::Rect boundRect1)
{

	if(num_concaves >=3) //3 points ou plus pr dessiner la courbe de jonction
	{
    double Matrice[3][4],stockcase;
    double Calcul[3][4];
    int i,j,k;
    double x,y,z;
    //remplissage de la matrice
	//valeur des x2              
    Matrice[0][0]=(listconcave[0].y) * (listconcave[0].y);  
    Matrice[1][0]=(listconcave[1].y) * (listconcave[1].y);							   
    Matrice[2][0]=(listconcave[2].y) * (listconcave[2].y);                               
    
	//valeur de x 
	Matrice[0][1]=listconcave[0].y;
	Matrice[1][1]=listconcave[1].y;           
    Matrice[2][1]=listconcave[2].y;
	
	 // les 1 
	 Matrice[0][2]=1;
     Matrice[1][2]=1;  
     Matrice[2][2]=1;     							 

	  //valeur de y
	 Matrice[0][3]=listconcave[0].x;
     Matrice[1][3]=listconcave[1].x;
     Matrice[2][3]=listconcave[2].x;
	
	
    //remplissage de la 1 ligne;
    //remplissage des lignes restantes avec la methode
    for( i=0;i<2;i++)
    {
    for(j=i+1;j<3;j++)
    {
    stockcase=Matrice[j][i];
    for(k=0;k<4;k++)
    {
    Matrice[j][k]= Matrice[j][k]- stockcase/Matrice[i][i]*Matrice[i][k];
    }
    }
    }
 
 z=Matrice[2][3]/Matrice[2][2];
 y=(Matrice[1][3]-Matrice[1][2]*z)/Matrice[1][1];
 x=(Matrice[0][3]-Matrice[0][2]*z-Matrice[0][1]*y)/Matrice[0][0];
 
// ltest->Text="Equation courbe de jonction:	"+x+" x²+	"+y+" x+	"+z;
	
	//dessiner la courbe de jonction
	dessiner_cj2_pouce(dst2,contourHand,listconcave,listconcave.size(),x,y,z,numImg,boundRect1);
	
	}

}
/*************************************************************/
   //fonction qui calcul le y d'un x donné selon l'équation de la courbe de jonction ax² + bx +c,retourne -1 si pas de solution
 double solution_y(int x,double a,double b,double c)
 {
    double y=-1,res1=0,res2=0;
	 res1 = a * x * x;
	 res2 = b * x;
     y= res1 + res2 + c;
	 return y;
 }
  
 /******************************************************************************/

 void dessiner_courbe_jonction(cv::Mat img,vector<cv::Point> contourHand,vector<int> listconcave,int sizeconcave,double a,double b,double c,int numImg,cv::Rect RectHand)
 {
	 double y_p1,y_p2,y_p3 , y_p;
	 y_p1= solution_y (contourHand[listconcave[0]].x,a,b,c);
	 y_p2= solution_y (contourHand[listconcave[1]].x,a,b,c);
	 y_p3= solution_y (contourHand[listconcave[2]].x,a,b,c);
	 cv::Point p;

// for(int i=RectHand.tl().x;i < RectHand.width;i++)
 for(int i=0;i < contourHand.size() ; i++)
	 {
	 y_p= solution_y (contourHand[i].x,a,b,c);
  if( y_p != -1)
  {
  cv::circle(img, cv::Point(contourHand[i].x,y_p), 0.1, cv::Scalar(0, 0, 255), 2); 
  /*if( contourHand[i].y < y_p1 && contourHand[i].y < y_p2 && contourHand[i].y < y_p3 )
	  cv::circle(img, cv::Point(contourHand[i].x,y_p), 1, cv::Scalar(45, 168, 255), 2); */
  }
  
 }
 string nom_courbe;
 nom_courbe=concatene_nom_img(nb_alea,"courbe_jonction");
 cv::imwrite(nom_courbe,img);
 cv::imshow("dessiner courbejonction",img);
 cv::waitKey(0);

 }
 /*****************************/
 void dessiner_courbe_jonction2(cv::Mat img,vector<cv::Point> contourHand,vector<cv::Point> listconcave,int sizeconcave,double a,double b,double c,int numImg,cv::Rect RectHand)
 {
	 double y_p1,y_p2,y_p3 , y_p;
	 cv::Point p;
	 if(a != 0 ){
// for(int i=RectHand.tl().x;i < RectHand.width;i++)
 for(int i=0;i < contourHand.size() ; i++)
	 {
	 y_p= solution_y (contourHand[i].x,a,b,c);
  if( y_p != -1)
  {
  cv::circle(img, cv::Point(contourHand[i].x,y_p), 0.1, cv::Scalar(0, 0, 255), 2); 
  /*if( contourHand[i].y < y_p1 && contourHand[i].y < y_p2 && contourHand[i].y < y_p3 )
	  cv::circle(img, cv::Point(contourHand[i].x,y_p), 1, cv::Scalar(45, 168, 255), 2); */
  }
  
 }
 string nom_courbe;
 nom_courbe=concatene_nom_img(nb_alea,"new_courbe_jonction");
 cv::imwrite(nom_courbe,img);
 /*cv::imshow("dessiner courbe jonction calcul moindre carré",img);
 cv::waitKey(0);
 */
	 }
 }
 /*******************************************/
 void dessiner_cj2_pouce(cv::Mat img,vector<cv::Point> contourHand,vector<cv::Point> listconcave,int sizeconcave,double a,double b,double c,int numImg,cv::Rect RectHand)
 {
	 double  x_p;
	 cv::Point p;
	 if( a !=0 ){

 for(int i=0;i < contourHand.size() ; i++)
	 {
	 x_p= solution_y (contourHand[i].y,a,b,c);
  if( x_p != -1)
  {
  cv::circle(img, cv::Point(contourHand[i].y,x_p), 0.1, cv::Scalar(255, 0, 255), 1); 
  /*if( contourHand[i].y < y_p1 && contourHand[i].y < y_p2 && contourHand[i].y < y_p3 )
	  cv::circle(img, cv::Point(contourHand[i].x,y_p), 1, cv::Scalar(45, 168, 255), 2); */
  }
  
 }
 string nom_courbe;
 nom_courbe=concatene_nom_img(nb_alea,"cj2");
 cv::imwrite(nom_courbe,img);
/* cv::imshow("cj2 pouce",img);
 cv::waitKey(0);
	*/
	}
 }
 /*****************************/
 /*****************************/
 void capture_background(cv::Mat *bg, cv::VideoCapture cap)
 {
 int i;
 //VideoCapture cap(0);
 for(i=0;i<60;i++)
 {
 cap.grab();
 cap.retrieve(*bg,CV_CAP_OPENNI_BGR_IMAGE);
 }
 cv::imshow("Background",*bg);
 cv::waitKey(0);

 }
 
 /******************************/
 void acquisition_image_boucle(cv::Mat bg,cv::Mat *imggest,cv::Mat *RGBgest,cv::Mat *RGB,int nb_alea,cv::VideoCapture cap)
 {
 cv::Mat fg;
 int i;
//cap>>bg;
   cap.grab();
   cap.retrieve(*RGB,CV_CAP_OPENNI_BGR_IMAGE);
   imwrite("RGB10.jpg",*RGB);
   
    cvtColor(*RGB, fg, CV_RGB2YCrCb);

    cvtColor(bg, bg, CV_RGB2YCrCb);
	*RGBgest=fg;
    cv::Mat distance = cv::Mat::zeros(fg.size(), CV_32F);

    vector<cv::Mat> fgChannels;
    cv::split(fg, fgChannels);

    vector<cv::Mat> bgChannels;
    cv::split(bg, bgChannels);

    for(size_t i = 0; i < fgChannels.size(); i++)
    {
        cv::Mat temp = abs(fgChannels[i] - bgChannels[i]);
        temp.convertTo(temp, CV_32F);

        distance = distance + temp;
    }


    cv::Mat mask ;
    cv::threshold(distance, mask, 70, 255, cv::THRESH_BINARY);

    //cv::Mat kernel5x5 = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    //morphologyEx(mask, mask, cv::MORPH_OPEN, kernel5x5);
 
/* string  nom_mask=concatene_nom_img(nb_alea,"mask");
	 cv::imwrite(nom_mask,mask); */
     *imggest=mask;
    
}
 /*****************************/
 void acquisition_image_geste(cv::Mat *imggest,cv::Mat *RGBgest,cv::Mat *RGB,int nb_alea)
 {
 cv::Mat fg,bg;
 int i;
//VideoCapture cap(0);
cv::VideoCapture cap;
cap.open(CV_CAP_OPENNI);
for(i=0;i<60;i++)
 {
 cap.grab();
 cap.retrieve(bg,CV_CAP_OPENNI_BGR_IMAGE);
 }
 imshow("background",bg);
 cvWaitKey(0);

//cap>>bg;
for( i=0;i<60;i++)
    //Mat fg = imread("fg.jpg");
    {cap.grab();
 cap.retrieve(*RGB,CV_CAP_OPENNI_BGR_IMAGE);}
   imwrite("RGB10.jpg",*RGB);
   
    cvtColor(*RGB, fg, CV_RGB2YCrCb);
    cvtColor(bg, bg, CV_RGB2YCrCb);
	*RGBgest=fg;
    cv::Mat distance = cv::Mat::zeros(fg.size(), CV_32F);

    vector<cv::Mat> fgChannels;
    cv::split(fg, fgChannels);

    vector<cv::Mat> bgChannels;
    cv::split(bg, bgChannels);

    for(size_t i = 0; i < fgChannels.size(); i++)
    {
        cv::Mat temp = abs(fgChannels[i] - bgChannels[i]);
        temp.convertTo(temp, CV_32F);

        distance = distance + temp;
    }


    cv::Mat mask ;
    cv::threshold(distance, mask, 70, 255, cv::THRESH_BINARY);

    //cv::Mat kernel5x5 = getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    //morphologyEx(mask, mask, cv::MORPH_OPEN, kernel5x5);

    cv::imshow("fg", fg);
    cv::waitKey(0);
    cv::imshow("bg", bg);
    cv::waitKey(0);
    cv::imshow("mask", mask);
    string nom_mask;
   	 nom_mask=concatene_nom_img(nb_alea,"mask");
	 cv::imwrite(nom_mask,mask);
   *imggest=mask;
    cv::waitKey(0);
}
  /*******************************************************************/
 void suppression_bras(cv::Mat *img,cv::Rect boundRect )
 {
 
 int seuil_y= boundRect.tl().y +boundRect.width;
   for(int i=seuil_y; i< (*img).rows ;i++)
	   for(int j=0;j< (*img).cols; j++)
	   {

		   (*img).at<cv::Vec3b>(i,j)[0]=0;
		   (*img).at<cv::Vec3b>(i,j)[1]=0;
		   (*img).at<cv::Vec3b>(i,j)[2]=0;

	   }
 
 }
/***********************************************************************/
  void suppression_bras2(cv::Mat *img,cv::Rect boundRect )
 {
 
 int seuil_y= boundRect.tl().y +70;
   for(int i=seuil_y; i< (*img).rows ;i++)
	   for(int j=0;j< (*img).cols; j++)
	   {

		   (*img).at<cv::Vec3b>(i,j)[0]=0;
		   (*img).at<cv::Vec3b>(i,j)[1]=0;
		   (*img).at<cv::Vec3b>(i,j)[2]=0;

	   }
 
 }
 /*******************************************************************/
 //trouver points convexes à partir de centre de masse
 void find_convexe(cv::Point2f center_of_mass,std::vector<cv::Point> points,cv::Mat *img,int numImg,int *num,vector<cv::Point> *fingersfnl)
 {
    int  dist1=0,dist2=0;
    cv::Point max_point = cv::Point(0,0);
    cv::Point min_point = cv::Point(0,0);
    min_point = center_of_mass;
    int NUM_FINGERSt=10;
    vector<cv::Point> fingers(NUM_FINGERSt);
	vector<double> finger_distance(10);
	int  num_fingers = 0;
 
    for (int i = 0; i <points.size(); i++) 
	{
        int dist;
        int cx = center_of_mass.x;
        int cy = center_of_mass.y;

        //cvCircle(image, points[i], 0.1,CV_RGB(0,0,0),0.1);
		double a=(cx - points[i].x) * (cx - points[i].x);
		double b= (cy - points[i].y) * (cy - points[i].y);
        dist = sqrt(a+b);

       if (dist < dist1 && dist1 > dist2 && max_point.x != 0 && max_point.y < min_point.y)
        {
            finger_distance[num_fingers] = dist;
            fingers[num_fingers++] = max_point;
            if(num_fingers>= NUM_FINGERSt)
                break;
        }

        dist2 = dist1;
        dist1 = dist;
        max_point = points[i];
    }

	/* for(int i=0;i<num_fingers;i++)
	 {
	
	 cv::circle(*img, fingers[i], 4,CV_RGB(0,255,255),0.1);
	 cv::line(*img, center_of_mass, fingers[i], CV_RGB(255,255,0), 1, CV_AA, 0);
	 cv::imshow("Points convexes Masse",*img);
	 cv::waitKey(0);
	 }
     string nom_convexe;
   	 nom_convexe=concatene_nom_img(numImg,"pconvexe");
	 cv::imwrite(nom_convexe,*img);*/
 
	 *num=num_fingers;
	 *fingersfnl=fingers;
 }

 /*******************************************************************/
 //trouver points convexes à partir de centre de masse
 void find_concave(cv::Point2f center_of_mass,std::vector<cv::Point> points,cv::Mat *img,int numImg,int *num,vector<cv::Point> *concaves)
 {
    int  dist1=0,dist2=0;
    cv::Point max_point = cv::Point(0,0);
    cv::Point min_point = cv::Point(0,0);
    min_point = center_of_mass;
    int NUM_FINGERSt=10;
    vector<cv::Point> fingers(NUM_FINGERSt);
    vector<double> finger_distance(10);
    int  num_fingers = 0;
    cv::Point pouce(0,0);

    for (int i = 0; i <points.size(); i++) 
	{
        int dist;
        int cx = center_of_mass.x;
        int cy = center_of_mass.y;
		
		bool trouvePouce=false;
        //cvCircle(image, points[i], 0.1,CV_RGB(0,0,0),0.1);
		double a=(cx - points[i].x) * (cx - points[i].x);
			double b= (cy - points[i].y) * (cy - points[i].y);
        dist = sqrt(a+b);

       if (dist > dist1 && dist1 < dist2 && max_point.x != 0)
        {
			if(max_point.y < min_point.y)
			{
            finger_distance[num_fingers] = dist;
            fingers[num_fingers++] = max_point;
            if(num_fingers >= NUM_FINGERSt)
				break;
			}
/*			else  //point concave sous centre gravité, on cherche celui du pouce 
			{
				if(max_point.y < min_point.y +10)
				{
					trouvePouce=true;
				pouce.x = max_point.x;
				pouce.y = max_point.y;
				}
			
			}*/
        }


        dist2 = dist1;
        dist1 = dist;
        max_point = points[i];
    }

	 for(int i=0;i<num_fingers;i++)
	 {
		 
	// cv::circle(*img, fingers[i], 4,CV_RGB(0,0,255),0.1);
	 //cv::line(*img, center_of_mass, fingers[i], CV_RGB(255,255,0), 1, CV_AA, 0);
	/* cv::imshow("Points concaves Masse",*img);
	 cv::waitKey(0);*/
	 }
	/* cv::circle(*img, pouce, 4,CV_RGB(0,255,0),0.1);
	 cv::imshow("Pouce",*img);
	 cv::waitKey(0);*/

     string nom_convexe;
   	 nom_convexe=concatene_nom_img(numImg,"pconcave");
	 cv::imwrite(nom_convexe,*img);
    
	 *num=num_fingers;
	 *concaves=fingers;
 }

 /********************************************************/
 /*****************************************************************************/
  void eliminer_vallees(vector<cv::Point> concaves,int size,vector<cv::Point> *res,int *sizeres,cv::Mat *img,cv::Point2f mc,int nb_alea)
{
	int i=0,j=0;
	bool elimin=false;
	double dist=0;
	int k=0,p=0;
	double seuil=8;
	int taille=taille_vect(size-1);
	vector<double> distance(taille);
		for(i=0;i<size - 1;i++)
		{
		j=i+1;
		elimin=false;
		while( j< size && elimin== false)
		{
		dist=calculer_distance ( concaves[i],concaves[j]);
		distance[p]=dist; p++;

		if(dist < seuil )
			elimin=true;

			j++;
		}

			if(elimin == false) //insérer le point convexe
			{
			(*res)[k]=concaves[i];
			k=k+1;
			}
        }

		//insérer le dernier point convexe
		if(size > 0)
		{
		(*res)[k]=concaves[i];
		k=k+1;
		}


*sizeres=k;

 for(int i=0;i<k;i++)
	 {

	 cv::circle(*img, (*res)[i], 4,CV_RGB(255,255,0),0.1);
	 cv::line(*img, mc, (*res)[i], CV_RGB(60,48,152), 1, CV_AA, 0);
	 }

    // cv::imshow("vallées",*img);
	// cv::waitKey(0);
     string nom_pics;
//   	 nom_pics=concatene_nom_img(nb_alea,"pconcaveeliminer");
	// cv::imwrite(nom_pics,*img);

}

  /***************************************************************************/
void trier_concaves_jonction(vector<cv::Point> *concaves,int size)
{
	cv::Point temp(0,0);
	bool trouve=true;

	while( trouve == true )
	{
		trouve=false;
	 for(int j=0;j<size-1;j++)
	 {
		 if((*concaves)[j].x > (*concaves)[j+1].x )
		 {
		 temp=(*concaves)[j];
		 (*concaves)[j]= (*concaves)[j+1];
		  (*concaves)[j+1]=temp;
		  trouve=true;
		 }

	 }//fin for
	} //fin while

}

  /***************************************************************************/

 cv::Point ymin_concave(vector<cv::Point> list,int size)
 {
 cv::Point min=list[0];
 for(int i=0;i<size;i++)
 {

 if(list[i].y < min.y)
	 min=list[i];

 }

 return min;

 }


  /***************************************************************************/
void Choisir_concaves_valide(vector<cv::Point> list,int size,vector<cv::Point> *res,int *sizeres)
  {
	  cv::Point ymin(0,0);

   int diff_y=640; // height de l'image
   if(size <= 3)
   { *res=list;
     *sizeres=size;
   }
   else
   { // on procède au choix des points concaves
	   ymin= ymin_concave(list,size);
       (*res)[0]=ymin;
 	   for(int i=0;i<size;i++)
	   {
	   if( list[i] != ymin && ( (list[i].y - ymin.y) < diff_y) )
	   { (*res)[1]=list[i];
		 diff_y= (list[i].y - ymin.y);
	   }
	   }

	   diff_y=640;
	   for(int i=0;i<size;i++)
	   {
	   if( list[i] != ymin && list[i] !=(*res)[1] && ( (list[i].y - ymin.y) < diff_y) )
	   { (*res)[2]=list[i];
		 diff_y= (list[i].y - ymin.y);
	   }
	   }
    *sizeres=3;
   }

  }
 /********************************************************/
cv::Point chercher_pouce_concave(vector<cv::Point> concave,int size)
 {
 double dist;
 cv::Point pouce(0,0);
 bool distanceInf=false;
 double seuil=50;
vector<double> distance(taille_vect(size-1));
for(int i=0;i<size;i++)
{
 int j=0;
  while(j<size && distanceInf==false)
  {
  if (concave[i]!=concave[j])
  {
  dist=calculer_distance(concave[i],concave[j]);
  if(dist<seuil)
  {
  distanceInf=true;
  }

  }
  j++;
  }
  if(j==size)
  {
  return concave[i];
  }
}
return pouce;


 }
/*********************************************************/
 cv::Point min_concave_y(vector<cv::Point> concaves,int nbconcave)
 {
 cv::Point min(0,0);
	 if(nbconcave >0)
	 min=concaves[0];
 for(int i=1; i< nbconcave;i++)
 {
 if(concaves[i].y < min.y)
   min=concaves[i];
 }
 
 return min;
 }

/***********************************************************/
 int indice_point_concave_pouce(vector<cv::Point> contourHand,cv::Point pouce )
 {
 
	 int indice=-1,i=0,trouve=false;
     while( (i < contourHand.size()) && trouve==false)
	 {
		 if( (contourHand[i].x == pouce.x) && (contourHand[i].y != pouce.y) )
		 {
			 indice=i;
			 trouve=true;
		 }
	 i++;
	 }

  return indice; //tester par rapport à -1 
 }
/**********************************************************/
 void calcul_dessin_cj2(cv::Mat* img,cv::Point pouce,cv::Point ppouce,vector<cv::Point> contourHand,int numImg,cv::Rect boundRect1)
 {
 cv::Point milieu(0,0);
  milieu.x =(pouce.x + ppouce.x)/2;
  milieu.y =(pouce.y + ppouce.y)/2;
 vector<cv::Point> points(3);
 points[0]=pouce;
 points[1]=ppouce;
 points[2].x=milieu.x+3;
 points[2].y=milieu.y;

 cv::circle(*img, pouce, 4,CV_RGB(100,200,255),2);
 cv::circle(*img, ppouce, 4,CV_RGB(60,30,255),2);
 cv::circle(*img, milieu, 4,CV_RGB(255,70,90),2);
 cv::circle(*img, points[2], 4,CV_RGB(255,100,55),2);

 courbe_jonction_pouce(points,points.size(),contourHand,*img, numImg,boundRect1);
 }
 /*******************************************************/
 cv::Point   chercher_pouce(vector<cv::Point> convexes,int nbconvexe,cv::Point minconcave,cv::Point2f mc)
 {
	 vector<cv::Point> p_candidat(nbconvexe);
 cv::Point pouce(0,0);
 double distmax=0;
 int j =0,i=0;
 bool trouve=false;
  if(minconcave != cv::Point(0,0)){
 while(j <nbconvexe )
 {
 if((convexes[j].y  > minconcave.y ) || (convexes[j].y > minconcave.y - 5))
 {
 p_candidat[i]=convexes[j];
 i++;
 }
 j++;
 }

 for(j=0;j < i; j++)
 {
 if(calculer_distance(mc,p_candidat[j]) >distmax)
	 pouce=p_candidat[j];

 }
 }
 return pouce;
 
 }

 /*********************************************************/
 void find_concave_masse(vector<cv::Point> contourHand,cv::Point2f center_of_mass,vector<cv::Point> *concaves,int *size,cv::Mat *img,int nb_alea)
 {
 int NUM_CONCAVES=10;
// convexityDefects = cvConvexityDefects(contour, convexHull2);

vector<cv::Point>  pointsConvexe(contourHand.size());
vector<int> pointsConvexeI(contourHand.size());
cv:: Point pouce;
 cv::convexHull( cv::Mat(contourHand), pointsConvexe, false);
 cv::convexHull( cv::Mat(contourHand), pointsConvexeI, false);
 std::vector<cv::Vec4i> convdefect(contourHand.size());
 if(pointsConvexeI.size() > 3 )
 cv::convexityDefects(contourHand,pointsConvexeI,convdefect);

 cv::Mat dst=(*img).clone();
 std::vector<cv::Vec4i>::iterator d=convdefect.begin();
 int num_concaves=0;
 bool pouceDetecte=false;

    while( d != convdefect.end() )
   // for(int j = 0; j < convexityDefects -> total; ++j)
    {
       // cDef = CV_GET_SEQ_ELEM( CvConvexityDefect, convexityDefects,j);
		cv::Vec4i& v=(*d);
		int faridx=v[2];
		float depth=v[3]/256;
       cv::Point ptFar( contourHand[faridx] );

        if(depth > 40 && depth < 90 )
        {
            //cout << "depth =  " << cDef->depth << endl;
            if(ptFar.y < center_of_mass.y)
            {

                (*concaves)[num_concaves].x = ptFar.x;
                (*concaves)[num_concaves].y = ptFar.y;
                num_concaves++;
            }
            else
				if(pouceDetecte==false) // if pouce.y > ptFar.y 
            {
                pouceDetecte = true;
                pouce.x = ptFar.x;
                pouce.y = ptFar.y;
            }

            if(num_concaves >= NUM_CONCAVES)
                    break;
        }

		d++;
    }
 
     for(int i=0;i<num_concaves;i++)
	 {
		 
	 cv::circle(*img, (*concaves)[i], 4,CV_RGB(0,0,255),1);
	 cv::line(*img, center_of_mass, (*concaves)[i], CV_RGB(255,255,0), 1, CV_AA, 0);
	/* cv::imshow("Dans concave masse",*img);
	 cv::waitKey(0);*/
	 }
	 cv::circle(*img, pouce, 4,CV_RGB(255,128,0),1);
	  cv::imshow("concave masse depth",*img);
	 cv::waitKey(0);
    /* string nom_convexe;
   	 nom_convexe=concatene_nom_img(nb_alea,"pconcave");
	 cv::imwrite(nom_convexe,*img);
	 */
 *size=num_concaves;
 }
 /*************************************************************/
 void courbe(int num_fingers,vector<cv::Point> concaves,vector<cv::Point> fingers,int num_concaves,cv::Point2f center_of_mass,vector<cv::Point> contourHand,cv::Mat img,int nb_alea,double a1,double b1,double c1)
{
double a,b,c,d,e,f;
    if(num_concaves < 10) 
	{
    if(num_concaves >= 3)
    {
           // ui->pointsSup->setText(QString::number(0));
            NewmoindreCarre(num_concaves,concaves,contourHand,img,nb_alea,&a1,&b1,&c1);  
    }
    else if(num_concaves > 0)
    {

        int pointsSup =0;
        double dist1, dist2;
        cv::Point minPoint;
        minPoint = cv::Point(-1,-1);
        for(int i=0; i < num_concaves; i++)
        {
            // calculer la distance
            //cvCircle(image, concaves[i], 2, CVX_BLUE, 2);
			 a=(minPoint.x - center_of_mass.x)*(minPoint.x - center_of_mass.x);
			 b=(minPoint.y - center_of_mass.y)*(minPoint.y - center_of_mass.y);
            dist1 = sqrt(a+b);
			 c=(concaves[i].x - center_of_mass.x)*(concaves[i].x - center_of_mass.x);
			 d=(concaves[i].y - center_of_mass.y)*(concaves[i].y - center_of_mass.y);
            dist2 = sqrt(c+d);

            if( dist1>dist2)
                minPoint.x = concaves[i].x;
                minPoint.y = concaves[i].y;
        }
        e=(minPoint.x - center_of_mass.x)*(minPoint.x - center_of_mass.x);
		f=(minPoint.y - center_of_mass.y)*(minPoint.y - center_of_mass.y);
        dist1 = sqrt(e+f);


        for(int i=0; i< num_fingers; i++)
        {
           // intersection(fingers[i], dist1);
			intersection(fingers[i], dist1,center_of_mass,concaves,num_concaves,pointsSup);
        }
        cv::Point ptmp;
        //trier le tabeau des concaves
    for(int i=0; i < num_concaves-1; i++)
    {
        for(int j=i+1;j<num_concaves;j++)
        {
            if(concaves[j].x<concaves[i].x)
            {
                ptmp = concaves[i];
                concaves[i] = concaves[j];
                concaves[j] = ptmp;
            }
        }
        //cout << "concave1 " << i << " : " << concaves[i].x << ","  << concaves[i].y << endl;
    }
    //cout << "concave1 " << num_concaves << " : " << concaves[num_concaves-1].x << ","  << concaves[num_concaves-1].y << endl;
        NewmoindreCarre(num_concaves,concaves,contourHand,img,nb_alea,&a1,&b1,&c1);
       // controle();
    }
	}
    
}
/************************************************************/
void NewmoindreCarre(int num_concaves,vector<cv::Point> concaves,vector<cv::Point> contourHand,cv::Mat img,int nb_alea,double *aa,double *bb,double *cc)
{
    cv::Rect rect = cv::boundingRect(contourHand);
    double SX=0,SX2=0,SX3=0,SX4=0,SYX=0,SYX2=0,SY=0;
    double d,a,b,c;
    int t,s;
  
    for(int i=1; i<num_concaves; i++)
    {
    // calculer les sommes

    t = concaves[i].x;
    s = concaves[i].y;
    SX += t;
    SY += s;
    SX2 += t*t;
    SX3 += t*t*t;
    SX4 +=  t*t*t*t;
    SYX += t * s;
    SYX2 += (t*t) * s;
    }

    d = SX4 * (num_concaves * SX2 - (SX*SX)) + SX3 * (SX * SX2 - num_concaves * SX3) + SX2 * (SX * SX3 - (SX2*SX2));
    c = (SX4 * (SX2 * SY - SX * SYX) + SX3 * (SX * SYX2 - SX3 * SY) + SX2 * (SX3 * SYX - SX2 * SYX2)) / d;
    b = (SX4 * (num_concaves * SYX - SY * SX) + SX3 * (SY * SX2 - num_concaves * SYX2) + SX2 * (SYX2 * SX - SYX * SX2)) / d;
    a = (SYX2 * (num_concaves * SX2 - (SX*SX)) + SYX * (SX * SX2 - num_concaves * SX3) + SY * (SX * SX3 - (SX2*SX2))) / d;

	*aa=a;
	*bb=b;
	*cc=c;
   /* if(a > 0 && c > 300 && b > -15)
    {
        //affichage de l'equation
       /* ui->constanteA->setText(QString::number(a));
        ui->constanteB->setText(QString::number(b));
        ui->constanteC->setText(QString::number(c));
        QString equation;
        equation +="Y = "+QString::number(a)+"X² ";
        if(b>0)
            equation+=" + ";
        equation+=QString::number(b)+"X ";
        if(c>0)
            equation+=" + ";
        equation+= QString::number(c);
        ui->infoEquation->setText(equation);
      */
     /*   double res;
        for(int x=rect.x; x< rect.x + rect.width;x++)
        {
            for(int y=rect.y;y<rect.y+rect.height;y++)
            {
                res = (x*x)*a + x*b + c - y;
                if((int)res == 0)
                {
                    //if(ui->afficherCourbe->isChecked())
                        cv::circle(img, cv::Point(x,y), 1, CV_RGB(255,255,255),1);
                    //courbeOk = true;
					cv::imshow("new moindre carré",img);
						cv::waitKey(0);
                }
            }
        }
    }*/


/* string nom_courbe;
   	 nom_courbe=concatene_nom_img(nb_alea,"CJONCTION");
	 cv::imwrite(nom_courbe,img); */
}


/**********************************************/
void intersection(cv::Point doigt, double r,cv::Point2f center_of_mass,vector<cv::Point> concaves,int num_concaves,int pointsSup)
{
    double t;
    double dist;
    int x1,x2,y1,y2,x,y;
    x1 = doigt.x;
    x2 = center_of_mass.x;
    y1 = doigt.y;
    y2 = center_of_mass.y;

	double sq1=(x1-x2 * x1-x2 );
	double sq2=(y1-y2 * y1-y2);
    dist = sqrt( sq1+ sq2);
    t=r/dist;

    x = t*x1+(1-t)*x2;
    y = t*y1+(1-t)*y2;



    if(y < (center_of_mass.y-10))
    {
             bool ajout = true;
             for(int k=0;k<num_concaves;k++)
             {
                 if(abs(x-concaves[k].x)<5)
                 {
                    ajout = false;
                 }
             }
             if(ajout == true)
             {
                 concaves[num_concaves].x = x;
                 concaves[num_concaves].y = y;
                 num_concaves++;
                 pointsSup++;
             }

    }

    //ui->pointsSup->setText(QString::number(pointsSup));

}
 /********************************************************/
void eliminer_pics(vector<cv::Point> convexes,int size,vector<cv::Point> *res,int *sizeres,cv::Mat *img,cv::Point2f mc,int nb_alea)
{
	int i=0,j=0;
	bool elimin=false;
	double dist=0;
	int k=0,p=0;
	double seuil=20;
	int taille=taille_vect(size-1);
	vector<double> distance(taille);
		for(i=0;i<size - 1;i++)
		{
		j=i+1;
		elimin=false;
		while( j< size && elimin== false)
		{
		dist=calculer_distance ( convexes[i],convexes[j]);
		distance[p]=dist; p++;
		
		if(dist < seuil )
			elimin=true;
	
			j++;
		}

			if(elimin == false) //insérer le point convexe
			{
			(*res)[k]=convexes[i];
			k=k+1;
			}
        }

		//insérer le dernier point convexe
		if(size > 0)
		{
		(*res)[k]=convexes[i];
		k=k+1;
		}


*sizeres=k;

 for(int i=0;i<k;i++)
	 {
	
	 cv::circle(*img, (*res)[i], 4,CV_RGB(255,0,0),0.1);
	 cv::line(*img, mc, (*res)[i], CV_RGB(60,48,152), 1, CV_AA, 0);
	 }

    /* cv::imshow("pics",*img);
	 cv::waitKey(0);*/
     string nom_pics;
   	 nom_pics=concatene_nom_img(nb_alea,"pics");
	 cv::imwrite(nom_pics,*img);

}
/***************************************************/
void acquisitionBackground(cv::Mat *depth, cv::VideoCapture cap)
{
int i;
cv::Mat Depth;

 for(i=0;i<30;i++)
 {
 cap.grab();
 cap.retrieve(*depth,CV_CAP_OPENNI_DEPTH_MAP);
 }
(*depth).convertTo(Depth,CV_8UC1,100.0/2048.0);
*depth=Depth;

}

/************************************/
void acquisitionGest(cv::Mat *Mask,cv::Mat *RGB,cv::Mat bg ,cv::VideoCapture cap)
{ int i;
	cv::Mat depth2,RGB1,Depth2,result,resultat,thresh,diff;
	for(i=0;i<90;i++){
    cap.grab();
    cap.retrieve(depth2,CV_CAP_OPENNI_DEPTH_MAP);
	cap.retrieve(*RGB,CV_CAP_OPENNI_BGR_IMAGE);}
    depth2.convertTo(Depth2,CV_8UC1,100.0/2048.0);
    cv::threshold(Depth2,result,20, 255, CV_THRESH_BINARY_INV);
     diff=bg-Depth2;
     cv::threshold(diff,resultat,20, 255, CV_THRESH_BINARY);
     cv::threshold(diff,thresh,35, 255, CV_THRESH_BINARY);
     *Mask=thresh-result;
}


/***********************************************************************************************/
/*int MainSimul(string rgb,string maskstart,array< Guide>^ vect_puzzle)
{	
	/************ MAIN QUI FAIT APPEL A LA KINECT **********************/
	//Initialiser l'interface de simulation
	/*array< Guide>^ vect_puzzle = gcnew array< Guide >(9);
    initialise_tableau(vect_puzzle);
	start_puzzle();*/
/*    int cpt=0;

while( cpt < 3) 
{

	//Application
	srand(time(NULL));
	nb_alea = (rand() % 10000) + 1; //génère un nombre entre 1 et 100 
	//System::String^ managedString = nb_alea.ToString();
    //msclr::interop::marshal_context context;
    //int numImg = context.marshal_as<string>(managedString);

	   /**** Prise de l'image du background ****/
/*	string imgtest,segtest;
	 cv::Mat imggest,RGBgest,RGB;
    // acquisition_image_geste(&imggest,&RGBgest,&RGB,nb_alea);
	 //photossss\\segkinect37.jpg

	if(cpt == 1 ) {imgtest="photossss\\bb7.jpg"; segtest="photossss\\aa7.jpg";}
	if(cpt == 0 )  {imgtest="photossss\\bb88.jpg"; segtest="photossss\\aa8.jpg";}
	if(cpt == 2)  {imgtest="photossss\\bb6.jpg"; segtest="photossss\\aa6.jpg";}

	cpt++;

    //imggest=cv::imread("photossss\\aa11.jpg"); 
	//RGB=cv::imread("photossss\\bb11.jpg");
	
/*	imggest=cv::imread(segtest); 
	RGB=cv::imread(imgtest);
	 cv::Mat rgbCourbe=RGB.clone();
	 string nom_rgb;
   	 nom_rgb=concatene_nom_img(nb_alea,"RGB");
	 cv::imwrite(nom_rgb,RGB);

	 	// String^ nomm="original.jpg";
		string str=nom_rgb;
		String^ nom_RGBimg= gcnew String(str.c_str());
//		LoadNewPict(nom_RGBimg,ImgOrigin);

	   /**** changer espace colorimétrique ****/
	
	/*    array< HSV >^ tab1 = gcnew array< HSV >(RGBgest.rows*RGBgest.cols);  
        for(int l=0;l< RGBgest.rows*RGBgest.cols ;l++)
			tab1[l] = HSV();
		array< ycbcr >^ tab2 = gcnew array< ycbcr >(RGBgest.rows*RGBgest.cols);
		
		RGBtoHSV(tab1,RGBgest);
		RGBtoYCbCr(tab2,RGBgest);
	*/

		/**** Segmentation Pronfondeur et Peau****/
	/*	//cv::Mat dst=segmentationVS(imggest,tab1,tab2);
		cv::Mat dst=segmentationDirecte(imggest,RGB);
		cv::imshow("segment",dst);
		cv::waitKey(0);
		string nom_seg;
		nom_seg=concatene_nom_img(nb_alea,"segment");
		cv::imwrite(nom_seg,dst);
	

	    /*** eliminer visage ****/
/*
	   
        face_eliminer(RGB,&dst,nb_alea);
        cv::imshow("No_face",dst);
        cv::waitKey(0);


	/**********************************************************/

/*	   cv::Rect boundRect;
	   cv::Mat  dst2 =dst.clone();
	 //  cv::Mat dst2=cv::imread("original.jpg");
	   //retrouver contour de la main seulement pr faire la paume et puis suppression
	   std::vector<cv::Point> contour;
	   detecterContours(dst2 ,&boundRect,&contour);

	  

	  /********************* Suppression du bras *************************/

	 /* suppression_bras(&dst2, boundRect );
	  cv::imshow("arm deleted",dst2);
	  string nom_delet;
	  nom_delet=concatene_nom_img(nb_alea,"deletarm");
	  cv::imwrite(nom_delet,dst2);
      cv::waitKey(0);
	  

	  /********************* Détecter contourHand aprés suppression ****************************/
	    //fonction qui retourne les indices des points de convexitydefects dans la liste contours
	  /*  cv::Rect boundRect1;
	    std::vector<cv::Point> contourHand;
	    std::vector<cv::Point> pointCVD= detecterContours_modifier(dst2 ,&boundRect1,&contourHand,nb_alea);
	    if(pointCVD.size() > 0 ) {
		std::vector<int> vect_indiceCVD;
		std::vector<int> indice =convert_indic(pointCVD ,contourHand);

		 /******** Trouver et Dessiner paume de la main ********/
	  
	/*  int max_ip=0,max_jp=0,taillePaumeMain,br_paume_y;
      int indic=construct_sous_Mat( boundRect1,dst2,&max_ip,&max_jp);
      taillePaumeMain=indic;
	  draw_paume_main(dst2,max_ip,max_jp,indic,boundRect1,&br_paume_y,nb_alea);
	  
	  /********************** Trouver centre de gravité	**********************/
/*	      cv::Point2f mc= find_centre_gravit(dst2, contourHand,nb_alea);
		 trouver_points_courbures(dst2,contourHand,nb_alea,mc,boundRect1 );

	   /************* Chetverecov  ***********************************/
	 
	 // vector<sharp> list_p;
	  
	/*  std::vector<int> resultat;
	  int length=0;
	  //cloner Mat F = A.clone(); copie A dans F,  A.copyTo(G) copie A dans G
	
	  find_corners(&resultat,20,10,190,contourHand,&length);
	  if(resultat.size() > 0 )
	  dessiner_points_forte_courbure(dst2,resultat,contourHand,length,nb_alea);
	  
     /************ fusionner points de chetverekov et convexitydefects **********************/
	
	  //fait la fusion trié des deux vecteurs d'indice des points chetverecov et points convexitydefects
	/*int taillefusion=0,concavesize=0,convexesize=0;
    std::vector<int> vect_fusion= fusion_trie(resultat ,indice,length,&taillefusion);
    
	
	/***************		Fractionner convexe/concave **************************/
    //nous retourne deux listes :points concaves /points convexes , on prends pas=10 pr trouver p- et p+
/*	array< Point_rest >^ lconcave = gcnew array< Point_rest >(taillefusion);
	array< Point_rest>^ lconvexe = gcnew array< Point_rest >(taillefusion);
    fraction_concave_convexe(dst2,lconcave,lconvexe,vect_fusion,contourHand,taillefusion,&concavesize,&convexesize);
	
 /*	 //trouver seuil avec
     vector<double> distance;
	 distance= distance_entre_points(lconvexe,convexesize,contourHand);
	 */
	/****** regroupement par distance et elimination par angle des points de fortes courbures ,
	retourne vecteur d'indice des points fortes courbures retenus ************************/
	/*
   vector<int> filtrerconvexe,filtrerconcave;
   vector<int> indic_p_cnvx(convexesize);
   vector<int> indic_p_cncv(concavesize);
   int size_vecindi=0,size_vecindi1=0;
   int convexepouce=-1,concavepouce= -1;
   //seuil distance pr éliminer points convexes/concaves
  double seuilconvexe=25,seuilconcave=7;
  filtrerconvexe=filtrage_distance_entre_points(lconvexe,convexesize,contourHand,&size_vecindi1,seuilconvexe,&indic_p_cnvx, boundRect1.br());
//  filtrerconcave=filtrage_distance_entre_points(lconcave,concavesize,contourHand,&size_vecindi,seuilconcave,&indic_p_cncv,  boundRect1.br());	 

   // trouver pouce s'il existe sinn -1
//  concavepouce= findPouce(indic_p_cncv, lconcave, filtrerconcave,concavesize,size_vecindi);

  //direction: pouce à gauche ou à droite
  directionpouce=concavepouce;
  
  //m.a.j des listes sans le point du pouce
 // vector<int> listwithoutconcavepouce(0);
  vector<int> listwithoutconvexepouce(0);
  
  if(concavepouce != -1 ) //enlever le pouce de la liste s'il existe
   { //dessine point concave du pouce
	 //  cv::circle(dst2, contourHand[concavepouce], 4, cv::Scalar(0, 0, 255), 2);    
  
  filtrerconcave =liste_sans_point_donne(filtrerconcave,&size_vecindi, concavepouce);

  }
 /* else //pas de pouce détecté
	{
		//dessine les points de fortes courbures concaves et convexes avc des couleurs différentes
    dessiner_points_concave_convexe_courbure(RGB, filtrerconcave, filtrerconvexe, contourHand , size_vecindi,size_vecindi1);
    cv::imshow("Poins finals",RGB);
	cv::waitKey(0);
    
   } */
/*	
    if( filtrerconcave.size()> 0 && concavepouce != -1)
	{
	   //selon le point concave du pouce,trouver direction de la main
	int direction= direction_main(concavepouce, filtrerconcave[0],contourHand);
	
	  //trouver point convexe du pouce
	
	convexepouce=find_convexe_pouce( concavepouce, filtrerconvexe,size_vecindi1, direction, contourHand);
	//cv::circle(dst2, contourHand[convexepouce], 4, cv::Scalar(168, 45, 255), 2);  

     //enlever point convexe du pouce de la liste des points convexes
    listwithoutconvexepouce =liste_sans_point_donne(filtrerconvexe,&size_vecindi1, convexepouce);
	/*dessiner_points_concave_convexe_courbure(RGB, filtrerconcave, listwithoutconvexepouce, contourHand , filtrerconcave.size(),listwithoutconvexepouce.size());
	
	cv::imshow("Points finals avec pouce",RGB);
	cv::waitKey(0);*/

	//}

	/*dessiner_points_concave_convexe_courbure(RGB, filtrerconcave, filtrerconvexe, contourHand , size_vecindi,size_vecindi1,concavepouce,convexepouce);
    cv::imshow("Poins finals",RGB);
	cv::waitKey(0);



	//trouver l'équation de la courbe de jonction
	courbe_jonction( filtrerconcave, contourHand,rgbCourbe,nb_alea,boundRect1 );*/

	 /************* Lancer le Game selon résultat de la reconnaissance *****************************/    

			 //start_puzzle();
		/*	 int indicee= size_vecindi1 - 1;
			 if(indicee < 9 )
			 {

			 move_img(vect_puzzle, indicee);
			 label1->Text="nombre de points convexes: "+indicee;

			 }
			 else 
MessageBox::Show("Geste non reconnu,veuillez réessayer !","Geste non valide",MessageBoxButtons::OK,MessageBoxIcon::Stop);

			 
			 
			 cvDestroyAllWindows(); 
	}

	else // vecteur vide , contour de la main non valide, ne rien faire
  
MessageBox::Show("Contour obtenu non valide,veuillez relancer l'exécution !","Contour non valide",MessageBoxButtons::OK,MessageBoxIcon::Stop);

cv::waitKey(100);	
} // fin while

int res=0;
			 if((res=gagner_puzzle(vect_puzzle)) == 1)
				  label1->Text="YOU WIN ! Partie terminée";
			 else  label1->Text="YOU LOOSE ,TRY NEXT TIME !";

cvDestroyAllWindows(); 	
	  return 0;

}*/
		

/*******************************/
cv::Point centre_paume(cv::Point TL,int taille)
{

cv::Point centre;
centre.x=TL.x+taille/2;
centre.y=TL.y+taille/2;

return centre;
}

/*********************************************************/
int reconnaissance(int nb_convexe,bool pouce)
{
	label3->Text="convexe	"+nb_convexe+"pouce "+pouce;
	label3->Refresh();

	if(nb_convexe==0)
	{
		if(pouce==true)
        return 5;
		else return -1; 
	}
	if(nb_convexe==1)
	{
		if(pouce==true) return 6;
		else return 0; 
	}
	if(nb_convexe==2)
	{	if(pouce==true) return 7;
		else return 1; 
	}
		if(nb_convexe==3)
       {if(pouce==true) return 8;
		else return 2; 
		}
	if(nb_convexe==4)
	{
		if(pouce==true) return 4;
		else return 3; 
	}
		return -1;
	}


/******************************/
int MainBoucleSimul(string rgb,string maskstart,array<Guide>^ vect_puzzle)
{	
	   /************ MAIN QUI FAIT APPEL A LA KINECT **********************/

	srand(time(NULL));
	Globalpouce=false;
    //	System::String^ managedString = nb_alea.ToString();
    //  msclr::interop::marshal_context context;
   //string numImg = context.marshal_as<string>(managedString);
	 

	 
	 /**** Prise de l'image du background ****/

	 cv::Mat bg,imggest,RGBgest,RGB,imggest1,RGB1;
	 cv::Mat depth;
     cv::VideoCapture cap;
     cap.open(CV_CAP_OPENNI);
     acquisitionBackground(&depth,cap);
	 //cv::imshow("depth",depth);
	 //cv::waitKey(0);
	 label2->Text="	Allez-y faite un geste";
	 label2->Refresh();
	 int cpt=0;
    
	while( cpt < 1000)
	 {
	  //    label1->Text="";
    //      label1->Refresh();
	int	nb_alea = (rand() % 10000) + 1; //génère un nombre entre 1 et 100
	
	/******* Acquisition du geste *********************/
   	 acquisitionGest(&imggest1,&RGB1,depth,cap);
	 cv::Mat rgbCourbe=RGB1.clone();
	 string nom_rgb;
	 nom_rgb=concatene_nom_img(nb_alea,"RGB");
	 cv::imwrite(nom_rgb,RGB1);
	 String^ nom= gcnew String(nom_rgb.c_str());
//	 LoadNewPict1(nom,boxRGB);
	 string nom_mask;
   	 nom_mask=concatene_nom_img(nb_alea,"mask");
	 cv::imwrite(nom_mask,imggest1);


	 String^ nomm="original.jpg";
	 string str=nom_rgb;
	


		cv::Mat dst=segmentationDirecte(imggest1,RGB1);
		string nom_seg;
		nom_seg=concatene_nom_img(nb_alea,"segmentsimul");
		cv::imwrite(nom_seg,dst);
	

	    /*** eliminer visage ****/

	   
        face_eliminer(RGB1,&dst,nb_alea);
      

	/**********************************************************/

	   cv::Rect boundRect;
	   cv::Mat  dst2 =dst.clone();
	   //retrouver contour de la main seulement pr faire la paume et puis suppression
	   std::vector<cv::Point> contour;
	   detecterContours(RGB1,dst2 ,&boundRect,&contour);

	  /********************* Suppression du bras *************************/

	  suppression_bras(&dst2, boundRect );
	  string nom_delet;
	  nom_delet=concatene_nom_img(nb_alea,"deletarmsimul");
	  cv::imwrite(nom_delet,dst2);
   
      /************** détection du contour aprés suppression *********************/
	cv::Rect boundRect1;
	    std::vector<cv::Point> contourHand;
	    detecterContours_modifier(RGB1,dst2 ,&boundRect1,&contourHand,nb_alea);
	    if(contourHand.size() > 0 ) {


		 /******** Trouver et Dessiner paume de la main ********/

	  int max_ip=0,max_jp=0,taillePaumeMain,br_paume_y;
      int indic=construct_sous_Mat( boundRect1,dst2,&max_ip,&max_jp);
      taillePaumeMain=indic;
	        cv::Point TLpaume;
	 TLpaume=draw_paume_main(RGB1,dst2,max_ip,max_jp,indic,boundRect1,&br_paume_y,nb_alea);

	  /********************** Trouver centre de gravité	**********************/
	  cv::Point2f mc;
	  mc=centre_paume(TLpaume,taillePaumeMain);


		cv::circle(RGB1,mc,4,cv::Scalar(0,0,0),2);
		int size_convexe=0;

		trouver_points_courbures(&RGB1,contourHand,nb_alea,mc,boundRect1,&size_convexe);

		imwrite("final5.jpg",RGB1);
        
	 /************* Lancer le Game selon résultat de la reconnaissance *****************************/    

			 //start_puzzle();
			  int indicee=0;
			 if( (indicee=reconnaissance(size_convexe,Globalpouce)) != -1)
			 {
              
			 move_img(vect_puzzle, indicee);
			 label1->Text="nombre de points convexes: "+indicee;

			 }
			 else 
//MessageBox::Show("Geste non reconnu,veuillez réessayer !","Geste non valide",MessageBoxButtons::OK,MessageBoxIcon::Stop);
  {
	label1->Text="Geste non reconnu!,veuillez réessayer";
     label1->Refresh();	
	
			 }			 
		 
	}

	else // vecteur vide , contour de la main non valide, ne rien faire
	{ 
//MessageBox::Show("Contour obtenu non valide!","Contour non valide",MessageBoxButtons::OK,MessageBoxIcon::Stop);
label1->Text="Contour obtenu non valide!";
label1->Refresh();

		}
		cpt++;
	
} // fin while

int res=0;
			 if((res=gagner_puzzle(vect_puzzle)) == 1)
				  {label1->Text="Résultat de la partie: YOU WIN ! Partie terminée";
			 label1->Refresh();}
			 else { label1->Text="Résultat de la partie: YOU LOOSE ,TRY NEXT TIME !";
			 label1->Refresh();
			 }
cvDestroyAllWindows();
	  return 0;

}
/*****************************************************************************/
	public: System::Windows::Forms::PictureBox^  box1;
	protected: 
	private: System::Windows::Forms::PictureBox^  box2;
	private: System::Windows::Forms::PictureBox^  box3;
	private: System::Windows::Forms::PictureBox^  box4;
	private: System::Windows::Forms::PictureBox^  box5;
	private: System::Windows::Forms::PictureBox^  box6;
	private: System::Windows::Forms::PictureBox^  box7;
	private: System::Windows::Forms::PictureBox^  box8;
	private: System::Windows::Forms::PictureBox^  box9;

	private: System::Windows::Forms::Button^  stopapp;
	private: System::Windows::Forms::Button^  guide;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;










	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(simul::typeid));
			this->box1 = (gcnew System::Windows::Forms::PictureBox());
			this->box2 = (gcnew System::Windows::Forms::PictureBox());
			this->box3 = (gcnew System::Windows::Forms::PictureBox());
			this->box4 = (gcnew System::Windows::Forms::PictureBox());
			this->box5 = (gcnew System::Windows::Forms::PictureBox());
			this->box6 = (gcnew System::Windows::Forms::PictureBox());
			this->box7 = (gcnew System::Windows::Forms::PictureBox());
			this->box8 = (gcnew System::Windows::Forms::PictureBox());
			this->box9 = (gcnew System::Windows::Forms::PictureBox());
			this->stopapp = (gcnew System::Windows::Forms::Button());
			this->guide = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// box1
			// 
			this->box1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box1.Image")));
			this->box1->Location = System::Drawing::Point(81, 60);
			this->box1->Name = L"box1";
			this->box1->Size = System::Drawing::Size(125, 127);
			this->box1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box1->TabIndex = 0;
			this->box1->TabStop = false;
			this->box1->Click += gcnew System::EventHandler(this, &simul::box1_Click);
			// 
			// box2
			// 
			this->box2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box2.Image")));
			this->box2->Location = System::Drawing::Point(212, 60);
			this->box2->Name = L"box2";
			this->box2->Size = System::Drawing::Size(123, 127);
			this->box2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box2->TabIndex = 1;
			this->box2->TabStop = false;
			this->box2->Click += gcnew System::EventHandler(this, &simul::box2_Click);
			// 
			// box3
			// 
			this->box3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box3.Image")));
			this->box3->Location = System::Drawing::Point(341, 60);
			this->box3->Name = L"box3";
			this->box3->Size = System::Drawing::Size(136, 127);
			this->box3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box3->TabIndex = 2;
			this->box3->TabStop = false;
			// 
			// box4
			// 
			this->box4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box4.Image")));
			this->box4->Location = System::Drawing::Point(81, 193);
			this->box4->Name = L"box4";
			this->box4->Size = System::Drawing::Size(125, 133);
			this->box4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box4->TabIndex = 3;
			this->box4->TabStop = false;
			// 
			// box5
			// 
			this->box5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box5.Image")));
			this->box5->Location = System::Drawing::Point(212, 193);
			this->box5->Name = L"box5";
			this->box5->Size = System::Drawing::Size(123, 133);
			this->box5->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box5->TabIndex = 4;
			this->box5->TabStop = false;
			// 
			// box6
			// 
			this->box6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box6.Image")));
			this->box6->Location = System::Drawing::Point(341, 193);
			this->box6->Name = L"box6";
			this->box6->Size = System::Drawing::Size(136, 133);
			this->box6->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box6->TabIndex = 5;
			this->box6->TabStop = false;
			// 
			// box7
			// 
			this->box7->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box7.Image")));
			this->box7->Location = System::Drawing::Point(81, 332);
			this->box7->Name = L"box7";
			this->box7->Size = System::Drawing::Size(125, 135);
			this->box7->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box7->TabIndex = 6;
			this->box7->TabStop = false;
			// 
			// box8
			// 
			this->box8->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"box8.Image")));
			this->box8->Location = System::Drawing::Point(212, 332);
			this->box8->Name = L"box8";
			this->box8->Size = System::Drawing::Size(123, 135);
			this->box8->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box8->TabIndex = 7;
			this->box8->TabStop = false;
			// 
			// box9
			// 
			this->box9->Location = System::Drawing::Point(341, 332);
			this->box9->Name = L"box9";
			this->box9->Size = System::Drawing::Size(136, 135);
			this->box9->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->box9->TabIndex = 8;
			this->box9->TabStop = false;
			// 
			// stopapp
			// 
			this->stopapp->Font = (gcnew System::Drawing::Font(L"Joan", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->stopapp->Location = System::Drawing::Point(23, 521);
			this->stopapp->Name = L"stopapp";
			this->stopapp->Size = System::Drawing::Size(105, 35);
			this->stopapp->TabIndex = 10;
			this->stopapp->Text = L"Quitter";
			this->stopapp->UseVisualStyleBackColor = true;
			this->stopapp->Click += gcnew System::EventHandler(this, &simul::stopapp_Click);
			// 
			// guide
			// 
			this->guide->Font = (gcnew System::Drawing::Font(L"Joan", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->guide->Location = System::Drawing::Point(265, 476);
			this->guide->Name = L"guide";
			this->guide->Size = System::Drawing::Size(119, 42);
			this->guide->TabIndex = 11;
			this->guide->Text = L"Guide du Puzzle";
			this->guide->UseVisualStyleBackColor = true;
			this->guide->Click += gcnew System::EventHandler(this, &simul::guide_Click);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Joan", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->button1->Location = System::Drawing::Point(144, 476);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(115, 42);
			this->button1->TabIndex = 12;
			this->button1->Text = L"Lancer Jeu";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &simul::button1_Click);
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->label1->Font = (gcnew System::Drawing::Font(L"Joan", 14.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(217, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(406, 32);
			this->label1->TabIndex = 13;
			this->label1->Text = L"Résultat de la partie:";
			this->label1->Click += gcnew System::EventHandler(this, &simul::label1_Click);
			// 
			// label2
			// 
			this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 11.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(574, 458);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(157, 23);
			this->label2->TabIndex = 14;
			this->label2->Text = L"Indicateur de début";
			this->label2->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->Location = System::Drawing::Point(510, 74);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(274, 356);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 15;
			this->pictureBox1->TabStop = false;
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(585, 491);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(134, 33);
			this->label3->TabIndex = 16;
			// 
			// simul
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(884, 584);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->guide);
			this->Controls->Add(this->stopapp);
			this->Controls->Add(this->box9);
			this->Controls->Add(this->box8);
			this->Controls->Add(this->box7);
			this->Controls->Add(this->box6);
			this->Controls->Add(this->box5);
			this->Controls->Add(this->box4);
			this->Controls->Add(this->box3);
			this->Controls->Add(this->box2);
			this->Controls->Add(this->box1);
			this->Cursor = System::Windows::Forms::Cursors::Hand;
			this->DoubleBuffered = true;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"simul";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Puzzle";
			this->Load += gcnew System::EventHandler(this, &simul::simul_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &simul::simul_KeyDown_1);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->box9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void stopapp_Click(System::Object^  sender, System::EventArgs^  e) {
				// this->Hide();
	if (MessageBox::Show("Voulez vous quitter le puzzle ?","Puzzle",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{//Application::ExitThread();
			  this->Hide();
	if (MessageBox::Show("Voulez vous quitter l'application?","Application",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{Application::ExitThread();
	}
	//else { e->Cancel= true;}
	
	}
		 }	
		/***********************************************************/
/*void simul_KeyDown( System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e){ 

if(e->KeyCode == Keys::E )
	stop=true;

} */

			 /*****************************************************/
 void initialise_tableau( array< Guide >^ vect_puzzle)
  {
  //array< simul::simul::guide >^ vect_puzzle=gcnew array< simul::simul::guide >(9);
	vect_puzzle[0].etat=1;    
	vect_puzzle[0].num=6;
    vect_puzzle[0].nom_img="ImagePuzzle\\img6.jpg";
	vect_puzzle[0].box=box1;

    vect_puzzle[1].etat=1;
    vect_puzzle[1].num=8;
	vect_puzzle[1].nom_img="ImagePuzzle\\img8.jpg";
	vect_puzzle[1].box=box2;

    vect_puzzle[2].etat=1;
    vect_puzzle[2].num=4;
	vect_puzzle[2].nom_img="ImagePuzzle\\img4.jpg";
	vect_puzzle[2].box=box3;

    vect_puzzle[3].etat=1;
    vect_puzzle[3].num=2;
	vect_puzzle[3].nom_img="ImagePuzzle\\img2.jpg";
	vect_puzzle[3].box=box4;

    vect_puzzle[4].etat=1;
    vect_puzzle[4].num=7;
    vect_puzzle[4].nom_img="ImagePuzzle\\img7.jpg";
	vect_puzzle[4].box=box5;

	vect_puzzle[5].etat=1;
    vect_puzzle[5].num=1;
    vect_puzzle[5].nom_img="ImagePuzzle\\img1.jpg"; 
	vect_puzzle[5].box=box6;

    vect_puzzle[6].etat=1;
    vect_puzzle[6].num=3;
    vect_puzzle[6].nom_img="ImagePuzzle\\img3.jpg";
	vect_puzzle[6].box=box7;

	vect_puzzle[7].etat=1;
    vect_puzzle[7].num=5;
	vect_puzzle[7].nom_img="ImagePuzzle\\img5.jpg";
	vect_puzzle[7].box=box8;

	vect_puzzle[8].etat=0;
    vect_puzzle[8].num=9;
    vect_puzzle[8].nom_img=nullptr;
	vect_puzzle[8].box=box9;
   //return vect_puzzle;
  }



       /***********************************************************/
   void start_puzzle()
   {
   			  LoadNewPict1("ImagePuzzle\\img6.jpg",box1);
			  LoadNewPict1("ImagePuzzle\\img8.jpg",box2);
			  LoadNewPict1("ImagePuzzle\\img4.jpg",box3);
			  LoadNewPict1("ImagePuzzle\\img2.jpg",box4);
			  LoadNewPict1("ImagePuzzle\\img7.jpg",box5);
			  LoadNewPict1("ImagePuzzle\\img1.jpg",box6);
			  LoadNewPict1("ImagePuzzle\\img3.jpg",box7);
			  LoadNewPict1("ImagePuzzle\\img5.jpg",box8);
              Clear1(box9);
   }
   
      /************************************************************/
   public:	void LoadNewPict1(String^ s,System::Windows::Forms::PictureBox^  box)
   {
      // You should replace the bold image 
      // in the sample below with an icon of your own choosing.
        
		// ImgOperation->Image = Image::FromFile(s); 
	 
	try
      {
		  if(s != nullptr)
         {Bitmap^ image1 = dynamic_cast<Bitmap^>(Image::FromFile( s, true ));
		 box->Image= image1;
         box->SizeMode = PictureBoxSizeMode::StretchImage;
		 box->Refresh();
		  }
		 //delete formGraphics;
      }
      catch ( System::IO::FileNotFoundException^ ) 
      {
         MessageBox::Show( "There was an error opening the bitmap."
         "Please check and try again." );
      }


	  
   }

     /*********************************************************************/
   public: void Clear1(System::Windows::Forms::PictureBox^  box)
	{
   if (box->Image != nullptr)
	{
	box->Image->~Image();
	box->Image = nullptr;
	box->Refresh();
	}	
	}
		   /************************************************************/

int gagner_puzzle( array< Guide>^ vect_puzzle) //retourne 1 si trier et 0 sinon
{   int i=0;
	while(i < 9 && (vect_puzzle[i].num < vect_puzzle[i+1].num))
		i++;
	if( i==9 ) return 1;
	else return 0;
}

           /*******************************************/
int find_free_case(array <Guide>^ vect_puzzle)
{int i=0;
while( vect_puzzle[i].etat == 1)
	i++;

return i; //indice de la case vide 
}
       /*************************************************/
void move_img(array <Guide>^ vect_puzzle,int indice)
{
if(vect_puzzle[indice].etat == 1)
{
 int free=find_free_case(vect_puzzle);
 vect_puzzle[free].etat= 1;
 vect_puzzle[free].num= vect_puzzle[indice].num;
 vect_puzzle[free].nom_img=vect_puzzle[indice].nom_img;
 LoadNewPict1( vect_puzzle[free].nom_img, vect_puzzle[free].box);

  vect_puzzle[indice].etat= 0;
  vect_puzzle[indice].num= 9;
 // vect_puzzle[free].nom_img=nullptr;
  Clear1(vect_puzzle[indice].box);

}
else 
//MessageBox::Show("Case déja vide !","Erreur",MessageBoxButtons::OK,MessageBoxIcon::Error);
label3->Text="Case déja vide";
label3->Refresh();
}

           /***************************************************/
  public : int i;
		void test()
			{ 
			 LoadNewPict1("original.jpg",box1);
			}
private: System::Void simul_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void box1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void box2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void startapp_Click(System::Object^  sender, System::EventArgs^  e) {
           array< Guide>^ vect_puzzle = gcnew array< Guide >(9);
			 initialise_tableau(vect_puzzle);	
			  start_puzzle();
		
		 }
private: System::Void guide_Click(System::Object^  sender, System::EventArgs^  e) {
              	geste^ obj= gcnew geste();
				 obj->ShowDialog();
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {

			 array< Guide>^ vect_puzzle = gcnew array< Guide >(9);
			 initialise_tableau(vect_puzzle);	
			  start_puzzle();

			int res=MainBoucleSimul("RGBpp.jpg","segkinectp.jpg", vect_puzzle);

		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 array< Guide>^ vect_puzzle = gcnew array< Guide >(9);
			 initialise_tableau(vect_puzzle);
			 if(vect_puzzle[0].num==6) label1->Text="good";
			 start_puzzle();
			 
			 move_img( vect_puzzle,0);
		 }
private: System::Void Arreter_Click(System::Object^  sender, System::EventArgs^  e) {
			 stop=true;
		 }
private: System::Void simul_KeyDown_1(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {

			if (e->Alt) 
	        stop=0;
		 }
};
}
