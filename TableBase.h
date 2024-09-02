#pragma once

namespace luban
{
class ByteBuf;
    
class TableBase
{
public:
    virtual ~TableBase() = default;
    virtual bool load(::luban::ByteBuf& buf) = 0;
    virtual const char* getOutputDataFile() const = 0;
    virtual int getTypeId() const = 0;
};
}
