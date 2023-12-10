#pragma once
#include <utility>
#include <vector>
#include <string>
#include <map>

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
    bool isForeignKey;
    std::string foreignTable;
    std::string foreignColumn;
};

class IEntity
{
protected:
    typedef int position;
    typedef std::string value;
    typedef std::string condition;

public:
    virtual bool insert(std::vector<std::pair<position, value>>) = 0;
    virtual std::vector<std::map<std::string, std::string>> select(std::vector<condition> conditions) = 0;
    virtual bool update(int id, std::vector<std::pair<std::string, value>> values) = 0;
    virtual bool delete_(int id) = 0;
    virtual bool drop() = 0;
};
