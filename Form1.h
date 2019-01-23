#pragma once
#include "Aide.h"
/*************************/
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
#include <stdlib.h>
#include <time.h>
#include <opencv/cv.h>
#include <vector>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>


/************************/

namespace pfe_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;
	
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		//Global variables
		String^ img;
		int directionpouce;
		int nb_alea; 



	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::GroupBox^  Menu;
	private: System::Windows::Forms::Label^  ltest;
	private: System::Windows::Forms::Button^  button12;
	private: System::Windows::Forms::Button^  button11;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label5;





	public: 


	public: 
	int StartMain;
	 

		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

   
		/**************************************************/
/** la structure de HSV**/
 value struct HSV{
double H;
double S;
double V;
};
/** la structure de ycbcr**/
value struct ycbcr{
double yVal;
double cbVal;
double crVal;
};

/** structure point forte fotre courbure **/

value struct Point_rest
{int indic_point;
float angle;
};
 

//-------------------------------------------------------------------------------------------------------------

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

		/***************************************************************/

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
//----------------------------------------------------------------------------------------------------------------
/** la fonction qui verifie si un pixel est un pixel de peau ou non en utilisant les valeurs hsv**/
int verif_pixel_HSV(HSV elm)
{
   // cout << "dans verif_pix" << endl;
if( elm.H >0 && elm.H <50)
{
  if(elm.S <0.68 && elm.S >0.23){
         return 0;}
}
//cout << "avant le return 1" << endl;
return 1;
}
/*******************/

int verif_pixel_HSV(cv::Vec3b pixel)
{
				// 0<H<0.25  -   0.15<S<0.9    -    0.2<V<0.95
if( (pixel[0]>5) && (pixel[0] < 17) && (pixel[1]>38) && (pixel[1]<250) && (pixel[2]>51) && (pixel[2]<242) )
				return 0;
else return 1;
		}

//----------------------------------------------------------------------------------------------------------------
/** la fonction qui verifie si un pixel est un pixel de peau ou non en utilisant les valeurs YCbCr**/
int verif_pixel_YCbCr(ycbcr elm)
{
if( elm.yVal >14 && elm.yVal <255)
{

  if(elm.cbVal <134 && elm.cbVal >84){

    if(elm.crVal <184 && elm.crVal >134) //cout << "dans le 3eme if" << endl;
    {

    return 0;}}}

return 1;
}
//-------------------------------------------------------------------
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
/**elle crie un vecteur qui va contunir par la suite les valeur Ycbcr**/
 void RGBtoYCbCr(array<pfe_gui::Form1::ycbcr >^ dst ,cv::Mat src)
{
    cv::Vec3b p;

//	std::vector<pfe_gui::Form1::ycbcr> dst(src.rows*src.cols);
	//ycbcr^ myDataTable = gcnew ycbcr;
	//si ycbcr était une classe : array< ycbcr^ >^ dst = gcnew array< ycbcr^ >(ARRAY_SIZE);
	// array< pfe_gui::Form1::ycbcr >^ dst = gcnew array< pfe_gui::Form1::ycbcr >(src.rows*src.cols); //dst est de type cli::array<Type> ^
   // memset(dst,0,100*sizeof(ycbcr));

    int x, y,i=0;
	double yy,cb,cr;
   // assert (src->depth == IPL_DEPTH_8U);
	for (y = 0; y < src.rows; y++)
        {
			for (x = 0; x < src.cols; x++)
                {
                 // p = cvGet2D (src, y, x);
					p=src.at<cv::Vec3b>(y,x);
					dst[i] = pfe_gui::Form1::ycbcr();
                    calculYCbCr(p,&yy,&cb,&cr);
					dst[i].yVal=yy;
					dst[i].cbVal=cb;
					dst[i].crVal=cr;
                 i++;
                 }
         }

//return dst;

}

//----------------------------------------------------------------------------------------------------------------
/**elle crée un vecteur qui va contenir par la suite les valeur HSV**/
 void RGBtoHSV(array< pfe_gui::Form1::HSV >^ dst,cv::Mat src)
{ double H,S,V;
    cv::Vec3b p;
//	HSV *dst=new HSV[src.rows*src.cols];
    	//array< pfe_gui::Form1::HSV >^ dst = gcnew array< pfe_gui::Form1::HSV >(src.rows*src.cols);  
	// memset(dst,0,100*sizeof(HSV));
    int x, y,i=0;
    //assert (src->depth == IPL_DEPTH_8U);
	for (y = 0; y < src.rows; ++y)
        {
			for (x = 0; x < src.cols; ++x)
                {
                  //p = cvGet2D (src, y, x);
				  p=src.at<cv::Vec3b>(y,x);
                 // dst[i] = pfe_gui::Form1::HSV();
				  calcul_Hsv(p,&H,&S,&V);
				  dst[i].H=H;
				  dst[i].S=S;
				  dst[i].V=V;

                  i++;
                 }
         }
//       return dst;

}
//-------------------------------------------------------------------------------------------------------------

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
    ltest->Text="Couldn't load face_cascade";
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
      cv::rectangle( dessiner, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0,255,0), 2, 8, 0 );//dessine la bounding box autour de la main
  }


  }

  
////////*************************************************************************//////////////////////////////

//---------------------------------------------------------------------------------------------------------------
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
/*******************************/
cv::Point centre_paume(cv::Point TL,int taille,int numImg,cv::Mat *img)
{

cv::Point centre;
centre.x=TL.x+taille/2;
centre.y=TL.y+taille/2;
 cv::circle( *img ,centre, 4, cv::Scalar(255, 69,37), -1, 8, 0 );
	string nom_centre;
	nom_centre=concatene_nom_img(numImg,"centrepaume");
    cv::imwrite(nom_centre,*img);
return centre;
}
/******************************/
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
/*cv::Mat segmentationVS(cv::Mat imgres,array< pfe_gui::Form1::HSV >^ tab1,array< pfe_gui::Form1::ycbcr >^ tab2)
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
cv::Mat segmentationDirecte(cv::Mat imgres,cv::Mat imgRGB,int nb_alea)
{

	 CvScalar p;

 IplImage imgresD = imgres;
 IplImage* imgD = &imgresD; //résultat dans imgD
	cv::Mat dst(imgres.rows,imgres.cols,CV_8UC3,cv::Scalar(0,0,0));
	cv::Mat imgycbcr,result,imghsv;
	cvtColor(imgRGB,imgycbcr, CV_BGR2YCrCb);
	//cv::imwrite("YCbcr.jpg",imgycbcr);
	  string nom_ycbcr;
	 nom_ycbcr=concatene_nom_img(nb_alea,"YCbcr");
	 cv::imwrite(nom_ycbcr,imgycbcr);

	cvtColor(imgRGB,imghsv, CV_BGR2HSV);
	//cv::imwrite("HSV.jpg",imghsv);
	  string nom_hsv;
	 nom_hsv=concatene_nom_img(nb_alea,"HSV");
	 cv::imwrite(nom_hsv,imghsv);

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

	       string nom_segmavantOp;
	 nom_segmavantOp=concatene_nom_img(nb_alea,"peau");
    cv::imwrite( nom_segmavantOp,dst);

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
	
	//retourne 0 si concave et 1 si convexe
	int if_concave_convexe(cv::Mat img_seg,cv::Point p_plus,cv::Point p_moins,cv::Point p)
	{
	int x_mid=0,y_mid=0;
	x_mid=(p_plus.x + p_moins.x)/2;
	y_mid=(p_plus.y + p_moins.y)/2;
	//cv::circle( img_seg,p_plus,  4, cv::Scalar(255,255,0), 2 );
	//cv::circle( img_seg,p_moins,  4, cv::Scalar(0,128,255), 2 );
	cv::Point p_som=(p_plus + p_moins );
	cv::Point p_mid;
	p_mid.x = p_som.x /2;
	p_mid.y = p_som.y /2;
	cv::Vec3b pixel= img_seg.at<cv::Vec3b>(p_mid);
	//cv::Mat dessin= cv::Mat::zeros(img_seg.size(),CV_8UC3);
	//cv::circle( img_seg,p_mid,  4, cv::Scalar(87,122,185), 2 );
	//cv::circle( img_seg,p,  4, cv::Scalar(255,0,128), 2 );
	
	if(img_seg.at<cv::Vec3b>(p_mid)[0] == 0)
		{ 
		return 0; 
	    }
	else
	  {if(img_seg.at<cv::Vec3b>(p_mid)[0] == 255)
		{ 	return 1;}
	  }
   }
	/*    int white=0,black=0;
		std::vector<cv::Point> pdl;
		cv::Point p0,p1,p2,pt;
		p0=p_plus;
		p1=p_moins;
		double pixel;
		cv::LineIterator it(img_seg,p0,p1,8);
		for(int i=0;i< it.count;i++,it++)
		{
		pt=it.pos();
		pdl.push_back(pt);
		pixel=(double) img_seg.at<unsigned char>(pt.y,pt.x);
		if(pixel == 255)
			white++;
		else{
			if(pixel ==0) black++;}
		}
		
		if(white>black) //convexe
			return 1;
		else //concave 
			return 0;
	}*/

	/*****************************************************/
	
