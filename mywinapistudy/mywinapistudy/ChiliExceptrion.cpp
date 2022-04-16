#include "ChiliExceptrion.h"

ChiliExceptrion::ChiliExceptrion(int line, const char* file) noexcept :line(line), file(file)
{
}
const char* ChiliExceptrion::what()const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
	
}
const char* ChiliExceptrion::GetType()const  noexcept
{
	return "chiliException class";
}
int ChiliExceptrion::Getline()const  noexcept
{
	return line;
}
const std::string& ChiliExceptrion::GetFile()const  noexcept
{
	return file;
}
std::string ChiliExceptrion::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[LINE]" << line << std::endl << "[FILE]" << file ;
	return oss.str();
}