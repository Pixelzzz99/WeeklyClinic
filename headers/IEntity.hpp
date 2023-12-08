#pragma once
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>

namespace columnTypes
{
    const std::string INT = "INTEGER";
    const std::string STRING = "TEXT";
    const std::string FLOAT = "REAL";
    const std::string DOUBLE = "DOUBLE";
    const std::string CHAR = "CHAR";
    const std::string BOOL = "BOOLEAN";
    const std::string LONG = "LONG";
    const std::string BYTE = "BYTE";
    const std::string TEXT = "TEXT";
    const std::string BLOB = "BLOB";
};

struct column
{
    std::string name;
    std::string type;
    bool isPrimaryKey;
};

class IEntity
{
protected:
    typedef int position;
    typedef std::string value;

public:
    virtual bool insert(std::vector<std::pair<position, value>>) = 0;
    virtual bool select(std::string) = 0;
    virtual bool update(std::string) = 0;
    virtual bool delete_(std::string) = 0;
    virtual bool drop() = 0;
};