array< pfe_gui::Form1::sharp >^ first_pass(std::vector<cv::Point> ContourMain,int d_min,int d_max,float cos_min)
{
    int d_min_sq;
	d_min_sq= d_min*d_min;
	double a_sq;
    int d_max_sq = d_max*d_max;
	double cos_max ;
	bool seen_one_upper,seen_one_lower;
	double b_sq,c_sq,top;
    double bottom ,cos;
	double temp;
//	vector<sharp> sharpness(ContourMain.size());

		array< pfe_gui::Form1::sharp >^ sharpness = gcnew array< pfe_gui::Form1::sharp >(ContourMain.size());

for (int i=0;i<ContourMain.size();i++) {
/* Each element of the "sharpness" array contains the sharpness as the third value, and the offsets before and after the point where this sharpness maximum was obtained as the
first and second value. The fourth value is a flag which indicates whether or not the sharpness has been calculated for this point. */
	(sharpness)[i].indplus=-1;
	(sharpness)[i].indmoins=-1;
	(sharpness)[i].angle=1000;
	(sharpness)[i].empty=0;
	
 cos_max = -1;
 seen_one_upper=false;

for (int plus = i + 1; plus < ContourMain.size(); plus++) 
{
temp = calculer_distance(ContourMain[i],ContourMain[plus]); 
a_sq=temp*temp;
if (seen_one_upper && a_sq > d_max_sq) 
{
	if ((sharpness)[i].empty == 0)
		(sharpness)[i].empty = 1;
break;
}

if (a_sq < d_min_sq)
continue;
seen_one_upper = true;
seen_one_lower=false;

for (int minus = i - 1; minus >= 0; minus--) 
{
temp = calculer_distance(ContourMain[i],ContourMain[minus] );
b_sq=temp*temp;

if (seen_one_lower && b_sq > d_max_sq) {
	if ((sharpness)[i].empty == 0)
		(sharpness)[i].empty = 2;
break;
}
if (b_sq < d_min_sq)
continue;
seen_one_lower = true;
temp =calculer_distance(ContourMain[plus],ContourMain[minus] ); 
c_sq=temp*temp;
top = a_sq + b_sq - c_sq;
bottom = 2.0* sqrt(a_sq*b_sq);
cos = -2;
if (bottom != 0.0) 
{
cos = top/bottom;
}

if (cos < cos_min) {
	if ((sharpness)[i].empty == 0) 
	{
		(sharpness)[i].angle = cos;
		(sharpness)[i].empty=3;
	}
break;

}
if (cos_max < cos) 
{
cos_max = cos;
(sharpness)[i].indmoins=minus;
(sharpness)[i].indplus=plus ;
(sharpness)[i].angle=cos;
(sharpness)[i].empty=4;
} 
else 
{
	if ((sharpness)[i].empty == 0) {
		(sharpness)[i].angle = cos;
		(sharpness)[i].empty = 3;
								}}}}}

return sharpness;
}


/* Find maxima of the sharpnesses. */
void second_pass(std::vector<int> *corners,array< pfe_gui::Form1::sharp >^ sharpness,std::vector<cv::Point> contMain,int cos_min,int d_max,int *length)
{
	
int n_found = -1;
int last_found = -1;
int i;
bool found;
float dist_sq,temp;
int d_max_sq=d_max*d_max;
for (i=0;i<contMain.size();i++)
{
found = false;
if (sharpness[i].angle > cos_min && sharpness[i].empty== 4) {
if (last_found >= 0) {
temp = calculer_distance(contMain[last_found],contMain[i]);
dist_sq=temp*temp;
if (dist_sq > d_max_sq)
{
n_found++;
found = true;
} else if (sharpness[i].angle > sharpness[last_found].angle)
{
found = true;
}
} else 
{
n_found++;
found = true;
}
}
if (found) {
last_found = i;
(*corners)[n_found] = i;
// alert ("Found corner "+n_found+" at "+i);
}
}
*length=n_found;
}

