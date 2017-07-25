//
//  main.cpp
//  PuzzleSolver
//
//  Created by Joe Zeimen on 4/4/13.
//  Copyright (c) 2013 Joe Zeimen. All rights reserved.
//

#include <iostream>
#include <string.h>
#include "puzzle.h"
#include <cassert>
#include "utils.h"
#include "PuzzleDisjointSet.h"
#include <Windows.h>
#include <stdint.h>
#include "utils.h"
#include <string>
#include <iostream>
#include <filesystem>
using namespace cv;
using namespace std;

//Dont forget final "/" in directory name.
static const std::string input = "C:\\jig\\Scans\\";
static const std::string output = "C:\\jig\\finaloutput.png";

///CONTINUE GETTING VECTOR IN 8 NEIGBORHOOD



bool isSameColor(Vec3b a, Vec3b b)
{
	double thresh = 0.75 ;

	//cout << "brightness threshold: " << thresh << '\n';

	double blueRatio = a.val[0] / (b.val[0]+0.01);

	double greenRatio = a.val[1] / (b.val[1]+0.01);

	double redRatio = a.val[2] / (b.val[2]+0.01);

	double ratioMean = (blueRatio + greenRatio + redRatio)/3 ;

	double ratioVariance = (abs(blueRatio - ratioMean) + abs(greenRatio - ratioMean) + abs(redRatio - ratioMean)) / 3;

	if (ratioVariance < thresh)
	{
	//	cout << ratioVariance << "\n";
		return true;
	}
		
	return false;
}


int CalcMHWScore(vector<int> scores)
{
	int median;
	size_t size = scores.size();

	sort(scores.begin(), scores.end());

	if (size % 2 == 0)
	{
		median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
	}
	else
	{
		median = scores[size / 2];
	}

	return median;
}


double euclidDist(Vec3b a, vector<int> b)
{

	return sqrt((a.val[0] - b[0]) ^ 2 + (a.val[1] - b[1]) ^ 2 + (a.val[2] - b[2]) ^ 2);
}

double euclidDist2(Vec3b a, Vec3b b)
{

	return sqrt((a.val[0] - b.val[0]) ^ 2 + (a.val[1] - b.val[1]) ^ 2 + (a.val[2] - b.val[2]) ^ 2);
}

vector <int> meidianVec(Vec3b Vecs[8])
{

	vector<int> blue (8);
	vector<int> green (8);
	vector<int> red (8);

	for (int i = 0; i < 8; i++)
	{
		blue[i] = Vecs[i].val[0];
		
		green[i] = Vecs[i].val[1];

		red[i] = Vecs[i].val[2];

	}

	return{ CalcMHWScore(blue), CalcMHWScore(green), CalcMHWScore(red) };
	
}
bool checkIfToblaken(Vec3b current, vector<int> medianVec, Vec3b a, Vec3b b, Vec3b c, Vec3b d, Vec3b e, Vec3b f, Vec3b g, Vec3b h)
{
	int thresh = 1.5;

	if (euclidDist(current, medianVec) < thresh || euclidDist2(current, a) < thresh || euclidDist2(current, b) < thresh, euclidDist2(current, c) < thresh, euclidDist2(current, d) < thresh, euclidDist2(current, e) < thresh, euclidDist2(current, f) < thresh, euclidDist2(current, g) < thresh, euclidDist2(current, h) < thresh)
	{
		return true;

	}
	else
	{
		if (isSameColor(current, a) || isSameColor(current, b) || isSameColor(current, c) || isSameColor(current, d) || isSameColor(current, e) || isSameColor(current, f) || isSameColor(current, g) || isSameColor(current, h))
		{
			return true;


		}
	}

	return false;
}

