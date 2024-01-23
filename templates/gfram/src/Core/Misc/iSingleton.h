#pragma once

template<class T>
class iSingleton
{
public:

	static inline void create( void )
	{
		m_instance = new T();
	}

	static inline void destroy( void )
	{
		delete m_instance;
		m_instance = nullptr;
	}

	static T& getInstance( void ) 
	{ 
		return *m_instance; 
	}

protected:
	iSingleton( void ) { }
	virtual ~iSingleton() { }

	inline static T* m_instance;
};