/*******************************************************************/
 void find_corners(std::vector<int> *resultat,int d_max,int d_min,float angle_max,std::vector<cv::Point> ContourMain,int *taille)
{
double angle;
double cos_min;
if (angle_max) {
angle = (  3.14 * angle_max)/180.0;
cos_min =  cos (angle);
}
int length;
std::vector<int> resultat2(ContourMain.size());

//std::vector<sharp> sharpness(ContourMain.size());
array< pfe_gui::Form1::sharp >^ sharpness ;//= gcnew array< pfe_gui::Form1::sharp >(ContourMain.size());
sharpness=first_pass(ContourMain,d_min,d_max,cos_min);
second_pass(&resultat2,sharpness,ContourMain,cos_min,d_max,&length);
*resultat=resultat2;
*taille=length;
}


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

	/*string nom_pointfin;
	nom_pointfin=concatene_nom_img(nb_alea,"pointsfinals");
	cv::imwrite(nom_pointfin,img);*/

	}
	/************************************************************/
	
	double calculer_angle_alpha(double a,double b,double c)
	{
	double alpha=0;
	alpha= acos((a*a + b*b - c*c)/(2*a*b));
	return alpha;
	}
	/******************************************************/
	//cherche l'indice d'un point dans le contour, si elle le trouve pas elle retourne -1 sinn retourne point
	int indice_p_dans_contour(vector<cv::Point> contourHand,cv::Point p)
	{
		int i=0,indic=-1;
		bool trouve=false;
		while(i < contourHand.size() && trouve==false)
		{
		if(p.x== contourHand[i].x && p.y== contourHand[i].y )
		{
			indic=i;
			trouve=true;
		}
		i++;
		}
		return indic;
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

	/*****************************************/
	// tri +fusion:  chetverecov /convexitydefects 


	//fonction qui retourne les indices des points de convexitydefects dans la liste contours
	std::vector<int> convert_indic(std::vector<cv::Point> list ,std::vector<cv::Point> contourHand)
	{

	 //dans main : 
		std::vector<int> indice(list.size());
	 int j=0;
	 for(int i=0; i<list.size();i++)
	 {
	 while (contourHand[j] != list[i] )  
	 {
		 j++;
	 }
	  //point trouvé dans contourHand
	 indice[i]=j;
	 j=0;
	 //j++;
	 }
	  
	return indice;
	}
	/***********************************************/
 //fait la fusion trié des deux vecteurs d'indice des points chetverecov et points convexitydefects
std::vector<int> fusion_trie(std::vector<int> listChet ,std::vector<int> listCVD,int realsize,int *taillefusion)
	{
	
	int i=0,j=0,k=0;
	int taille= realsize+ listCVD.size();
	std::vector<int> res(taille);
	
	while(i<realsize && j<listCVD.size() )
	{
	 if(listChet[i] < listCVD[j])
	 {	 res[k]=listChet[i];
		 i++;
		 k++;
	 }
	 else{
		if(listChet[i] > listCVD[j])
		 {	 res[k]=listCVD[j];
			 j++;
			 k++;
		 }
		else{
		res[k]=listCVD[j];
			 j++;
			 k++;
			 i++;
		    }
	 
	 }
	
	}//fin while
	
	while( i < realsize )
	{
	res[k]=listChet[i];
		 i++;
		 k++;
	}

	while( j<listCVD.size() )
	{
		res[k]=listCVD[j];
			 j++;
			 k++;
	
	}
	*taillefusion=k;
	//std::vector<int> res2(*taillefusion);
	std::sort (res.begin(), res.end());
	/*int min=res[0], ind_min=0;
	for(int j=0;j<k;j++)
	{
		for(int o=ind_min;o<k;o++)
		{
		if(res[o]<min)
			min=res[o];
		}
		res2[j]=min;
		ind_min=j + 1;
	}*/
	return res;

	}
 /*******************************************************/

//nous retourne deux listes :points concaves /points convexes , on prends pas=10 pr trouver p- et p+
void fraction_concave_convexe(cv::Mat img,array< pfe_gui::Form1::Point_rest >^ lconcave,array< pfe_gui::Form1::Point_rest >^ lconvexe,
	std::vector<int> listindic, std::vector<cv::Point> contourHand,int taille,int *concavesize,int *convexesize)
{   /* à créer dans le main avant l'appel de la fonction	
  array< pfe_gui::Form1::Point_rest >^ lconcave = gcnew array< pfe_gui::Form1::Point_res >(taille);
	array< pfe_gui::Form1::Point_rest >^ lconvexe = gcnew array< pfe_gui::Form1::Point_res >(taille);
	*/
	int ind=0,j=0,k=0;
	int pas=10;
	double a,b,c;
	double angle;
	cv::Point p,pMoins,pPlus;
	int sizeContour=contourHand.size();
	for(int i=0; i< taille;i++) 
	{
	 ind=listindic[i];
	 p=contourHand[ind];
	 if(ind+pas >= sizeContour)
	 {pPlus=contourHand[(ind+pas) -sizeContour];}
	 else 
		 {pPlus=contourHand[ind+pas];}
	 if(ind-pas <0)
	 {pMoins=contourHand[sizeContour-pas+ind]; }//ici ça beug sometimes car ind -pas est en dehors du contour
	 else 
		{ pMoins=contourHand[ind-pas];}
	 a=calculer_distance(p,pPlus);
	 b=calculer_distance(p,pMoins);
	 c=calculer_distance(pMoins,pPlus);
	 //calculer angle alpha
	 angle=calculer_angle_alpha(a,b,c);
	 double max_angl_concave=2.5;
	 //concave convexe
	 if(if_concave_convexe(img,pPlus,pMoins,p) ==1 && angle < max_angl_concave)
	 { //insérer dans lconvexe
	   lconvexe[j].angle=angle;
	   lconvexe[j].indic_point=ind;
	   j++;
	 }
	 else 
	  if( angle < max_angl_concave )	
	 {                                  //insérer dans lconcave
	   lconcave[k].angle=angle;
	   lconcave[k].indic_point=ind;
	   k++;
 	    }
	}
	*concavesize=k;
	*convexesize=j;
}
/****************************************************/

bool exist_indice_between(int ind1,int ind2,array< pfe_gui::Form1::Point_rest >^ list,int taille)
{
	int i=0;
	bool trouve=false;
	int indice;
	while(i<taille && trouve==false)
	{
		indice=list[i].indic_point;
		if(indice > ind1)
		 {
	     if(indice < ind2) 
		  {
			trouve=true;
		  }
		 }
		i++;
	}
return trouve;
}

/*****************************************************/
void elimination (array< pfe_gui::Form1::Point_rest >^ resultat, array< pfe_gui::Form1::Point_rest >^ list1, array< pfe_gui::Form1::Point_rest >^ list2,
	int taille1,int taille2, int *tailleres)
{
int i=0,j=0,indice=list1[0].indic_point;
float angle=list1[0].angle;

while(i <taille1)
{

 if(  exist_indice_between(list1[i].indic_point,list1[i+1].indic_point,list2, taille2) ==false)
 {
  if(list1[i+1].angle <= angle)
  {
   angle=list1[i+1].angle;
   indice=list1[i+1].indic_point;
  }
  i++;
 }
 else
 {
  resultat[j].indic_point=indice; 
  resultat[j].angle=angle;
  j++;
  i= i+1;
  angle=list1[i].angle;
  indice=list1[i].indic_point;
 }

}

resultat[j].indic_point=indice; 
resultat[j].angle=angle;
   *tailleres=j+1;

}
/***********************************************/
void final_list_point(array< pfe_gui::Form1::Point_rest >^ resconvexe,array< pfe_gui::Form1::Point_rest >^ resconcave, 
array< pfe_gui::Form1::Point_rest >^ listconvexe, array< pfe_gui::Form1::Point_rest >^ listconcave,int convexesize,int concavesize, int *Resconvexsize,int* Resconcavsize)
{
	//eliminer convexe 
elimination( resconvexe,listconvexe , listconcave,convexesize, concavesize, Resconvexsize);
    //eliminer concave
elimination( resconcave,listconcave , listconvexe,concavesize, convexesize, Resconcavsize);
}

/********************************************************/

/**********************************************/
void take_pictures(cv::Mat *RGB,cv::Mat *depth)
{

cv::
	VideoCapture sensor1;
sensor1.open(CV_CAP_OPENNI);
for(int i=0;i<30;++i)
       {
		   sensor1.grab();
           if( sensor1.retrieve(*depth,CV_CAP_OPENNI_DEPTH_MAP ) )
		    {
			sensor1.retrieve(*RGB,CV_CAP_OPENNI_BGR_IMAGE );
	        }
		    if( cv::waitKey( 30 )==27 )   break;
       }

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
  // calcule les distances entre les points de list 
  vector<double> distance_entre_points(array< pfe_gui::Form1::Point_rest >^ list,int listsize,vector<cv::Point> contourHand)
  {
      double dist=0;
	  int k=0;
	  int taillevec=taille_vect(listsize-1);
	  vector<double> distance(taillevec);

	  for(int i=0;i< (listsize - 1) ;i++)
	  {
	    for(int j=i+1; j<listsize; j++)
		{
		 dist=calculer_distance(contourHand[list[i].indic_point] ,contourHand[list[j].indic_point] );
		 distance[k]=dist;
		 k++;
		} 
	  }
  return distance;
  }
  
  /*********************************************************************/
  // regroupement par distance et elimination par angle des points de fortes courbures , retourne vecteur d'indice des points fortes courbures retenus
  vector<int> filtrage_distance_entre_points(array< pfe_gui::Form1::Point_rest>^ listconc,int listsize,vector<cv::Point> contourHand,int *size_vecindi,double seuil, vector<int> *indic_p_courb,cv::Point brRect)
  {
      double dist=0;
	  int k=0,i=0;
	  double min_angle;
	  int min_indic,indicList;
	  int taillevec=taille_vect(listsize-1);
	  vector<int> distance(listsize);
	
	  int dsize=0;
      //double seuil=5;
	  while(i < (listsize-1) )
	  {
		  while(i<listsize && listconc[i].indic_point == -1 )
			  i++ ; //sauter les cases vides déja traitées 
		  
		  if(i != listsize)
		  { //initialisation
		  min_angle=listconc[i].angle;
	      min_indic=listconc[i].indic_point;
        indicList=i;
	    for(int j=i+1; j<listsize; j++)
		{
			if (listconc[j].indic_point == -1)
				continue;
			//else
		 dist=calculer_distance(contourHand[listconc[i].indic_point] ,contourHand[listconc[j].indic_point] );
		 // vérifier si < seuil , garder l'angle minimum pr l'élimination
		 if(dist < seuil) //regroupement
		 {
			
			 if(listconc[j].angle < min_angle)
			 {
			 min_angle=listconc[j].angle;
			 min_indic=listconc[j].indic_point;
			 indicList=j;
			 }
			  //elimination
			 listconc[j].indic_point=-1;
		 }
		}
		//eliminer les points qui reste de la suppression qui sont prés du br du Rect de la main
		if( (brRect.y - contourHand[min_indic].y ) > 5)
		{
		// insérer point retenu qui a l'angle min et avancer dans laa liste 
		 distance[dsize]=min_indic;
		 (*indic_p_courb)[dsize]=indicList;
		 dsize++;
		}
		 i++;
		} 
	  }

	  *size_vecindi=dsize;
      return distance;
  }
  //-------------------------------------------------------------------------------------
  // retourne indice pouce s'il existe -1 sinon
  int findPouce( vector<int> indic_p_cncv, array< pfe_gui::Form1::Point_rest>^ lconcave, vector<int>filtreConcave,int sizearray,int sizevector)
  {
 
  if(sizevector ==0 ) return -1;
   int var=indic_p_cncv[0];
  int max=0,i;
  for(i=1;i<sizevector;i++)
  {
  if(lconcave[indic_p_cncv[i]].angle>lconcave[indic_p_cncv[max]].angle)
	 max=i;

  }
  if((lconcave[indic_p_cncv[max]].angle > 1) && (lconcave[indic_p_cncv[max]].angle < 1.30))
  {
	  return filtreConcave[max];
  }
  else return -1;
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

  /***********************************************************************/
  //retourne 1 si main droite et 2 si main gauche, on l'appelle si le pouce exist , compare les x entre point concave pouce et autres p concaves
  int direction_main(int pouce, int indice_p,vector<cv::Point> contourHand)
  {

  if( contourHand[pouce].x < contourHand[indice_p].x )
	  return 2;
  else
	  return 1;
  }

  /*****************************************************************************/
  //trouve le point convexe du pouce et l'isole de la liste p convexe, selon le direction p convexe le plus petit/grand qque concave pouce
  int find_convexe_pouce(int pouce, vector<int> filtreconvexe,int size,int direction,vector<cv::Point> contourHand)
  {
	  
  int find=-1,i=0;
  if(direction = 2 ) //main gauche
  {
	  while( i < size && find == -1)
	  {
       if(contourHand[filtreconvexe[i]].x < contourHand[pouce].x ) // cest le point convexe du pouce
		   find=filtreconvexe[i];
	   i++;
	  }
  }
  else 
	  if(direction = 1 ) //main droite
	  {
		  while( i < size && find == -1)
		  {
		   if(contourHand[filtreconvexe[i]].x > contourHand[pouce].x ) // cest le point convexe du pouce
			   find=filtreconvexe[i];
		   i++;
	  }
	  
	  }
  
  return find;
  }
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

  void moindreCarre(int n, vector<cv::Point> listconcave,double *aa,double *bb,double *cc)
{    
       int** matrice = new int*[3];
     for(int i = 0; i <3; i++)
	   matrice[i] = new int[4];

	 vector<double> access(4);
     double coefficient,a,b,c;
     int i=0;
     double xprim,yprim;
	 double somx=0, somy=0,somx2=0, somx3=0, somx4=0, somxy=0,somx2y=0;
	 for(int p=0; p<n ;p++)
	 {
	  
		 xprim=listconcave[i].x;
		 yprim=listconcave[i].y;
		  somx+=xprim;
          somx2+=(xprim*xprim);
          somx3+=(xprim*xprim*xprim);
          somx4+=(xprim*xprim*xprim*xprim);
          somy+=yprim;
          somxy+=(yprim*xprim);
          somx2y+=((xprim*xprim)*yprim);
	 
	 }
    
     matrice[0][0]=somx4;
	 matrice[0][1]=somx3; 
	 matrice[0][2]=somx2; 
	 matrice[0][3]=somx2y;
     matrice[1][0]=somx3;
	 matrice[1][1]=somx2; 
	 matrice[1][2]=somx;  
	 matrice[1][3]=somxy;
     matrice[2][0]=somx2;
	 matrice[2][1]=somx;  
	 matrice[2][2]=n;     
	 matrice[2][3]=somy;
     if(matrice[0][0] != 0)
     coefficient=(-1*matrice[1][0]/matrice[0][0]);
    
     i=0;
     for(i=0;i<=3;i++)
     {
     matrice[1][i]=(coefficient*matrice[0][i])+matrice[1][i];
     }
      if(matrice[0][0] != 0)
     coefficient=(-1*matrice[2][0]/matrice[0][0]);
    
     i=0;
     for(i=0;i<=3;i++)
     {
     matrice[2][i]=(coefficient*matrice[0][i])+matrice[2][i];
     }

	 if(matrice[1][1] != 0 )
     coefficient=(-1*matrice[2][1]/matrice[1][1]);
    
     i=1;
     for(i=0;i<=3;i++)
     {
     matrice[2][i]=(coefficient*matrice[1][i])+matrice[2][i];
     }
      if(matrice[2][2] != 0 )
     c=matrice[2][3]/matrice[2][2];
	  if(matrice[1][1] != 0 )
     b=(matrice[1][3]-(matrice[1][2]*c))/matrice[1][1];
	   if(matrice[0][0] != 0 )
     a=(matrice[0][3]-((matrice[0][1]*b)+(matrice[0][2]*c)))/matrice[0][0];
	 *aa=a;
	 *bb=b;
	 *cc=c;

}     

  
  /***************************************************************/
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
	
	 cv::circle(*img, (*res)[i], 4,CV_RGB(255,0,0),1);
	// cv::line(*img, mc, (*res)[i], CV_RGB(60,48,152), 1, CV_AA, 0);
	 }

     /*cv::imshow("vallées",*img);
	 cv::waitKey(0);*/
     string nom_pics;
   	 nom_pics=concatene_nom_img(nb_alea,"pconcaveeliminer");
	 cv::imwrite(nom_pics,*img);

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

  /*****************************************************************************/
  /********************************************************/
void intersection_courbe_contour(vector<cv::Point> contourHand,double a,double b,double c,cv::Mat *img)
{
 vector<cv::Point> pcourbe(contourHand.size());
	 int taillecourbe=0;
	 cv::Point p;
	 double y_p;
if(a > -1 )
{
// for(int i=RectHand.tl().x;i < RectHand.width;i++)
 for(int i=0;i < contourHand.size() ; i++) //calculer les points qui font partis de la courbe
	 {
	 y_p= solution_y (contourHand[i].x,a,b,c);
	  if( y_p != -1)
	  {
	 // cv::circle(img, cv::Point(contourHand[i].x,y_p), 0.1, cv::Scalar(0, 0, 255), 2); 
      cv::Point inser((contourHand[i].x),y_p);
		  pcourbe[i]=inser;
	  taillecourbe++;
	  }
     }

 bool trouve=false;
 vector<cv::Point> intersect(pcourbe.size());
 int intersecsize=0,k=0,j=0;
 //chercher intersection entre point de courbe et point de contour
 for(int i=0;i<taillecourbe;i++)
 {j=0;
 while( j<contourHand.size() && trouve==false)
 {
	 if/*(contourHand[j].x == pcourbe[i].x )&&*/ (contourHand[j].y == pcourbe[i].y )
	 {
	 intersect[k]=contourHand[j];
	 k++;
     trouve=true;
	 }
 j++;
 }
 }

 for(int i=0;i<k;i++)
 {
cv::circle(*img,intersect[k],4,cv::Scalar(100,200,99),2); 
 }


}


}
/*******************************************************************************/
void dessinerpartie(vector<cv::Point> contour,vector<cv::Point> concave,int size,cv::Mat img)
{
int i=0,j,ind1=-1,ind2=-1;
int indD,indF;
if(ind1<ind2)
{
	indD=ind1;
	indF=ind2;
}
else
{
	indD=ind2;
	indF=ind1;
}
for(i=indD;i<=indF;i++)
{
	cv::circle(img, cv::Point(contour[i]), 0.1, cv::Scalar(102, 232, 255), 2);
}
cv::imshow("partition",img);
cv::waitKey(0);
}
/********************************************************************************/
void above_courbe(double a,double b,double c,vector<cv::Point> contourHand)
{
	double y_p;
	int taillecourbe=0;
	vector<cv::Point> pcourbe;
if(a > -1 )
{
//récupérer les points de la courbe de jonction dans pcourbe
 for(int i=0;i < contourHand.size() ; i++)
	 {
	 y_p= solution_y (contourHand[i].x,a,b,c);
	  if( y_p != -1)
	  {
	 // cv::circle(img, cv::Point(contourHand[i].x,y_p), 0.1, cv::Scalar(0, 0, 255), 2); 
      pcourbe[taillecourbe]=cv::Point(contourHand[i].x,y_p);
	  taillecourbe++;
	 
	  }
  
    }

}
}
/******************************************************************************/
void listetoDrawPartition(vector<cv::Point> contour,vector<cv::Point> concaves,int size,cv::Mat img,vector<cv::Point> *liste)
{
	vector<cv::Point> liste1(size+2);
	cv::Point pt1,pt2;
	int i=0;
	bool trouve=false;
	while(i<contour.size() && trouve==false)
	{
		if((contour[i].x<concaves[0].x-5)&& (contour[i].y-concaves[0].y<7 )&&(contour[i].y-concaves[0].y>2 ))
		{ 
			trouve=true; pt1=contour[i];
		}
		i++;
	}
	trouve=false;
	i=0;
	while(i<contour.size() && trouve==false)
	{
		if((contour[i].x>concaves[size-1].x+5)&& (contour[i].y-concaves[size-1].y<7 )&&(contour[i].y-concaves[size-1].y>2 ))
		{ 
			trouve=true; pt2=contour[i];
		}
		i++;
	}
	liste1[0]=pt1;
	cv::circle(img, pt1, 4, cv::Scalar(0, 200, 255), 2);
          cv::imshow("pt1",img);
        cv::waitKey(0);
	for(i=0;i<size;i++)
	{
	liste1[i+1]=concaves[i];
	}
	liste1[i+1]=pt2;
	cv::circle(img, pt2,4, cv::Scalar(100, 232, 255), 2);
          cv::imshow("pt2",img);
        cv::waitKey(0);
	*liste=liste1;
}
/******************************************************************************/
int min2(int a,int b)
{
if(a<b) return a;
else return b;
}
int max2(int a,int b)
{	
if(a<b) return b;
else return a;
}
 /****************************************************************************/
bool rechecher_p_dans_liste(vector<cv::Point> liste,int size,cv::Point p)
{
	int i=0;
	bool trouve=false;
while(i <size && trouve==false)
{
	if(p == liste[i])
		trouve=true;
	i++;

}
return trouve;
}
/****************************************************************************/
void DrawPartition(vector<cv::Point> contour,vector<cv::Point> liste,int size,cv::Mat img)
{
	int max_y=0;
	int couleur=50,couleur1=50,j=0;
int i=0,ind1=-1,ind2=-1;
int indD,indF;
	for(i=0;i<size-1;i++)
	{
	    max_y=max2(liste[i].y,liste[i+1].y);
	    cv::circle(img,liste[i], 2, cv::Scalar(10, 255, 30), 2);
		cv::circle(img,liste[i+1], 2, cv::Scalar(255, 60, 10), 2);
	  
		for(j=0;j<contour.size();j++)
          {
			  if(contour[j].x > liste[i].x && contour[j].x < liste[i+1].x && contour[j].y < max_y   )
			  {
	         cv::circle(img, cv::Point(contour[j]), 0.1, cv::Scalar(couleur,couleur1,255), 2);
             cv::imshow("dans for draw partition",img);
			 cv::waitKey(0);
			  }
	      }

	   couleur=couleur+30;
	    couleur1=couleur1+30;
	}

cv::imshow("dans draw partition",img);
cv::waitKey(0);

}
/*******************************************************************************/
void DrawPartition2(vector<cv::Point> contour,vector<cv::Point> liste,int size,cv::Mat img)
{
cv::Point pt=liste[0];
int couleur=0,couleur1=0;
int indic=indice_p_dans_contour(contour,pt);
indic++;
if(indic == contour.size()) indic=0;

while(contour[indic] != pt)
{
	while(rechecher_p_dans_liste(liste, size,contour[indic])==false)
	{
		cv::circle(img, cv::Point(contour[indic]), 0.1, cv::Scalar(couleur,couleur1,255), 2);
		indic++;
		if(indic==contour.size()) indic=0;

	}

	    //couleur=couleur+100;
	    couleur1=couleur1+30;
		if(contour[indic] != pt)
		{indic++;
		if(indic==contour.size()) indic=0; }
}

cv::imshow("Draw partition2",img);
cv::waitKey(0);
}

  /**************************************************************************************/
void supp_concave_distance_seuil(vector<cv::Point> *concaves,int *size,cv::Point mc,cv::Mat *img)
{
int seuil=50,j=0;
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

for(int i=0;i<j;i++)
	 {
	
	 cv::circle(*img, res[i], 4,CV_RGB(255,0,255),1);
	 cv::line(*img, mc, res[i], CV_RGB(255,0,0), 1, CV_AA, 0);
	
 /*cv::imshow("dans supp boucle 30",*img);
	 cv::waitKey(0);
*/}

  /*   cv::imshow("dans supp seuil 5 vallées",*img);
	 cv::waitKey(0);
	 */

}


  /**************************************************************************************/
void point_convexe_geste(vector<cv::Point> *concaves,int *size,cv::Point mc,cv::Mat *img,int nb_alea)
{
int seuil=5,j=0;
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

for(int i=0;i<j;i++)
	 {
	
	 cv::circle(*img, res[i], 4,CV_RGB(255,20,100),2);
	// cv::line(*img, mc, (*res)[i], CV_RGB(60,48,152), 1, CV_AA, 0);
	 }

     /*cv::imshow("pics",*img);
	 cv::waitKey(0);*/
     string nom_pics;
   	 nom_pics=concatene_nom_img(nb_alea,"pics");
	 cv::imwrite(nom_pics,*img);
}

/********************************************************************/
void remplir_liste_concaves(vector<cv::Point> *concaves,int *sizecv,vector<cv::Point> convexes,int sizecx,cv::Point mc,cv::Mat img)
{
	if((*sizecv) >  0 && (*sizecv) <3 )
	{
		vector<cv::Point> res(*sizecv + sizecx);
		int j=0;
		double dist1=0,dist2;
		cv::Point pt;
		dist1=calculer_distance((*concaves)[0],mc);

	for(int i=0;i<sizecx;i++)
	{
	cv::LineIterator it(img,convexes[i],mc,8);
		for(int i=0;i< it.count;i++,it++)
		{
		pt=it.pos();
		dist2=calculer_distance(pt,mc);
		if(dist2 == dist1+10 || dist2 == dist1-10)
		{
		res[j]=pt;
		j++;
		}
		}
	
	}
	for(int i=0;i<*sizecv;i++)
	{
	res[j]=(*concaves)[i];
	j++;
		cv::circle(img, res[j], 4,CV_RGB(100,255,100),2);
	}

	(*concaves)=res;
	*sizecv=j;
   }

}
 /****************************************************************************/
String^ descripteur_global(int nbconvexe,int nbconcave,cv::Point concavepouce,cv::Point convexepouce)
{
	String^ phrase="<CP>	";

for(int i=0;i<nbconvexe;i++)
{
phrase=phrase+"	P"+(i+1)+"		";
}

if(nbconcave != 0 ) phrase=phrase+"CJ1	";
phrase=phrase+"	PALM	";
if(concavepouce != cv::Point(0,0) && convexepouce != cv::Point(0,0))
phrase=phrase+"	Thumb";

phrase=phrase+"</CP>";
return phrase;

}

  /****************************************************************************/
void trouver_points_courbures(cv::Mat *img,vector<cv::Point> contourHand,int nb_alea,cv::Point2f mc,cv::Rect boundRect1,cv::Mat original)
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

    point_convexe_geste(&res,&sizeConvxelimin,mc,img,nb_alea);//choisir les p convexes significatifs


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
		string nom_pointfin;
	nom_pointfin=concatene_nom_img(nb_alea,"pointsfinals");
	cv::imwrite(nom_pointfin,*img);

	/**********  dessiner la courbe de jonction ********************************/
	vector<cv::Point> resconcaves(3);
	int sizeres=0;
	filtrage_concave_courbe(concaves,num_concaves,&listfiltrer,&ressize); //elimine les p concaves qui ont le meme x
	eliminer_vallees(listfiltrer,ressize,&listfiltrer,&ressize,img,mc,nb_alea); //elimine les points concaves qui se trouvent dans la meme région
	EliminerConcaveAngle(contourHand,&listfiltrer,&ressize,img); //elimine les p concaves qui ont un grand angle 
	supp_concave_distance_seuil(&listfiltrer,&ressize,mc,img); //elimine les p concaves qui se trouvent sur les doigts (entre cocanves/convexes)
	//chercher concave pouce
	cv::Point concavepouce=chercher_pouce_concave(listfiltrer,ressize);
   if(concavepouce != cv::Point(0,0))
	 cv::circle(*img,concavepouce, 4,CV_RGB(0,255,0),2);
  
   //trier et choisir les p concaves pour dessiner la courbe de jonction
	trier_concaves_jonction(&listfiltrer,ressize);
    Choisir_concaves_valide(listfiltrer,ressize,&resconcaves,&sizeres);
	double aa=0,bb=0,cc=0;
 	courbe_jonction_vectconcave(resconcaves,sizeres,contourHand,*img,nb_alea,boundRect1,&aa,&bb,&cc);
/*	cv::imshow("courbe jonction",*img);  
    cv::waitKey(0);

	/******************* moindre carré *******************************/
	moindreCarre(sizeres,resconcaves ,&aa,&bb,&cc);
	dessiner_courbe_jonction2(*img,contourHand,resconcaves,sizeres,aa,bb,cc,nb_alea,boundRect1);
	/*cv::imshow("moindre carré",*img);  
    cv::waitKey(0);
	*/
	String^ descripteur=descripteur_global(sizeConvxelimin,sizeres,concavepouce,pouce);
	label1->Text=descripteur;
	cv::imwrite("resultat.jpg",*img);
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
 
 ltest->Text="Equation courbe de jonction:	"+x+" x²+	"+y+" x+	"+z;
	
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
		if(size!=0)
		{
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
void courbe_jonction_vectconcave(vector<cv::Point> listconcave,int num_concaves,vector<cv::Point> contourHand,cv::Mat dst2,int numImg,cv::Rect boundRect1,double *a,double *b,double *c)
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
 
 //ltest->Text="Equation courbe de jonction:	"+x+" x²+	"+y+" x+	"+z;
	
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
 
 ltest->Text="Equation courbe de jonction:	"+x+" x²+	"+y+" x+	"+z;
	
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
 nom_courbe=concatene_nom_img(nb_alea,"courbe_jonction");
 cv::imwrite(nom_courbe,img);
 /*cv::imshow("courbe jonction points concave non trié",img);
 cv::waitKey(0);*/
 
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

/***********************************************************************/
  void suppression_bras(cv::Mat *img,cv::Rect boundRect )
 {
 
 int seuil_y= boundRect.tl().y +140;
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
 /*------------------------------------------------------------------------------------------------*/
 void EliminerConcaveAngle(vector<cv::Point> ContourHand,vector<cv::Point> *concave,int *size,cv::Mat *img )
 {

double Angle;
int pas=5;

vector<cv::Point> resultat(*size);

int i,indice,indicePlus,indiceMoins,j=0;
for(i=0;i<(*size);i++)
{

indice=indice_p_dans_contour(ContourHand,(*concave)[i]);

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

	 cv::circle(*img, (resultat)[i], 4,CV_RGB(30,60,80),2);
	// cv::line(*img, mc, (resultat)[i], CV_RGB(255,48,152), 1, CV_AA, 0);
	 }
*concave=resultat;
*size=j;
 }
/*-----------------------------------------------------------------------------------------------*/
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
		 
	 cv::circle(*img, fingers[i], 4,CV_RGB(0,0,255),0.1);
	// cv::line(*img, center_of_mass, fingers[i], CV_RGB(255,255,0), 1, CV_AA, 0);
	/* cv::imshow("Points concaves Masse",*img);
	 cv::waitKey(0);*/
	 }
	// cv::circle(*img, pouce, 4,CV_RGB(0,255,0),0.1);
	/* cv::imshow("Point concave",*img);
	 cv::waitKey(0);*/

     string nom_convexe;
   	 nom_convexe=concatene_nom_img(numImg,"pconcave");
	 cv::imwrite(nom_convexe,*img);
    
	 *num=num_fingers;
	 *concaves=fingers;
 }

 /********************************************************/
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
	double seuil=12;
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
	
	 cv::circle(*img, (*res)[i], 4,CV_RGB(5,20,100),1);
	// cv::line(*img, mc, (*res)[i], CV_RGB(60,48,152), 1, CV_AA, 0);
	 }

    /* cv::imshow("pics",*img);
	 cv::waitKey(0);*/
    string nom_pics;
   	 nom_pics=concatene_nom_img(nb_alea,"picsavtElimin");
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
{ 
	cv::Mat depth2,RGB1,Depth2,result,resultat,thresh,diff;
   
	for(int i=0;i<120;i++)
	{
	 cap.grab();
    cap.retrieve(depth2,CV_CAP_OPENNI_DEPTH_MAP);
    cap.retrieve(*RGB,CV_CAP_OPENNI_BGR_IMAGE);
	}
    depth2.convertTo(Depth2,CV_8UC1,100.0/2048.0);
	cv::threshold(Depth2,result,20, 255, CV_THRESH_BINARY_INV);
    diff=bg-Depth2;
     cv::threshold(diff,resultat,20, 255, CV_THRESH_BINARY);
     cv::threshold(diff,thresh,35, 255, CV_THRESH_BINARY);
     *Mask=thresh-result;
	
}
/********************************************************/
bool above_concave(vector<cv::Point> concaves,int size,cv::Point p)
{
	int i=0; 
	bool trouve=false;
	while(i < size && trouve == false)
	{
	if( p.y > concaves[i].y )
		trouve=true;

	i++;
	}
	return trouve;
}

/****************************************************/
 



/****************************************************/





/****************************************************/
void Partitions(cv::Mat *img,vector<cv::Point> contourHand,double a,double b,double c,vector<cv::Point> concaves,int size)
{
	 double y_p1,y_p2,y_p3 , y_p;
	 vector<cv::Point> pcourbe(contourHand.size());
	 int taillecourbe=0;
	 cv::Point p;
	 int haut_min=solution_y (contourHand[0].x,a,b,c);;
if(a > -1 )
{
// for(int i=RectHand.tl().x;i < RectHand.width;i++)
 for(int i=0;i < contourHand.size() ; i++)
	 {
	 y_p= solution_y (contourHand[i].x,a,b,c);
	  if( y_p != -1)
	  {
	 // cv::circle(img, cv::Point(contourHand[i].x,y_p), 0.1, cv::Scalar(0, 0, 255), 2); 
      pcourbe[i]=cv::Point(contourHand[i].x,y_p);
	  taillecourbe++;
	  if(y_p < haut_min)
		  haut_min=y_p;
	  }
  
    }

 bool trouve=false,sup=false;
 int j=0;
 for(int i=0;i<contourHand.size();i++)
 {j=0;
	
  while(j<pcourbe.size() && sup== false)
  {
   if(contourHand[i].y > pcourbe[j].y )
    // cv::circle(*img, contourHand[i], 4, cv::Scalar(64, 0, 128), 2); 
    sup=true;
     j++;
   }
	  if( j == pcourbe.size()) // point au dessus de la courbe
	  {
	  if( above_concave(concaves,size,contourHand[i])==true )
	  {cv::circle(*img, contourHand[i], 4, cv::Scalar(64, 0, 128), 1); }
  
	  }

}

 cv::imshow("Partitions",*img);
 cv::waitKey(0);


}

}

/***********************************************************/
void Partitions1(cv::Mat *img,vector<cv::Point> contourHand,double a,double b,double c,vector<cv::Point> concaves,int size)
{
	 double y_p1,y_p2,y_p3 , y_p;
	 vector<cv::Point> pcourbe(contourHand.size());
	 int taillecourbe=0;
	 cv::Point p;
	 int haut_min=solution_y (contourHand[0].x,a,b,c);;
if(a > -1 )
{
// for(int i=RectHand.tl().x;i < RectHand.width;i++)
 for(int i=0;i < contourHand.size() ; i++)
	 {
	 y_p= solution_y (contourHand[i].x,a,b,c);
	  if( y_p != -1)
	  {
	 // cv::circle(img, cv::Point(contourHand[i].x,y_p), 0.1, cv::Scalar(0, 0, 255), 2); 
      pcourbe[i]=cv::Point(contourHand[i].x,y_p);
	  taillecourbe++;
	  if(y_p < haut_min)
		  haut_min=y_p;
	  }
  
    }

 bool trouve=false,sup=false;
 int j=0;
 for(int i=0;i<contourHand.size();i++)
 {
	
 
	  if( contourHand[i].y < haut_min) // point au dessus de la courbe
	  {
	  if( above_concave(concaves,size,contourHand[i])==true )
	  {cv::circle(*img, contourHand[i], 2, cv::Scalar(64, 0, 128), 1); }
  
	  }

}

 cv::imshow("Partitions01",*img);
 cv::waitKey(0);


}

}

/********************************************************/
int MyMain()
{	
	/************ MAIN QUI FAIT APPEL A LA KINECT **********************/
	
	srand(time(NULL));
	nb_alea = (rand() % 10000) + 1; //génère un nombre entre 1 et 100 
	System::String^ managedString = nb_alea.ToString();
    msclr::interop::marshal_context context;
    string numImg = context.marshal_as<string>(managedString);

	   /**** Prise de l'image du background ****/

	 cv::Mat imggest,RGBgest,RGB;
     cv::Mat depth;
     cv::VideoCapture cap;
    cap.open(CV_CAP_OPENNI);
     acquisitionBackground(&depth,cap);
	/* cv::imshow("depth",depth);
	 cv::waitKey(0);*/
	 int cpt=0;
	// while(cpt <3000){
	  label5->Text="	Vous pouvez faire un geste !";
		  label5->Refresh();
     nb_alea = (rand() % 10000) + 1;
   	 acquisitionGest(&imggest,&RGB,depth,cap);
	/*  cv::imshow("rgb",RGB);
	 cv::waitKey(0); 
	  cv::imshow("mask",imggest);
	 cv::waitKey(0); 	*/
   // imggest=cv::imread("photossss\\mask7625.jpg"); 
//	RGB=cv::imread("photossss\\RGB7625.jpg"); 
	 
 
	 string nom_rgb;
   	 nom_rgb=concatene_nom_img(nb_alea,"RGB");
	 cv::imwrite(nom_rgb,RGB);
	
	 string nom_mask;
   	 nom_mask=concatene_nom_img(nb_alea,"mask");
	 cv::imwrite(nom_mask,depth);

	 	// String^ nomm="original.jpg";
		string str=nom_rgb;
		String^ nom_RGBimg= gcnew String(str.c_str());
		LoadNewPict(nom_RGBimg,ImgOrigin);

	  

		/**** Segmentation Pronfondeur et Peau****/
		
	    cv::Mat dst=segmentationDirecte(imggest,RGB,nb_alea);
		/*cv::imshow("segment",dst);
		cv::waitKey(0); */
		string nom_seg;
		nom_seg=concatene_nom_img(nb_alea,"segment");
		cv::imwrite(nom_seg,dst);
	

	    /*** eliminer visage ****/

	   
        face_eliminer(RGB,&dst,nb_alea);
        /*cv::imshow("No_face",dst);
        cv::waitKey(0);*/


	/**********************************************************/

	   cv::Rect boundRect;
	   cv::Mat  dst2 =dst.clone();
	 //RGB=cv::imread("photossss\\segment9558.jpg");
	 //LoadNewPict("photossss\\RGB9558.jpg",ImgOrigin);
	 //cv::Mat dst2=cv::imread("photossss\\RGB9558.jpg");
	   //retrouver contour de la main seulement pr faire la paume et puis suppression
	   std::vector<cv::Point> contour;
	   detecterContours(RGB,dst2 ,&boundRect,&contour);

	//  label1->Text="Descripteur Global: \n<CP> P1 P2 P3 P4 <CJ1> PALM Thumb </CP>";

	  /********************* Suppression du bras *************************/

	  suppression_bras(&dst2, boundRect );
	 /* cv::imshow("arm deleted",dst2);
	  cv::waitKey(0);*/
	  string nom_delet;
	  nom_delet=concatene_nom_img(nb_alea,"deletarm");
	  cv::imwrite(nom_delet,dst2);
      
	  

	  /********************* Détecter contourHand aprés suppression ****************************/
	  
	    cv::Rect boundRect1;
	    std::vector<cv::Point> contourHand;
	    detecterContours_modifier(RGB,dst2 ,&boundRect1,&contourHand,nb_alea);
	    if(contourHand.size() > 0 ) {
		cv::Mat original=dst2.clone();

		 /******** Trouver et Dessiner paume de la main ********/
	  
	  int max_ip=0,max_jp=0,taillePaumeMain,br_paume_y;
      int indic=construct_sous_Mat( boundRect1,dst2,&max_ip,&max_jp);
      taillePaumeMain=indic;
	        cv::Point TLpaume;
	 TLpaume=draw_paume_main(RGB,dst2,max_ip,max_jp,indic,boundRect1,&br_paume_y,nb_alea);
	  

	  /********************** Trouver centre de gravité	**********************/
	   
	     cv::Point2f mc;
	     mc=centre_paume(TLpaume,taillePaumeMain,nb_alea,&dst2);


		cv::circle(RGB,mc,4,cv::Scalar(0,0,0),2);
		int size_convexe=0;
		trouver_points_courbures(&dst2,contourHand,nb_alea,mc,boundRect1,original);
		

	  
	}
	else // contour main vide , contour de la main non valide, ne rien faire
  
MessageBox::Show("Contour obtenu non valide,veuillez relancer l'exécution !","Contour non valide",MessageBoxButtons::OK,MessageBoxIcon::Stop);

//	cpt++;}//fin while
	cvDestroyAllWindows(); 
		
	  return 0;

}


/***********************************************************************************************/
int MainBoucle()
{	
	   /************ MAIN QUI FAIT APPEL A LA KINECT **********************/

	srand(time(NULL));

	 
	 /**** Prise de l'image du background ****/

	 cv::Mat bg,imggest,RGBgest,RGB,imggest1,RGB1;
	 cv::Mat depth;
     cv::VideoCapture cap;
     cap.open(CV_CAP_OPENNI);
     acquisitionBackground(&depth,cap);
	 
	 int cpt=0;
     //acquisition_image_geste(&imggest,&RGBgest,&RGB,nb_alea);
	while(cpt<3000)
	 {
		 int	nb_alea = (rand() % 10000) + 1; //génère un nombre entre 1 et 100
	// acquisition_image_boucle(bg,&imggest1,&RGBgest,&RGB1,nb_alea,cap);
   	acquisitionGest(&imggest1,&RGB1,depth,cap);
	 cv::Mat rgbCourbe=RGB1.clone();
	 string nom_rgb;
	 nom_rgb=concatene_nom_img(nb_alea,"RGB");
	 cv::imwrite(nom_rgb,RGB1);


	string nom_mask;
   	 nom_mask=concatene_nom_img(nb_alea,"mask");
	cv::imwrite(nom_mask,imggest1);

   //  RGB=cv::imread(nom_rgb);
	 //imggest=cv::imread(nom_mask);

	 	 String^ nomm="original.jpg";
		string str=nom_rgb;
	String^ nom_RGBimg= gcnew String(str.c_str());
	LoadNewPict(nom_RGBimg,ImgOrigin);

	     /**** changer espace colorimétrique ****/
		/**** Segmentation Pronfondeur et Peau****/
		//cv::Mat dst=segmentationVS(imggest,tab1,tab2);
		cv::Mat dst=segmentationDirecte(imggest1,RGB1,nb_alea);
	//	cv::imshow("RGB1",RGB1);
	//	cv::waitKey(0);
		imwrite("rgb5.jpg",RGB1);
		imwrite("segmentation5.jpg",dst);
		string nom_seg;
//		nom_seg=concatene_nom_img(nb_alea,"segment");
	//	cv::imwrite(nom_seg,dst);


	    /*** eliminer visage ****/



        face_eliminer(RGB1,&dst,nb_alea);
      //  cv::imshow("No_face",dst);
       // cv::waitKey(0);


	/**********************************************************/

	   cv::Rect boundRect1;
	   cv::Mat  dst2 =dst.clone();
	   //cv::Mat dst2=cv::imread("original.jpg");
	   //retrouver contour de la main seulement pr faire la paume et puis suppression
	   std::vector<cv::Point> contourHand;
    detecterContours_modifier(RGB1,dst2,&boundRect1,&contourHand,nb_alea);

//imshow("dessiner contour2",RGB1);
//cvWaitKey(0);


	  /********************* Suppression du bras *************************/

	 /* suppression_bras(&dst2, boundRect1 );
	  cv::imshow("arm deleted",dst2);
	  string nom_delet;
	  nom_delet=concatene_nom_img(nb_alea,"deletarm");
	  cv::imwrite(nom_delet,dst2);
      cv::waitKey(0);


      /************** détection du contour aprés suppression *********************/
	    //cv::Rect boundRect1;
	   // std::vector<cv::Point> contourHand;
	    //detecterContours_modifier(dst2 ,&boundRect1,&contourHand,nb_alea);
	    if(contourHand.size() > 0 ) {


		 /******** Trouver et Dessiner paume de la main ********/

	  int max_ip=0,max_jp=0,taillePaumeMain,br_paume_y;
      int indic=construct_sous_Mat( boundRect1,dst2,&max_ip,&max_jp);
	  taillePaumeMain=indic;
	        cv::Point TLpaume;
	 TLpaume=draw_paume_main(RGB1,dst2,max_ip,max_jp,indic,boundRect1,&br_paume_y,nb_alea);
	  

	  /********************** Trouver centre de gravité	**********************/
	   
	  cv::Point2f mc;
	     mc=centre_paume(TLpaume,taillePaumeMain,nb_alea,&RGB1);


		cv::circle(RGB1,mc,4,cv::Scalar(0,0,0),2);
		int size_convexe=0;


		trouver_points_courbures(&RGB1,contourHand,nb_alea,mc,boundRect1,RGB1 );

		cv::imshow("Resultat final",RGB1);
		cv::waitKey(1);
		imwrite("final5.jpg",RGB1);
        
	}
	else // vecteur vide , contour de la main non valide, ne rien faire

cout<<"Contour obtenu non valide,veuillez relancer l'exécution !Contour non valide"<<endl;



	cpt++;}
	 	cvDestroyAllWindows();
	  return 0;

}
/*************************************************************************************************/

	private: System::Windows::Forms::Button^  Lancer;
	protected: 

	protected: 

	protected: 


private: System::Windows::Forms::PictureBox^  ImgOperation;

	private: System::Windows::Forms::Button^  Quitter;



private: System::Windows::Forms::PictureBox^  ImgOrigin;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label3;


private: System::Windows::Forms::Label^  label4;
private: System::ComponentModel::IContainer^  components;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->Lancer = (gcnew System::Windows::Forms::Button());
			this->ImgOperation = (gcnew System::Windows::Forms::PictureBox());
			this->Quitter = (gcnew System::Windows::Forms::Button());
			this->ImgOrigin = (gcnew System::Windows::Forms::PictureBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->Menu = (gcnew System::Windows::Forms::GroupBox());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->ltest = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImgOperation))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImgOrigin))->BeginInit();
			this->Menu->SuspendLayout();
			this->SuspendLayout();
			// 
			// Lancer
			// 
			this->Lancer->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
			this->Lancer->BackColor = System::Drawing::Color::Lavender;
			this->Lancer->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Lancer->Location = System::Drawing::Point(718, 393);
			this->Lancer->Name = L"Lancer";
			this->Lancer->Size = System::Drawing::Size(137, 33);
			this->Lancer->TabIndex = 2;
			this->Lancer->Text = L"Lancer Application";
			this->Lancer->UseVisualStyleBackColor = false;
			this->Lancer->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// ImgOperation
			// 
			this->ImgOperation->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ImgOperation->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ImgOperation.BackgroundImage")));
			this->ImgOperation->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ImgOperation->Location = System::Drawing::Point(475, 59);
			this->ImgOperation->Name = L"ImgOperation";
			this->ImgOperation->Size = System::Drawing::Size(380, 318);
			this->ImgOperation->TabIndex = 4;
			this->ImgOperation->TabStop = false;
			this->ImgOperation->Click += gcnew System::EventHandler(this, &Form1::ImgOperation_Click);
			// 
			// Quitter
			// 
			this->Quitter->BackColor = System::Drawing::Color::LavenderBlush;
			this->Quitter->DialogResult = System::Windows::Forms::DialogResult::Abort;
			this->Quitter->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Quitter->Location = System::Drawing::Point(718, 435);
			this->Quitter->Name = L"Quitter";
			this->Quitter->Size = System::Drawing::Size(137, 33);
			this->Quitter->TabIndex = 7;
			this->Quitter->Text = L"Quitter";
			this->Quitter->UseVisualStyleBackColor = false;
			this->Quitter->Click += gcnew System::EventHandler(this, &Form1::Quitter_Click);
			// 
			// ImgOrigin
			// 
			this->ImgOrigin->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ImgOrigin->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ImgOrigin.BackgroundImage")));
			this->ImgOrigin->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ImgOrigin.Image")));
			this->ImgOrigin->Location = System::Drawing::Point(207, 59);
			this->ImgOrigin->Name = L"ImgOrigin";
			this->ImgOrigin->Size = System::Drawing::Size(262, 318);
			this->ImgOrigin->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->ImgOrigin->TabIndex = 9;
			this->ImgOrigin->TabStop = false;
			this->ImgOrigin->Click += gcnew System::EventHandler(this, &Form1::ImgOrigin_Click);
			// 
			// label2
			// 
			this->label2->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->label2->Font = (gcnew System::Drawing::Font(L"Nightclub BTN Cn", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(213, 20);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(112, 27);
			this->label2->TabIndex = 10;
			this->label2->Text = L"Image saisie";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label3
			// 
			this->label3->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->label3->Font = (gcnew System::Drawing::Font(L"Nightclub BTN Cn", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(483, 20);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(233, 27);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Transformations Appliquées";
			this->label3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label3->Click += gcnew System::EventHandler(this, &Form1::label3_Click);
			// 
			// label4
			// 
			this->label4->BackColor = System::Drawing::SystemColors::ControlLightLight;
			this->label4->Font = (gcnew System::Drawing::Font(L"Nightclub BTN Cn", 14.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(35, 20);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(149, 27);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Choix Offerts";
			this->label4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->label4->Click += gcnew System::EventHandler(this, &Form1::label4_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(6, 127);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(140, 23);
			this->button1->TabIndex = 21;
			this->button1->Text = L"Contour de la main";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click_2);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(6, 42);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(140, 21);
			this->button2->TabIndex = 22;
			this->button2->Text = L"Image segmentée";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click_1);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(6, 69);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(140, 22);
			this->button3->TabIndex = 23;
			this->button3->Text = L"Image dilatée et érosée";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(6, 156);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(140, 21);
			this->button4->TabIndex = 24;
			this->button4->Text = L"Paume de la main";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(6, 274);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(140, 24);
			this->button5->TabIndex = 25;
			this->button5->Text = L"Points de fortes courbures";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(6, 183);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(140, 25);
			this->button6->TabIndex = 26;
			this->button6->Text = L"Bras supprimé";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(6, 214);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(140, 24);
			this->button7->TabIndex = 27;
			this->button7->Text = L"Centre de la main";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::button7_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(6, 244);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(140, 24);
			this->button8->TabIndex = 28;
			this->button8->Text = L"Points convexes";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::button8_Click);
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(6, 304);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(140, 23);
			this->button9->TabIndex = 29;
			this->button9->Text = L"Détection du Pouce existant";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::button9_Click);
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(6, 333);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(140, 23);
			this->button10->TabIndex = 30;
			this->button10->Text = L"Courbe de jonction";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &Form1::button10_Click);
			// 
			// Menu
			// 
			this->Menu->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->Menu->Controls->Add(this->button12);
			this->Menu->Controls->Add(this->button11);
			this->Menu->Controls->Add(this->button1);
			this->Menu->Controls->Add(this->button10);
			this->Menu->Controls->Add(this->button2);
			this->Menu->Controls->Add(this->button9);
			this->Menu->Controls->Add(this->button3);
			this->Menu->Controls->Add(this->button8);
			this->Menu->Controls->Add(this->button4);
			this->Menu->Controls->Add(this->button7);
			this->Menu->Controls->Add(this->button5);
			this->Menu->Controls->Add(this->button6);
			this->Menu->Location = System::Drawing::Point(29, 50);
			this->Menu->Name = L"Menu";
			this->Menu->Size = System::Drawing::Size(155, 366);
			this->Menu->TabIndex = 31;
			this->Menu->TabStop = false;
			this->Menu->Text = L"Menu";
			this->Menu->Enter += gcnew System::EventHandler(this, &Form1::Menu_Enter);
			// 
			// button12
			// 
			this->button12->Location = System::Drawing::Point(6, 14);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(140, 22);
			this->button12->TabIndex = 32;
			this->button12->Text = L"Premier Plan";
			this->button12->UseVisualStyleBackColor = true;
			this->button12->Click += gcnew System::EventHandler(this, &Form1::button12_Click);
			// 
			// button11
			// 
			this->button11->Location = System::Drawing::Point(6, 97);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(140, 24);
			this->button11->TabIndex = 31;
			this->button11->Text = L"Détection Visage";
			this->button11->UseVisualStyleBackColor = true;
			this->button11->Click += gcnew System::EventHandler(this, &Form1::button11_Click);
			// 
			// ltest
			// 
			this->ltest->AutoSize = true;
			this->ltest->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->ltest->Location = System::Drawing::Point(43, 403);
			this->ltest->Name = L"ltest";
			this->ltest->Size = System::Drawing::Size(0, 13);
			this->ltest->TabIndex = 8;
			this->ltest->Click += gcnew System::EventHandler(this, &Form1::ltest_Click);
			// 
			// label1
			// 
			this->label1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->label1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label1->Font = (gcnew System::Drawing::Font(L"Modern No. 20", 11.25F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(282, 403);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(382, 44);
			this->label1->TabIndex = 32;
			this->label1->Text = L"Descripteur Global :";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// label5
			// 
			this->label5->BackColor = System::Drawing::SystemColors::ButtonHighlight;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(59, 434);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(148, 33);
			this->label5->TabIndex = 33;
			this->label5->Text = L"Indicateur";
			// 
			// Form1
			// 
			this->AcceptButton = this->Lancer;
			this->AccessibleRole = System::Windows::Forms::AccessibleRole::Cursor;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveBorder;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->CancelButton = this->Quitter;
			this->ClientSize = System::Drawing::Size(885, 490);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Menu);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->ImgOrigin);
			this->Controls->Add(this->ltest);
			this->Controls->Add(this->Quitter);
			this->Controls->Add(this->ImgOperation);
			this->Controls->Add(this->Lancer);
			this->Cursor = System::Windows::Forms::Cursors::Hand;
			this->DoubleBuffered = true;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Traitement d\'image";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImgOperation))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ImgOrigin))->EndInit();
			this->Menu->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

			 /*******************************************************************/
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 
			 }

			 /*******************************************************************/
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

				
  //cv::Mat RGBgest;
	int erreur=0;
	//RGBgest=MyMainComplet(0,&erreur);
	if(erreur != -1 )
	{
	erreur=MyMain();
	StartMain=1;
	}
	else
MessageBox::Show("Erreur survenue lors de l'exécution de l'application !","Erreur",MessageBoxButtons::OK,MessageBoxIcon::Error);
	//RGBgest=MyMainComplet(&erreur);
   //if  ( RGBgest.rows != 0 || RGBgest.cols != 0 ) 
	//{   //ltest->Text="it's working";
    /* LoadNewPict("\\RGBgest.jpg");
	 StretchPic("\\RGBgest.jpg");
    }*/
}

			 /*******************************************************************/
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }

			 /*******************************************************************/
