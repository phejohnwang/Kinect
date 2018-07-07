#ifndef RAT_H
#define RAT_H

#include <opencv2/opencv.hpp>

using namespace cv;

struct scale_t
{
	double lower, upper;
	int nFeatures;
	double *pFeatureMax;
	double *pFeatureMin;
};

typedef struct _RAT
{
	int id;

	Point centroid;	//after coordinates transformation
	Point nose;	//after oordinates transformation
	Point tail;	//after oordinates transformation

	int vx;
	int vy;
	double speed;

	float mouselength;
	float mousewidth;
	double ellipticity;

	double bodyarea;
	double bodyradius;

	double circularity;

	double NCT_Angle; //nose - centroid - tail angle
} RAT;

#endif