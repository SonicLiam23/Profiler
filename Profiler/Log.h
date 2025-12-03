#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

//#if defined _DEBUG


#define LOGTEXT(x) Log::LogText(__FILE__, __LINE__, x)
#define LOGTEXTM(x) Log::LogTextMinimal(x)
#define LOGTEXTC(x) Log::LogTextConsole(x)


//#else

//#define LOGTEXT(x)
//#define LOGTEXTM(x)
//#define LOGTEXTC(x)

//#endif


struct Log
{
	template <typename T>
	static void LogText(const char* file, int line, T text);


	template <typename T>
	static void LogTextMinimal(const T text);

	
	template <typename T>
	static void LogTextConsole(const T text);
	

	static void EndLog()
	{	
		file.close();
	}

private:
	static std::ofstream file;
};

template<typename T>
inline void Log::LogText(const char* file, int line, T text)
{
	std::stringstream t;
	try
	{
		
		t << file << " On line " << line << ": " << text << std::endl;
		LogTextMinimal(t.str());
	}
	catch(...) {}
}
template <typename T>
inline void Log::LogTextMinimal(const T text)
{
	try
	{
		if (!file.is_open())
		{
			file.open("log.txt", std::ios::app);
		}
		std::cout << text << std::endl;
		file << text << "\n";
		file.close();
	}
	catch(...) 
	{
		while (true)
		{
			std::cout << "iodhfjkidshdfs\n";
		}
	}
}


template<typename T>
inline void Log::LogTextConsole(const T text)
{
	try
	{
		std::cout << text << std::endl;
	}
	catch (...) {}
}

