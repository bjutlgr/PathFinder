#include "CBobsMap.h"


//this defines our little maze which Bob wanders
//around in
const int CBobsMap::map[MAP_HEIGHT][MAP_WIDTH] = 
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
 8, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1,
 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1,
 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1,
 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5,
 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

//lhx dirct path
const SPoint CBobsMap::m_spA(START_X,START_Y);
const SPoint CBobsMap::m_spB(END_X,END_Y);

const SPoint CBobsMap::m_sp1[BARRIER_COUNT1]=
{
	SPoint(290.76, 58),
	SPoint(267.74 ,89),
	SPoint(295.4 ,109),
	SPoint(318.42 ,79),
	
};
const SPoint CBobsMap::m_sp2[BARRIER_COUNT2]=
{
	SPoint(258,123),
	SPoint(217,119),
	SPoint(242,182),
	
};
const SPoint CBobsMap::m_sp3[BARRIER_COUNT3]=
{
	SPoint(317,143),
	SPoint(327,173),
	SPoint(355,164),
	SPoint(344,133),
	
};
const SPoint CBobsMap::m_sp4[BARRIER_COUNT4]=
{
	SPoint(294,275),
	SPoint(265,240),
	SPoint(226,277),
	
};
const SPoint CBobsMap::m_sp5[BARRIER_COUNT5]=
{
	SPoint(155,208),
	SPoint(167,245),
	SPoint(215,230),
	SPoint(203,192),
	
};
const SPoint CBobsMap::m_sp6[BARRIER_COUNT6]=
{
	SPoint(107,118),
	SPoint(107,138),
	SPoint(142,138),
	SPoint(142,118),
	
};

const int CBobsMap::m_aBarriesCount[MAX_BARRIERS]=
{
	 BARRIER_COUNT1	,
	 BARRIER_COUNT2	,
	 BARRIER_COUNT3	,
	 BARRIER_COUNT4	,
	 BARRIER_COUNT5	,
	 BARRIER_COUNT6
};

const int CBobsMap::m_iMapHeight = MAP_HEIGHT;
const int CBobsMap::m_iMapWidth  = MAP_WIDTH;

const int CBobsMap::m_iStartX = 14;
const int CBobsMap::m_iStartY = 7;

const int CBobsMap::m_iEndX = 0;
const int CBobsMap::m_iEndY = 2;


//-------------------------------Render -----------------------------

void CBobsMap::Render(const int cxClient,
					  const int cyClient,
					  HDC surface)
{
	const int border = 20;

	int BlockSizeX = (cxClient - 2*border)/m_iMapWidth;
	int BlockSizeY = (cyClient - 2*border)/m_iMapHeight;

	HBRUSH	BlackBrush, RedBrush, OldBrush;
	HPEN	NullPen, OldPen,RedPen;

	//grab a null pen so we can see the outlines of the cells
	NullPen = (HPEN)GetStockObject(BLACK_PEN);

	//grab a red pen 
	RedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	//grab a brush to fill our cells with
	BlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//create a brush for the exit/entrance points
	RedBrush = CreateSolidBrush(RGB(255,0,0));

	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(surface, BlackBrush);
	OldPen	 =	 (HPEN)SelectObject(surface, NullPen);

	//draw eadge lhx
	HBRUSH NullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(surface, NullBrush);
	Rectangle(surface, border, border , cxClient-border, cyClient-border);
	//draw barriers
	for(int i=0;i<MAX_BARRIERS;i++)
	{
		m_vecBarriers[i].Render(surface,cxClient,cyClient);
	}

	MoveToEx(surface,m_spA.x,m_spA.y,NULL);
	LineTo(surface,m_spB.x,m_spB.y);

	//A
	string Start = "A("+itos(m_spA.x)+string(",")+itos(m_spA.y)+string(")");	
	TextOut(surface, m_spA.x-20,m_spA.y-20, Start.c_str(), Start.size());
	//B
	string End = "B("+itos(m_spB.x)+string(",")+itos(m_spB.y)+string(")");	
	TextOut(surface, m_spB.x+20,m_spB.y+20, End.c_str(), End.size());






	//restore the original brush
	SelectObject(surface, OldBrush);

	//and pen
	SelectObject(surface, OldPen);
}

