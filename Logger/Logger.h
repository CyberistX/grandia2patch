#pragma once

#include <string>
#include <iostream>
#include <fstream>

class Logger
{
public:
	~Logger();
	static Logger* Instance();
	virtual int Log(std::string debugString);
	virtual int SetLogFile(std::string logFilePath);
	virtual bool IsSet();
	virtual void Close();

private:
	static Logger* privateInstance;

	std::ofstream logFile;

	Logger();
	Logger(const Logger&) = delete;
	void operator= (const Logger&) = delete;
};

