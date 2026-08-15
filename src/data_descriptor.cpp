#include <campaign/data_descriptor.hpp>
#include <stdexcept>
#include <sstream>

using namespace campaign;

std::string typeToString(DataDescriptor::Type type)
{
    switch (type)
    {
    case DataDescriptor::Type::Flag:
        return "Flag";

    case DataDescriptor::Type::Byte:
        return "Byte";

    case DataDescriptor::Type::Dynamic:
        return "Dynamic";

    case DataDescriptor::Type::Void:
        return "Void";
        
    default:
        return "?";
    }
}

std::string throwBadDescriptionTypeException(DataDescriptor::Type expectedType, DataDescriptor::Type actualType)
{
    std::stringstream stream;
    stream << "Trying to get a \"" << typeToString(expectedType) << "\" description from a \"" << typeToString(actualType) << "\" description.";

    throw std::logic_error(stream.str());
}

std::string throwDynamicDescriptionSizeMismatchException(size_t expectedSize, size_t actualSize)
{
    std::stringstream stream;
    stream << "Trying to get a dynamic description of size \"" << expectedSize << "\" but it's actually of size \"" << actualSize << "\".";

    throw std::logic_error(stream.str());
}

DataDescriptor::DataDescriptor(FlagDescription description)
    : type_(DataDescriptor::Type::Flag), descriptionPtr_(std::make_shared<FlagDescription>(description))
{
}

DataDescriptor::DataDescriptor(ByteDescription description)
    : type_(DataDescriptor::Type::Byte), descriptionPtr_(std::make_shared<ByteDescription>(description))
{
}

DataDescriptor::DataDescriptor(DynamicDescription description) 
    : type_(DataDescriptor::Type::Dynamic), descriptionPtr_(std::make_shared<DynamicDescription>(description))
{
}

DataDescriptor::DataDescriptor() 
    : type_(DataDescriptor::Type::Void), descriptionPtr_()
{
}

DataDescriptor::Type campaign::DataDescriptor::getType() const
{
    return type_;
}

DataDescriptor::FlagDescription DataDescriptor::getFlagDescription() const
{
    if (type_ != Type::Flag)
    {
        throwBadDescriptionTypeException(Type::Flag, type_);
    }

    return *dynamic_cast<FlagDescription*>(descriptionPtr_.get());
}

DataDescriptor::ByteDescription DataDescriptor::getByteDescription() const
{
    if (type_ != Type::Byte)
    {
        throwBadDescriptionTypeException(Type::Byte, type_);
    }

    return *dynamic_cast<ByteDescription*>(descriptionPtr_.get());
}

DataDescriptor::DynamicDescription DataDescriptor::getDynamicDescription(size_t expectedSize) const
{
    if (type_ != Type::Dynamic)
    {
        throwBadDescriptionTypeException(Type::Dynamic, type_);
    }

    auto &desc = *dynamic_cast<DynamicDescription*>(descriptionPtr_.get());

    if (desc.size != expectedSize)
    {
        throwDynamicDescriptionSizeMismatchException(expectedSize, desc.size);
    }

    return desc;
};