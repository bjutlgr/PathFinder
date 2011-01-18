#ifndef CBOBSMAP_H
#define CBOBSMAP_H

///////////////////////////////////////////////////////////////////////
//
//		File: CBobsMap.h
//
//		Author: Mat Buckland
//
//		Desc: Class for defining the map described in chapter 3
//
///////////////////////////////////////////////////////////////////////

#include "stdlib.h"
#include "windows.h"
#include <vector>
#include "Barrier.h"
#include "defines.h"


using namespace std;




class CBobsMap
{
private:
	
	//storage for the map
	static const int	map[MAP_HEIGHT][MAP_WIDTH];
	//lihx barries points 
	static const int	m_aBarriesCount[MAX_BARRIERS];
	static const SPoint	m_sp1[BARRIER_COUNT1];
	static const SPoint	m_sp2[BARRIER_COUNT2];
	static const SPoint	m_sp3[BARRIER_COUNT3];
	static const SPoint	m_sp4[BARRIER_COUNT4];
	static const SPoint	m_sp5[BARRIER_COUNT5];
	static const SPoint	m_sp6[BARRIER_COUNT6];
	//lhx	barries
	vector<CBarrier>	m_vecBarriers;
	static const SPoint m_spA;
	static const SPoint	m_spB;

	static const int	m_iMapWidth;
	static const int	m_iMapHeight;

	//index into the array which is the start point
	static const int	m_iStartX;
	static const int	m_iStartY;

	//and the finish point
	static const int	m_iEndX;
	static const int	m_iEndY;

	//lhx
	 HPEN				 m_OldPen ;
	 HPEN				 m_GreenPen;
	 HPEN				 m_RedPen;


public:

	//we can use this array as Bobs memory if rqd
	int					memory[MAP_HEIGHT][MAP_WIDTH];
	
	//lihx  diagonal length
	float				 m_fDiagonalLength;
	RECT				 m_recBound;
	vector<vector<WayPoint>> m_TestRoute;
	vector<vector<SPoint>>  m_BestRoute;


	CBobsMap();
	
	//takes a string of directions and see's how far Bob
	//can get. Returns a fitness score proportional to the 
	//distance reached from the exit.

	//double TestRoute(const vector<int> &vecPath, CBobsMap &memory);

	//given a surface to draw on this function uses the windows GDI
	//to display the map.
	void Render(const int cxClient, const int cyClient, HDC surface);

	//draws whatever path may be stored in the memory
		
	void RenderOriginRoute(const int cxClient, const int cyClient, HDC surface);
	void RenderShortestRoute(const int cxClient, const int cyClient, HDC surface);

	void ResetMemory();

	bool IsValidPoint (const SPoint &point);
	bool BarrierIntersection(const SPoint &a,const SPoint &b);
	
	//lihx gather one valid path
	bool GetOneValidPath(vector<int> &vecBits,int chromolen);
	vector<SPoint> FixToBestPath(const vector<WayPoint> &waypoints );
	//calculate invlid point count
	int CalculateInvalidPointCount(const vector<WayPoint> &waypoints);
	float GetPathLength(const vector<SPoint> &path);
	//transLate
	SPoint TransRelativePointToAbusolutePoint(const SPoint& src);
	//test Route
	vector<WayPoint>  Decode(const vector<int> &ycoodinate);

	double TestRoute(const vector<WayPoint> &vecWayPoints);

	


};



#endif



			