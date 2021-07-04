////////////////////////////////////////////////////////////////////////////////
// File name:		CCodeProfiler.h
//
// Purpose:			Using a high resolution timer to time the duration of functions.
//
// Author:			Kahran Ghosalkar
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Windows.h"

#include <queue>
#include <map>
#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//	CCodeProfiler:
//
//	Purpose:	Encompases all of the profiler functionality
class CCodeProfiler
{
	////////////////////////////////////////////////////////////////////////////////
	//	FunctionProfile:
	//
	//	Purpose:	The profile for a single function
	friend class FunctionProfile;
	class FunctionProfile
	{
		friend class CCodeProfiler;

		bool m_bCurrentlyRunning;
		int m_nNumOfCalls;
		queue<LARGE_INTEGER> m_StartTimes;
		queue<LARGE_INTEGER> m_EndTimes;

		public:
		FunctionProfile()
		{
			m_bCurrentlyRunning = false;
			m_nNumOfCalls = 0;
		}

		////////////////////////////////////////////////////////////////////////////////
		//	StartFunction:
		//
		//	Purpose:	gets and stores the timestamp for the start of the function
		void StartFunction();

		////////////////////////////////////////////////////////////////////////////////
		//	EndFunction:
		//
		//	Purpose:	gets and stores the timestamp for the end of the function
		void EndFunction();
	};

	//All of the functions currently being timed
	map<string,FunctionProfile> FunctionsTimings;

public:

	CCodeProfiler(void){}
	~CCodeProfiler(void){}

	////////////////////////////////////////////////////////////////////////////////
	//	GetInstance:
	//
	//	Purpose:	gets the one and only code profiler
	static CCodeProfiler *CCodeProfiler::GetInstance()
	{
		static CCodeProfiler m_pInstance;
		return &m_pInstance;
	}

	////////////////////////////////////////////////////////////////////////////////
	//	StartFunction:
	//
	//	Purpose:	gets and stores the timestamp for the start of the specified function
	//
	//	Parameters:	string FunctionToStart:
	//				the function we are timing
	void StartFunction(string FunctionToStart)
	{
		FunctionsTimings[FunctionToStart].StartFunction();
	}

	////////////////////////////////////////////////////////////////////////////////
	//	EndFunction:
	//
	//	Purpose:	gets and stores the timestamp for the end of the specified function
	//
	//	Parameters:	string FunctionToEnd:
	//				the function we are timing
	void EndFunction(string FunctionToEnd)
	{
		FunctionsTimings[FunctionToEnd].EndFunction();
	}

	////////////////////////////////////////////////////////////////////////////////
	//	WriteFiles:
	//
	//	Purpose:	writes out all of the information collected by the code profiler
	void WriteFiles(void);
};

//Add this to the beginning and end of the function you want to profile
#define PROFILER_START	CCodeProfiler::GetInstance()->StartFunction(string(__FUNCTION__));
#define PROFILER_END 	CCodeProfiler::GetInstance()->EndFunction(string(__FUNCTION__));