private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			  if (MessageBox::Show("Voulez-vous quitter cet onglet?","Traitement d'image",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{//Application::ExitThread();
			  this->Hide();}
	else  e->Cancel= true;
		 }


				   /*******************************************************************/
		 public:	void LoadNewPict(String^ s,System::Windows::Forms::PictureBox^  box)
   {
      // You should replace the bold image 
      // in the sample below with an icon of your own choosing.
        
		// ImgOperation->Image = Image::FromFile(s); 
	 
		  try
      {
         Bitmap^ image1 = dynamic_cast<Bitmap^>(Image::FromFile( s, true ));
         /*TextureBrush^ texture = gcnew TextureBrush( image1 );
         texture->WrapMode = System::Drawing::Drawing2D::WrapMode::Tile;
         Graphics^ formGraphics = this->CreateGraphics();
         formGraphics->FillEllipse( texture, RectangleF(90.0F,110.0F,100,100) );*/
		 box->Image= image1;
         box->SizeMode = PictureBoxSizeMode::StretchImage;
		 box->Refresh();
		 //delete formGraphics;
      }
      catch ( System::IO::FileNotFoundException^ ) 
      {
         MessageBox::Show( "Impossible de faire l'affichage ,Image non disponible !","Erreur" );
      }


	  
   }


   /*******************************************************************/

private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e) {
   
		 }

		 /*******************************************************************/
