//
//  main.cpp
//  CSC323 Project 1(K-Mean clustering)
//
//  Created by Joel Morel on 2/11/18.
//  Copyright © 2018 Joel Morel. All rights reserved.
//
#include <math.h>
#include <fstream>
#include <iostream>
using namespace std;

ifstream inFile;
ofstream outFile1;

class Point
{
public:
    int xCoordinate, yCoordinate, label;
    double distance;
    
    void printPoint(Point)
    {
        cout << this->xCoordinate << " " << this->yCoordinate;
    }
};

class KMean
{
public:
    
    struct xyCoord
    {
        int xCoordinate;
        int yCoordinate;
        int label;
    };
    
    int k, numPts, numRow, numCol, changeFlag;
    
    int** imageArray;
    Point* pointSet;
    xyCoord* kCentroids;
    int* sumCount;
    int* sumX;
    int* sumY;
    double* distance;
    
    KMean(int k, int numPts, int numRow, int numCol)
    {
        this->k = k;
        this->numPts = numPts;
        this->numRow = numRow;
        this->numCol = numCol;
        
        this->imageArray = new int*[numRow];
        for(int i=0; i < numRow; i++)
        {
            imageArray[i] = new int[numCol];
        }
        
        this->pointSet = new Point[numPts];
        this->kCentroids = new xyCoord[k+1];
        this->sumCount = new int[k+1];
        this->sumX = new int[k+1];
        this->sumY = new int[k+1];
        this->distance = new double[k+1];
        
    }
    
    void loadPointSet()
    {
        int i = 0, x, y;
        
        while(inFile >> x)
        {
            this->pointSet[i].xCoordinate = x;
            inFile >> y;
            this->pointSet[i].yCoordinate = y;
            i++;
        }
        
    }
    
    void assignLabel()
    {
        int label = 1;
        
        for(int i = 0; i < this->numPts; i++)
        {
            if(label > this->k)
            {
                label = 1;
            }
            
            this->pointSet[i].label = label;
            label++;
        }
    }
    
    double computeDistance(Point &a, xyCoord &b)
    {
        double result = 0;
       
        result = ((a.xCoordinate - b.xCoordinate)^2 + (a.yCoordinate - b.yCoordinate)^2);
        return result;
    }
    
    void mapPoint2Image()
    {
        int x,y;
        for(int i = 0; i < this->numPts; i++)
        {
            x = this->pointSet[i].xCoordinate;
            y = this->pointSet[i].yCoordinate;
            this->imageArray[x][y] = this->pointSet[i].label;
        }
    }
    
    void computeCentroid()
    {
        int label;
        
        for(int i = 0; i < this->numPts; i++)
        {
            label = this->pointSet[i].label;
            this->sumX[label] += this->pointSet[i].xCoordinate;
            this->sumY[label] += this->pointSet[i].yCoordinate;
            this->sumCount[label]++;
        }
        
        for(int i = 1; i <= this->k; i++)
        {
            this->kCentroids[i].xCoordinate = (this->sumX[i]/this->sumCount[i]);
        }
    }
    
    void clusterCheck()
    {
        double minDistance;
        int label=0, minLabel=0;
        
        for(int i = 0; i < this->numPts; i++)
        {
            label = this->pointSet[i].label;
            minDistance = 99999.0;
            
            for(int j = 1; j <= this->k; i++)
            {
                this->distance[j] = computeDistance(this->pointSet[i], this->kCentroids[j]);
                if(j < minDistance)
                {
                    minDistance = this->distance[j];
                    minLabel = j;
                }
            }
            
            if(this->pointSet[i].label != minLabel)
            {
                this->pointSet[i].label = minLabel;
                this->changeFlag++;
            }
        }
    }
    
    void printPointSet()
    {
        cout << "Writing point set to file..." << endl;
        
        for(int i = 0; i < this->numPts; i++)
        {
            outFile1 << this->pointSet[i].xCoordinate << " " << this->pointSet[i].yCoordinate << "   " << this->pointSet[i].label << endl;
        }
    }
    
    void PrettyPrint()
    {
        for (int i = 0; i < this->numRow; i++)
        {
            for ( int j = 0; j < this->numCol; j++)
            {
                if(this->imageArray[i][j] > 0)
                {
                    outFile1 << this->imageArray[i][j] << " ";
                }
                else
                {
                    outFile1 << " ";
                }
            }
            outFile1 << endl;
        }
        
    }
    
};

int main(int argc, const char * argv[])
{
    int k, numPts,numRow,numCol;
    
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    
    if (!inFile)
    {
        cerr << "Unable to open file";
        exit(1);   // call system to stop
    }
    
    inFile >> numRow;
    inFile >> numCol;
    inFile >> numPts;
    
    cout << "Enter the number of K-clusters: ";
    cin >> k;
    
    KMean* cluster = new KMean(k, numPts, numRow, numCol);
    cluster->loadPointSet();
    cluster->assignLabel();
    cluster->printPointSet();
    
    while(cluster->changeFlag > 0)
    {
        cluster->mapPoint2Image(); // step 3
        cluster->PrettyPrint();
        cluster->changeFlag = 0;
        cluster->computeCentroid();
        cluster->clusterCheck(); // step 7
    }
    
    inFile.close();
    outFile1.close();
    
    return 0;
    
}