//-------------------------------MemoryRender ------------------------
//
//	//draws whatever path may be stored in the memory
//--------------------------------------------------------------------
void CBobsMap::MemoryRender(const int cxClient,
							const int cyClient,
							HDC surface)
{
	const int border = 20;
	
	int BlockSizeX = (cxClient - 2*border)/m_iMapWidth;
	int BlockSizeY = (cyClient - 2*border)/m_iMapHeight;
	
	HBRUSH	GreyBrush, OldBrush;
	HPEN	NullPen, OldPen,RedPen;
	
	//grab a brush to fill our cells with
	GreyBrush = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//grab a red pen 
	RedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	//grab a pen
	//NullPen = (HPEN)GetStockObject(NULL_PEN);
	
	//select them into the device conext
	OldBrush = (HBRUSH)SelectObject(surface, GreyBrush);
	//OldPen	 =	(HPEN)SelectObject(surface, NullPen);

	bool isGiveUp=false;
	int testCount=0;
	
	Coordinate cord(m_spA,m_spB);
	SPoint c=m_spA;

	SVector2D line(m_spA.x-m_spB.x,m_spA.y-m_spB.y);
	int count = (int) Vec2DLength(line)/10;
	for(int i=0;i<=count;i++)
	{
		SPoint c3=cord.GetXProjection(i*10);
		SPoint c4=cord.GetCoordinate(i*10,RandInt(-100,100));
		while(true)
		{
			int j;
			for(j=0;j<MAX_BARRIERS;j++)
			{
				bool log = m_vecBarriers[j].IsIntersect(c,c4);		
				if(log)
				{
					c4=cord.GetCoordinate(i*10,RandInt(-100,100));
					testCount++;
					break;

				}
			}
			if(testCount>100) 
			{
				isGiveUp=true;
				break;
			}

			if(j==MAX_BARRIERS) break;
		}

		
		DrawLine(surface,c3,c4);

		if(isGiveUp) break;

		OldPen = (HPEN)SelectObject(surface, RedPen);
		DrawLine(surface,c,c4);
		SelectObject(surface, OldPen);
		c=c4;
	}




	//restore the original brush
	SelectObject(surface, OldBrush);
	
	//and pen
	//SelectObject(surface, OldPen);

}

//---------------------------- TestRoute ---------------------------
//
//	given a decoded vector of directions and a map object representing
//	Bob's memory, this function moves Bob through the maze as far as 
//	he can go updating his memory as he moves.
//-------------------------------------------------------------------
double CBobsMap::TestRoute(const vector<int> &vecPath, CBobsMap &Bobs)
{
	int posX = m_iStartX;
	int posY = m_iStartY;

	for (int dir=0; dir<vecPath.size(); ++dir)
	{
		int NextDir = vecPath[dir];

		switch(vecPath[dir])
		{
		case 0: //North

			//check within bounds and that we can move
			if ( ((posY-1) < 0 ) || (map[posY-1][posX] == 1) )
			{
				break;
			}

			else
			{
				posY -= 1;
			}

			break;

		case 1: //South

			//check within bounds and that we can move
			if ( ((posY+1) >= m_iMapHeight) || (map[posY+1][posX] == 1) )
			{
				break;
			}
			
			else
			{
				posY += 1;
			}

			break;

		case 2: //East

			//check within bounds and that we can move
			if ( ((posX+1) >= m_iMapWidth ) || (map[posY][posX+1] == 1) )
			{
				break;
			}
			
			else
			{
				posX += 1;
			}

			break;

		case 3: //West

			//check within bounds and that we can move
			if ( ((posX-1) < 0 ) || (map[posY][posX-1] == 1) )
			{
				break;
			}
			
			else
			{
				posX -= 1;
			}

			break;

		}//end switch

		//mark the route in the memory array
		Bobs.memory[posY][posX] = 1;

	}//next direction

	//now we know the finish point of Bobs journey, let's assign
	//a fitness score which is proportional to his distance from
	//the exit

	int	DiffX = abs(posX - m_iEndX);
	int DiffY = abs(posY - m_iEndY);

	//we add the one to ensure we never divide by zero. Therefore
	//a solution has been found when this return value = 1
	return 1/(double)(DiffX+DiffY+1);
}

//--------------------- ResetMemory --------------------------
//
//	resets the memory map to zeros
//------------------------------------------------------------
void CBobsMap::ResetMemory()
{
	for (int y=0; y<m_iMapHeight; ++y)
	{
		for (int x=0; x<m_iMapWidth; ++x)
		{
			memory[y][x] = 0;
		}
	}
}

CBobsMap::CBobsMap()
{
	ResetMemory();

		//lhx	add Barriers
		m_vecBarriers.push_back(CBarrier(m_sp1,m_aBarriesCount[0]));
		m_vecBarriers.push_back(CBarrier(m_sp2,m_aBarriesCount[1]));
		m_vecBarriers.push_back(CBarrier(m_sp3,m_aBarriesCount[2]));
		m_vecBarriers.push_back(CBarrier(m_sp4,m_aBarriesCount[3]));
		m_vecBarriers.push_back(CBarrier(m_sp5,m_aBarriesCount[4]));
		m_vecBarriers.push_back(CBarrier(m_sp6,m_aBarriesCount[5]));
}