private: System::Void quitterToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 Application::Exit();
		 }

		 /*******************************************************************/
private: System::Void aideToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
			 	 Aide^ frm= gcnew Aide();
				 frm->ShowDialog();

		 }
		 /*******************************************************************/
private: System::Void Depth_Click(System::Object^  sender, System::EventArgs^  e) {
			//try{ 
				LoadNewPict("depth.jpg",ImgOperation);
             			 
			//}
			/*catch {
 MessageBox::Show("Au moins une exécution de l'application est requise","Warning",MessageBoxButtons::OK,
         MessageBoxIcon::Hand);
			}*/
		 }
		 private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
			}

		  /*******************************************************************/
private: System::Void Segment_Click(System::Object^  sender, System::EventArgs^  e) {
			  //try {
				  LoadNewPict("segment.jpg",ImgOperation);
			    
			  //}
/*			 catch {
 MessageBox::Show("Au moins une exécution de l'application est requise","Warning",MessageBoxButtons::OK,
         MessageBoxIcon::Hand);
			}*/
		 }

		 /*******************************************************************/
private: System::Void Erolate_Click(System::Object^  sender, System::EventArgs^  e) {
			//try{  
				LoadNewPict("dilaterose.jpg",ImgOperation);
			   
		    //}
			/*catch {
 MessageBox::Show("Au moins une exécution de l'application est requise","Warning",MessageBoxButtons::OK,
         MessageBoxIcon::Hand);
			}*/
		 }
		 /*******************************************************************/
