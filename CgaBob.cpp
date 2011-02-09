#include "CgaBob.h"
long startTime=0;
long endTime=0;
long   GetMillisecondsCount()   
{   
	long   lMillisecondsCount;   
	SYSTEMTIME   sysTime;   
	::GetSystemTime(&sysTime);   
	lMillisecondsCount   =   sysTime.wMilliseconds   +   sysTime.wSecond   *   1000   +   sysTime.wMinute   *   1000   *   60   +   sysTime.wHour   *   1000   *   60   *   60;   
	return   lMillisecondsCount;   
}

//--------------------------RouletteWheelSelection-----------------
//
//	selects a member of the population by using roulette wheel 
//	selection as described in the text.
//------------------------------------------------------------------
SGenome& CgaBob::RouletteWheelSelection()
{
	double fSlice	= RandFloat() * m_dTotalFitnessScore;

	double cfTotal	= 0.0;

	int	SelectedGenome = 0;

	for (int i=0; i<m_iPopSize; ++i)
	{

		cfTotal += m_vecGenomes[i].dFitness;

		if (cfTotal > fSlice) 
		{
			SelectedGenome = i;
			break;
		}
	}

	return m_vecGenomes[SelectedGenome];
}

//----------------------------Mutate---------------------------------
//	iterates through each genome flipping the bits acording to the
//	mutation rate
//--------------------------------------------------------------------
void CgaBob::Mutate2(vector<int> &vectYCoordinates)
{
	int iGenLen=vectYCoordinates.size();
	for (int curBit=0; curBit<iGenLen; curBit++)
	{
		//do we flip this bit?
		if (RandFloat() < m_dMutationRate)
		{
			//flip the bit
			vectYCoordinates[curBit]+=RandInt(-50,50)*RandFloat();
		}
	}//next bit
}