int main(int argc, const char * argv[])
{

	cv::Mat image = cv::imread("green.jpg");

	/*
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.

	resizeWindow("Display window", 600, 600);

	waitKey(0);
	*/
	int weidth  = image.cols-10;
	int height = image.rows-10;

	//bool touched1[4000][4000] = { { false } };

	bool ** touched = new bool*[height];

	for (int i = 0; i < height; ++i)
	{
		touched[i] = new bool[weidth];
		for (int j = 0; j < weidth; j++)
			touched[i][j] = false;
	}

	cout << weidth << " " << height;
	int hheight = height / 2;
	int hweidth = weidth / 2;

	cout.precision(17);

	cout << "\n";
	
	Vec3b a = image.at<Vec3b>(0, 0) ;
	

	Vec3b b=  image.at<Vec3b>(0, weidth);

	Vec3b c =  image.at<Vec3b>(0, hweidth );
	

	Vec3b d= image.at<Vec3b>(height, 0);
	

	Vec3b e= image.at<Vec3b>(hheight, 0);
	

	Vec3b f= image.at<Vec3b>(height, hweidth);
	

	Vec3b g= image.at<Vec3b>(height, weidth);
	

	Vec3b h=  image.at<Vec3b>(hheight, weidth);
	

	cout << "\n";

//	image.at<Vec3b>(hheight, weidth) = { 0, 0, 0 };

	int thresh = 6;
	cout << "similarity threshold: " << thresh << '\n';

	Vec3b Vecs[8] = { image.at<Vec3b>(0, 0), image.at<Vec3b>(0, weidth), image.at<Vec3b>(0, hweidth), image.at<Vec3b>(height, 0), image.at<Vec3b>(hheight, 0), image.at<Vec3b>(height, hweidth), image.at<Vec3b>(height, weidth), image.at<Vec3b>(hheight, weidth) };

	vector<int> medianVec= meidianVec(Vecs);

   cout << medianVec[0] <<" "<< medianVec[1]<<" " << medianVec[2];

	cout << "\n";

	int dist = 40;
	bool curD = true;
	bool curU = true;
	bool curL = true;
	bool curR = true;

	int count = 0;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < weidth; j++)
		{
			if (touched[i][j])
			{
				image.at<Vec3b>(i, j) = { 0, 0, 0 };
				count++;
			}
			else
			{

				Vec3b current = image.at<Vec3b>(i, j);
				if (j - dist >= 0)
				{
					Vec3b currentD = image.at<Vec3b>(i, j - dist);
					curD = checkIfToblaken(currentD, medianVec, a, b, c, d, e, f, g, h);
					if (curD)
					{
						touched[i][j - dist] = true;
					}
				}
				if (i - dist >= 0)
				{
					Vec3b currentR = image.at<Vec3b>(i - dist, j);
					bool curR = checkIfToblaken(currentR, medianVec, a, b, c, d, e, f, g, h);
					if (curR)
					{
						touched[i - dist][j] = true;

					}
				}
				if (i + dist < height)
				{
					Vec3b currentL = image.at<Vec3b>(i + dist, j);
					bool curL = checkIfToblaken(currentL, medianVec, a, b, c, d, e, f, g, h);

					if (curL)
					{
					touched[i + dist][j] = true;

					}



				}

				if (j + dist < weidth)
				{
					Vec3b currentU = image.at<Vec3b>(i, j + dist);
					bool curU = checkIfToblaken(currentU, medianVec, a, b, c, d, e, f, g, h);

					if (curU)
					{
						touched[i][j + dist] = true;

					}


				}

				bool cur = checkIfToblaken(current, medianVec, a, b, c, d, e, f, g, h);

				if (cur && curD && curL && curR && curU)
				{
					image.at<Vec3b>(i, j) = { 0, 0, 0 };



				}
			}

		}

	}
	
	cout <<"\n" << count << " " << weidth*height ;

	Size size(600, 600);//the dst image size,e.g.100x100
	resize(image, image, size);//resize image


	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);                   // Show our image inside it.
	waitKey(0);

    return 0;
}

