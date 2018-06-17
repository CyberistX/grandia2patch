#include "stdafx.h"
#include "Logger.h"
#include <exception>

Logger* Logger::privateInstance = NULL;

Logger::Logger()
{

}


Logger::~Logger()
{
	
}

Logger* Logger::Instance()
{
	if (!privateInstance )
		privateInstance = new Logger();
	return privateInstance;
}

int Logger::Log(std::string debugString)
{
	if ( this -> logFile )
		this -> logFile << debugString << "\n";
	else throw std::exception ( "Log file is not set" );
	return 0;
}


int Logger::SetLogFile(std::string logFilePath)
{
	this -> logFile.open(logFilePath);
	if ( !this -> logFile.is_open() )
		throw std::exception("Couldn't open log file");
	this->Log ("Starting log");
	return 0;
}

bool Logger::IsSet()
{
	if (this->logFile)
		return true;
	return false;
}

void Logger::Close()
{
	this->logFile.close();
}