private: System::Void Contour_Click(System::Object^  sender, System::EventArgs^  e) {

			// try{ 
			 String^ nom_contour;
			 string str=concatene_nom_img(nb_alea,"contours");
			 nom_contour = gcnew String(str.c_str());
           //  Console::WriteLine(str2);
				 LoadNewPict(nom_contour,ImgOperation);
			     
		 //}

			 /*catch {
 MessageBox::Show("Au moins une exécution de l'application est requise","Warning",MessageBoxButtons::OK,
         MessageBoxIcon::Hand);
			}*/
		 }
		 /*******************************************************************/
private: System::Void PaumeHand_Click(System::Object^  sender, System::EventArgs^  e,System::Windows::Forms::PaintEventArgs^  e1) {
			
			 String^ nom_paume;
			 string str=concatene_nom_img(nb_alea,"paume");
			 nom_paume =gcnew String(str.c_str());
			 LoadNewPict(nom_paume,ImgOperation);
			

			
      // Draw image to screen.
      //e1->Graphics->DrawImage( newImage, ulCorner );
		 //}
			/*catch {
 MessageBox::Show("Au moins une exécution de l'application est requise","Warning",MessageBoxButtons::OK,
         MessageBoxIcon::Hand);
			}*/
		 }
		 /*******************************************************************/

