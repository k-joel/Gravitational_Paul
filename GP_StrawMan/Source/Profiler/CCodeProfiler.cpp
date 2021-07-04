////////////////////////////////////////////////////////////////////////////////
// File name:		CCodeProfiler.cpp
//
// Purpose:			Using a high resolution timer to time the duration of functions.
//
// Author:			Kahran Ghosalkar
//
////////////////////////////////////////////////////////////////////////////////

#include "CCodeProfiler.h"
#include <fstream>
using std::ofstream;
using std::ios_base;

////////////////////////////////////////////////////////////////////////////////
//	StartFunction:
//
//	Purpose:	gets and stores the timestamp for the start of the function
void CCodeProfiler::FunctionProfile::StartFunction()
{
	if(m_bCurrentlyRunning == false)
	{
		m_nNumOfCalls++;
		LARGE_INTEGER TimeToAdd;
		QueryPerformanceCounter(&TimeToAdd);
		m_StartTimes.push(TimeToAdd);
		m_bCurrentlyRunning = true;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	EndFunction:
//
//	Purpose:	gets and stores the timestamp for the end of the function
void CCodeProfiler::FunctionProfile::EndFunction()
{
	if(m_bCurrentlyRunning == true)
	{
		LARGE_INTEGER TimeToAdd;
		QueryPerformanceCounter(&TimeToAdd);
		m_EndTimes.push(TimeToAdd);
		m_bCurrentlyRunning = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
//	WriteFiles:
//
//	Purpose:	writes out all of the information collected by the code profiler
void CCodeProfiler::WriteFiles(void)
{
	ofstream fout;
	map<string,FunctionProfile>::iterator Iter;
	char FileName[255];

	LARGE_INTEGER StartTime;
	LARGE_INTEGER EndTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	LONGLONG elapsedcounts;
	long double elapsedtime;

	long double TotalTime=0;

	for(Iter = FunctionsTimings.begin(); Iter != FunctionsTimings.end(); Iter++)
	{
		strcpy_s(FileName,255,((*Iter).first + "_Profiler_Output.txt").c_str());

		string temp = FileName;

		for(unsigned int i =0; i < temp.size();i++)
			if(temp[i] == ':')
				temp[i] = '_';

		fout.open(temp.c_str(),ios_base::out);

		fout << "======================================================\n";
		fout << (*Iter).first.c_str();
		fout << "\n======================================================\n";
		ofstream::pos_type p = fout.tellp();

		while( (*Iter).second.m_nNumOfCalls
			&& !((*Iter).second.m_StartTimes.empty())
			&& !((*Iter).second.m_EndTimes.empty()))
		{
			StartTime = (*Iter).second.m_StartTimes.front();
			(*Iter).second.m_StartTimes.pop();
			EndTime = (*Iter).second.m_EndTimes.front();
			(*Iter).second.m_EndTimes.pop();

			elapsedcounts = EndTime.QuadPart - StartTime.QuadPart;
			elapsedtime = (elapsedcounts/(long double)Frequency.QuadPart)*1000;
			fout << elapsedtime << "ms";
			TotalTime += elapsedtime;
			fout << "\n";
		}

		//Totals
		fout.seekp(p);
		fout << "======================================================\n";
		fout << "Date: " << __DATE__ << '\n';
		fout << "Total Time: " << TotalTime << "ms\n";
		fout << "Average Time: " << TotalTime/(*Iter).second.m_nNumOfCalls << "ms\n";
		fout << "======================================================\n";

		TotalTime=0;
		fout.close();
	}
}