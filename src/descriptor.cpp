#include <campaign/descriptor.hpp>
#include <stdexcept>
#include <sstream>

using namespace campaign;

std::string typeToString(Descriptor::Type type)
{
    switch (type)
    {
    case Descriptor::Type::Flag:
        return "Flag";

    case Descriptor::Type::Byte:
        return "Byte";

    case Descriptor::Type::Dynamic:
        return "Dynamic";

    case Descriptor::Type::Void:
        return "Void";
        
    default:
        return "?";
    }
}

std::string throwBadDescriptionTypeException(Descriptor::Type expectedType, Descriptor::Type actualType)
{
    std::stringstream stream;
    stream << "Trying to get a \"" << typeToString(expectedType) << "\" info from a \"" << typeToString(actualType) << "\" descriptor.";

    throw std::logic_error(stream.str());
}

std::string throwDynamicDescriptionSizeMismatchException(size_t expectedSize, size_t actualSize)
{
    std::stringstream stream;
    stream << "Trying to get a dynamic info of size \"" << expectedSize << "\" but it's actually of size \"" << actualSize << "\".";

    throw std::logic_error(stream.str());
}

Descriptor::Descriptor(FlagInfo flagInfo)
    : type_(Descriptor::Type::Flag), infoPtr_(std::make_shared<FlagInfo>(flagInfo))
{
}

Descriptor::Descriptor(ByteInfo byteInfo)
    : type_(Descriptor::Type::Byte), infoPtr_(std::make_shared<ByteInfo>(byteInfo))
{
}

Descriptor::Descriptor(DynamicInfo dynamicInfo) 
    : type_(Descriptor::Type::Dynamic), infoPtr_(std::make_shared<DynamicInfo>(dynamicInfo))
{
}

Descriptor::Descriptor() 
    : type_(Descriptor::Type::Void), infoPtr_()
{
}

Descriptor::Type campaign::Descriptor::getType() const
{
    return type_;
}

Descriptor::FlagInfo Descriptor::getFlagInfo() const
{
    if (type_ != Type::Flag)
    {
        throwBadDescriptionTypeException(Type::Flag, type_);
    }

    return *dynamic_cast<FlagInfo*>(infoPtr_.get());
}

Descriptor::ByteInfo Descriptor::getByteInfo() const
{
    if (type_ != Type::Byte)
    {
        throwBadDescriptionTypeException(Type::Byte, type_);
    }

    return *dynamic_cast<ByteInfo*>(infoPtr_.get());
}

Descriptor::DynamicInfo Descriptor::getDynamicInfo(size_t expectedSize) const
{
    if (type_ != Type::Dynamic)
    {
        throwBadDescriptionTypeException(Type::Dynamic, type_);
    }

    auto &desc = *dynamic_cast<DynamicInfo*>(infoPtr_.get());

    if (desc.size != expectedSize)
    {
        throwDynamicDescriptionSizeMismatchException(expectedSize, desc.size);
    }

    return desc;
};