private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
		 /**************************************************/
private: System::Void ImgOperation_Click(System::Object^  sender, System::EventArgs^  e) {
			
		 }
		 /**************************************************/
private: System::Void Pred_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
		 /************************************************/
private: System::Void Quitter_Click(System::Object^  sender, System::EventArgs^  e) {
    if (MessageBox::Show("Voulez vous quitter cet onglet?","Traitement d'image",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{
			  this->Hide();
	if (MessageBox::Show("Voulez vous quitter l'application?","Application",MessageBoxButtons::YesNo,MessageBoxIcon::Question)
					 == System::Windows::Forms::DialogResult::Yes)
	{Application::ExitThread();
	}
	} 

		 }
		 /*******************************************************/
private: System::Void ModeB_Click(System::Object^  sender, System::EventArgs^  e) {

		 }

		 /************************************************************/

private: System::Void ImgOrigin_Click(System::Object^  sender, System::EventArgs^  e) {
			  
/*			 LoadNewPic("\\RGBgest.jpg");
             StretchPic("\\RGBgest.jpg");*/
		 }

		 /****************************************/

private: System::Void checkedListBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
		 }

		 /****************************************/

private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if( StartMain == 1){
				 string str=concatene_nom_img(nb_alea,"depth");
				String^ nom_depth= gcnew String(str.c_str());
			 LoadNewPict(nom_depth,ImgOperation);
			
			 }
			 else
MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
			 //afficher image depth
			/* int erreur=0;
cv::Mat depth=MyMainComplet(1,&erreur);
System::Drawing::Image ^img=depth;
if( erreur != -1 ){
		this->ImgOperation->Image::set = this->ImgOrigin->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ImgOrigin.img")));
			//(cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"ImgOperation.BackgroundImage")));
		this->ImgOperation->ImageLayout = System::Windows::Forms::ImageLayout::Stretch;
		}*/
		 }

		 /*************************************************/

