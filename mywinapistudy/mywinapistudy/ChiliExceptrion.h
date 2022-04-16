#pragma once
#include <exception>
#include <string>
#include <sstream>
class ChiliExceptrion :
    public std::exception
{
public:
    ChiliExceptrion(int line, const char* file) noexcept;
    const char* what() const noexcept override;
    virtual const char* GetType() const noexcept;
    int Getline() const noexcept;
    const std::string& GetFile() const noexcept;
    std::string GetOriginString() const noexcept;
private:
    int line;
    std::string file;
protected:
    mutable std::string whatBuffer;
};

