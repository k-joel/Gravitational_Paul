#pragma once 

#include <map>
using std::map; 

template <typename ClassIDType, typename BaseClassType>
class CObjectFactory 
{
private:

	typedef BaseClassType* (*ObjectCreator)(void); 

	map<ClassIDType, ObjectCreator> m_ObjectCreators; 

	static CObjectFactory<ClassIDType, BaseClassType> sm_Instance; 

	CObjectFactory(void) {}

	CObjectFactory(const CObjectFactory&); 

	CObjectFactory& operator=(const CObjectFactory&); 

	~CObjectFactory(void) {}

	template<typename DerivedClassType>
	static BaseClassType* ConstructObject(void)
	{
		return new DerivedClassType; 
	}

public:
	static CObjectFactory<ClassIDType, BaseClassType>* GetInstance(void)
	{
		return &sm_Instance; 
	}

	template<typename DerivedClassType>
	bool RegisterClassType(ClassIDType id)
	{
		std::pair<ClassIDType, ObjectCreator> objTypeDef; 
		objTypeDef.first = id; 
		objTypeDef.second = ConstructObject<DerivedClassType>; 

		m_ObjectCreators.insert(objTypeDef); 

		return true;
	}

	bool UnregisterClassType(ClassIDType id)
	{
		map<ClassIDType, ObjectCreator>::iterator iter = m_ObjectCreators.find(id); 

		if(iter == m_ObjectCreators.end())
			return false; 

		m_ObjectCreators.erase(iter); 

		return true; 
	}

	BaseClassType* CreateObject(ClassIDType id)
	{
		map<ClassIDType, ObjectCreator>::iterator iter = m_ObjectCreators.find(id);

		if(iter == m_ObjectCreators.end())
			return NULL; 
		
		return (*iter).second(); 
	}

	void ShutdownObjectFactory(void)
	{
		m_ObjectCreators.clear(); 
	}

};

template<typename ClassIDType, typename BaseClassType> 
CObjectFactory<ClassIDType, BaseClassType>
CObjectFactory<ClassIDType, BaseClassType>::sm_Instance; 