private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

		 /*****************************************************************/
private: System::Void ltest_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
		 /****************************************************************************/

private: System::Void button1_Click_2(System::Object^  sender, System::EventArgs^  e) {
			  if( StartMain == 1){
				 string str=concatene_nom_img(nb_alea,"contours");
				  String^ nom_depth= gcnew String(str.c_str());
			 LoadNewPict(nom_depth,ImgOperation);
			 }
			 else
MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
			 //afficher image depth
		 }
		 /****************************************************************************/

private: System::Void button2_Click_1(System::Object^  sender, System::EventArgs^  e) {
			  if(StartMain == 1){
			   string str=concatene_nom_img(nb_alea,"segment");
			   String^ nom_seg=  gcnew String(str.c_str());
			 LoadNewPict(nom_seg,ImgOperation);
			 }
		   else 
			   MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
		 }
		 /****************************************************************************/

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			  if(StartMain == 1 ){ 
				 string str=concatene_nom_img(nb_alea,"dilaterose");
				  String^ nom_dilat =  gcnew String(str.c_str());
		    LoadNewPict(nom_dilat,ImgOperation);
			}
			 else 
				 MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
		 }
		 /****************************************************************************/
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 	if(StartMain == 1 ) {
				string str=concatene_nom_img(nb_alea,"paume");
					String^ nom_paume =gcnew String(str.c_str());
			 LoadNewPict(nom_paume,ImgOperation);
			}
			else 
				MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
		 }
		 /****************************************************************************/

private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			  if (StartMain ==1 ) {
				 string str=concatene_nom_img(nb_alea,"pointsfinals");
				  String^ nom_point= gcnew String(str.c_str());
			     LoadNewPict(nom_point,ImgOperation);
			 }
			 else 
				 MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
		 }
		 /****************************************************************************/

private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
			 	if (StartMain ==1 ) {
			 string str=concatene_nom_img(nb_alea,"deletarm");
					String ^ nom_delet=gcnew String(str.c_str());
			 LoadNewPict(nom_delet,ImgOperation);
			 }
			 else 
				 MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
			 
		
		 }
		 /****************************************************************************/

private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
			 	 if (StartMain ==1 ) {
 			 string str=concatene_nom_img(nb_alea,"centrepaume");
					 String^ nom_centreg= gcnew String(str.c_str());
		     LoadNewPict(nom_centreg,ImgOperation);
			 }
			 else 
				 MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
			 
		 }
		 /****************************************************************************/

private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
			  if (StartMain ==1 ) {

				  string str=concatene_nom_img(nb_alea,"pics");
				  String ^ nom_pics= gcnew String(str.c_str());
		     LoadNewPict(nom_pics,ImgOperation);
			 }
			 else 
				 MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
	
		 }
		 /****************************************************************************/

private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
			 	if(directionpouce== -1) 
			{//pouce non détecté
MessageBox::Show("Pouce absent sur l'image !","Direction du Pouce",MessageBoxButtons::OK,MessageBoxIcon::Information);
			}   
			else // pouce détecté 
			{
				
MessageBox::Show("Le pouce est présent sur l'image !","Détection du Pouce",MessageBoxButtons::OK,MessageBoxIcon::Information);
		 
			//afficher l'image avc les points et pouce à part
			if (StartMain ==1 && directionpouce != -1 ) 
			{
				string str=concatene_nom_img(nb_alea,"pointsfinals");
				String^ nom_withoutpouce=gcnew String(str.c_str());	 
		     LoadNewPict(nom_withoutpouce,ImgOperation);
			 }
			 else 
				 MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
			 
			
			}
		 }
		 /****************************************************************************/

private: System::Void domainUpDown1_SelectedItemChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
		 /****************************************************************************/

private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {
			  if( StartMain == 1){
				string str=concatene_nom_img(nb_alea,"courbe_jonction");
				  String^ nom_depth= gcnew String(str.c_str());
			 LoadNewPict(nom_depth,ImgOperation);
			 
			 }
			 else
MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
		 }
private: System::Void Menu_Enter(System::Object^  sender, System::EventArgs^  e) {
		 }
		 /****************************************************************************/

private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(StartMain == 1 ) {
				string str=concatene_nom_img(nb_alea,"No_face");
				 String^ nom_face =gcnew String(str.c_str());
			 LoadNewPict(nom_face,ImgOperation);
			}
			else 
				MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
	
		 }
		 /****************************************************************************/

private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(StartMain == 1 ) {
				string str=concatene_nom_img(nb_alea,"mask");
				String^ nom_mask =gcnew String(str.c_str());
			 LoadNewPict(nom_mask,ImgOperation);
			}
			else 
				MessageBox::Show("Veuillez lancer l'application avant l'affichage des images obtenues !","Affichage Impossible",MessageBoxButtons::OK,MessageBoxIcon::Stop);
	
		 }
		 /****************************************************************************/

private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