//----------------------------Crossover--------------------------------
//	Takes 2 parent vectors, selects a midpoint and then swaps the ends
//	of each genome creating 2 new genomes which are stored in baby1 and
//	baby2.
//---------------------------------------------------------------------
void CgaBob:: Crossover2(const vector<int>	&mum,
						 const  vector<int>	&dad,
						 vector<int>	&baby1,
						 vector<int>	&baby2)
{
	//just return parents as offspring dependent on the rate
	//or if parents are the same
	if ( (RandFloat() > m_dCrossoverRate) && mum!=dad) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//determine a crossover point
	int cp = RandInt(0, m_iChromoLength - 1);

	//swap the bits
	for (int i=0; i<cp; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (int i=cp; i<mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}
}
//-----------------------------------Run----------------------------------
//
//	This is the function that starts everything. It is mainly another 
//	windows message pump using PeekMessage instead of GetMessage so we
//	can easily and dynamically make updates to the window while the GA is 
//	running. Basically, if there is no msg to be processed another Epoch
//	is performed.
//------------------------------------------------------------------------
void CgaBob::Run(HWND hwnd)
{
	startTime=GetMillisecondsCount();
	//The first thing we have to do is create a random population
	//of genomes
	CreateStartPopulation();

	m_bBusy = true;

}
//----------------------CreateStartPopulation---------------------------
//
//-----------------------------------------------------------------------
void CgaBob::CreateStartPopulation()
{
	//clear existing population
	m_vecGenomes.clear();

	for (int i=0; i<m_iPopSize; i++)
	{

		//lhx
		vector<int> yCoordinates;
		while(!m_BobsMap.GetOneValidPath(yCoordinates,m_iChromoLength))
		{
			yCoordinates.clear();
		}
		m_vecGenomes.push_back(SGenome (yCoordinates));

	}

	//reset all variables
	m_iGeneration		 = 0;
	m_iFittestGenome	 = 0;
	m_dBestFitnessScore  = 0;
	m_dTotalFitnessScore = 0;
}
//--------------------------------Epoch---------------------------------
//
//	This is the workhorse of the GA. It first updates the fitness
//	scores of the population then creates a new population of
//	genomes using the Selection, Croosover and Mutation operators
//	we have discussed
//----------------------------------------------------------------------
void CgaBob::Epoch()
{



	UpdateFitnessScores();

	//Now to create a new population
	int NewBabies = 0;

	//create some storage for the baby genomes 
	vector<SGenome> vecBabyGenomes;

	for(int i=0;i<2;i++)
	{
		vecBabyGenomes.push_back(m_vecGenomes[m_iFittestGenome]);
	}

	while (NewBabies < m_iPopSize)
	{
		//select 2 parents
		SGenome mum = RouletteWheelSelection();
		SGenome dad = RouletteWheelSelection();


		//operator - crossover
		SGenome baby1, baby2;
		Crossover2(mum.vecYCoordinates, dad.vecYCoordinates, baby1.vecYCoordinates, baby2.vecYCoordinates);

		//operator - mutate
		Mutate2(baby1.vecYCoordinates);
		Mutate2(baby2.vecYCoordinates);

		//add to new population
		vecBabyGenomes.push_back(baby1);
		vecBabyGenomes.push_back(baby2);

		NewBabies += 2;
	}

	//copy babies back into starter population
	m_vecGenomes = vecBabyGenomes;

	//increment the generation counter
	++m_iGeneration;
}

//---------------------------UpdateFitnessScores------------------------
//	updates the genomes fitness with the new fitness scores and calculates
//	the highest fitness and the fittest member of the population.
//	Also sets m_pFittestGenome to point to the fittest. If a solution
//	has been found (fitness == 1 in this example) then the run is halted
//	by setting m_bBusy to false
//-----------------------------------------------------------------------
void CgaBob::UpdateFitnessScores()
{
	m_iFittestGenome		= 0;
	m_dBestFitnessScore		= 0;
	m_dTotalFitnessScore	= 0;

	m_bBusy=true;
	CBobsMap TempMemory;

	//update the fitness scores and keep a check on fittest so far
	for (int i=0; i<m_iPopSize; ++i)
	{
		//lhx
		vector<WayPoint> vecWayPoints =TempMemory.Decode(m_vecGenomes[i].vecYCoordinates);

		double TourLength = TempMemory.TestRoute(vecWayPoints);

		m_vecGenomes[i].dFitness = TourLength;
		//keep a track of the shortest route found each generation
		if (TourLength < m_fShortestRoute)
		{
			m_fShortestRoute = TourLength;
			m_iFittestGenome = i;
		}

		//keep a track of the worst tour each generation
		if (TourLength > m_fLongestRoute)
		{
			m_fLongestRoute = TourLength;
		}

	}//next genome


	//Now we have calculated all the tour lengths we can assign
	//the fitness scores
	for (int i=0; i<m_iPopSize; ++i)
	{
		m_vecGenomes[i].dFitness = m_fLongestRoute - m_vecGenomes[i].dFitness;
		m_dTotalFitnessScore +=m_vecGenomes[i].dFitness;
	}
	m_BobsBrain = TempMemory;
}



//------------------------- Render -------------------------------
//
//	given a surface to render to this function renders the map
//	and the best path if relevant. cxClient/cyClient are the 
//	dimensions of the client window.
//----------------------------------------------------------------
void CgaBob::Render(int cxClient, int cyClient, HDC surface)
{
	//���Ƶ�ͼ
	m_BobsMap.Render(cxClient, cyClient, surface);
	//����ԭʼ·��
	if(m_bShowOrigin)
	{
		m_BobsBrain.RenderOriginRoute(cxClient, cyClient, surface);
	}
	//���ƾ���·��
	if(m_bShowFixed)
	{
		m_BobsBrain.RenderShortRoute(cxClient, cyClient, surface);
	}


	//���� ������� ��Ϣ
	string s = "Generation: " + itos(m_iGeneration);
	TextOut(surface, 5, 0, s.c_str(), s.size());

	if (!m_bBusy)
	{
		string Start = "Press Return to start a new run";

		TextOut(surface, cxClient/2 - (Start.size() * 3), cyClient - 20, Start.c_str(), Start.size());
	}

	else

	{
		string Start = "Space to stop";

		TextOut(surface, cxClient/2 - (Start.size() * 5), cyClient - 20, Start.c_str(), Start.size());
	}

	if(m_iGeneration>=m_iMaxGeneration)
	{ 
		Stop();
		endTime=GetMillisecondsCount();
		::ostringstream os;
		os<<"Cost Time = "<<endTime-startTime<<"(ms)";
		string TimeStr(os.str());
		TextOut(surface, cxClient/2 +(TimeStr.size() * 3), cyClient - 20, TimeStr.c_str(), TimeStr.size());
		RestGA();
		//add a best route genome
		m_vecBestGenomes.push_back(m_vecGenomes[m_iFittestGenome]);
		if(m_vecBestGenomes.size()>=10)
		{
			vector<SGenome>::const_iterator iter = m_vecBestGenomes.begin();
			while(iter!=m_vecBestGenomes.end())
			{
				vector<WayPoint> bestPath = m_BobsBrain.Decode(iter->vecYCoordinates);
				m_BobsBrain.RenderBestRoute(cxClient, cyClient, surface,bestPath);
				iter++;
			}
		}else
		{
			vector<WayPoint> bestPath = m_BobsBrain.Decode(m_vecGenomes[m_iFittestGenome].vecYCoordinates);
			m_BobsBrain.RenderBestRoute(cxClient, cyClient, surface,bestPath);
		}


	}

}

void	CgaBob::SetShowOption(bool showOrigin,bool showFixed)
{
	m_bShowOrigin = showOrigin;
	m_bShowFixed=showFixed;

}
//Rest ALL GA to do new Run
void	CgaBob::RestGA()
{
	m_dTotalFitnessScore=0;
	m_iGeneration=0;
	m_fLongestRoute=0;
	m_fShortestRoute=9999.0;
}
