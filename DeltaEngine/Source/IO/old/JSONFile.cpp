//#include "IO/JSONFile.h"
//
//namespace DeltaEngine
//{
//    void JSONFile::ReadFile(const char* fileName)
//    {
//        in.open(fileName, std::ifstream::in);
//
//        std::string inputLine;
//        while (std::getline(in, inputLine))
//        {
//            stringFromStream.append(inputLine + "\n");
//        }
//        in.close();
//
//        // parsing
//        ParseFile();
//    }
//
//    void JSONFile::ParseFile()
//    {
//        document.Parse(stringFromStream.c_str());
//    }
//
//    void JSONFile::addArray(const char* objectName)
//    {
//        // adding new attribute and value
//        rapidjson::Value array(rapidjson::kArrayType);
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(objectName, document.GetAllocator());
//
//        document.AddMember(attribute, array, document.GetAllocator());
//    }
//
//    std::string JSONFile::ToString()
//    {
//        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(jsonDocumentBuffer);
//
//        document.Accept(writer);
//        return jsonDocumentBuffer.GetString();
//    }
//
//    std::string JSONFile::ToString(std::string& indendation)
//    {
//        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(jsonDocumentBuffer);
//
//        writer.SetIndent(!indendation.empty() ? indendation.front() : '\0', indendation.length());
//
//        document.Accept(writer);
//        return jsonDocumentBuffer.GetString();
//    }
//
//    void JSONFile::Reload(const char* fileName)
//    {
//        // converting to string
//        std::string _indendation("  ");
//        ToString(_indendation);
//
//        FILE* file;
//        errno_t err = fopen_s(&file, fileName, "wb");
//
//        if (file)
//        {
//            fputs(jsonDocumentBuffer.GetString(), file);
//            fclose(file);
//        }
//    }
//
//    int JSONFile::getInt(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetInt();
//        }
//    }
//
//    long long JSONFile::getInt64(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetInt64();
//        }
//    }
//
//    unsigned JSONFile::getUInt(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetUint();
//        }
//    }
//
//    unsigned long long JSONFile::getUInt64(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetUint64();
//        }
//    }
//
//    bool JSONFile::getBool(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetBool();
//        }
//    }
//
//    double JSONFile::getDouble(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetDouble();
//        }
//    }
//
//    float JSONFile::getFloat(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetFloat();
//        }
//    }
//
//    const char* JSONFile::getString(const char* objectName, const char* memberName)
//    {
//        const rapidjson::Value& attributes = document[objectName];
//        assert(attributes.IsArray());
//
//        for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//        {
//            const rapidjson::Value& attribute = *itr;
//            assert(attribute.IsObject()); // each attribute is an object
//
//            return attribute[memberName].GetString();
//        }
//    }
//
//    // method chaining ---------------------------------------------------------
//    JSONFile& JSONFile::OpenObject(const char* objectName)
//    {
//        _member = rapidjson::Type::kObjectType;
//
//        if (!(document.HasMember(objectName)))
//        {
//            AddObject(objectName);
//        }
//        // the object to have member added to it
//        _object = &document[objectName];
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::OpenMember(const char* objectName)
//    {
//        _member = rapidjson::Type::kStringType;
//
//        if (!(document.HasMember(objectName)))
//        {
//            AddArray(objectName);
//        }
//        // the object to have member added to it
//        _object = &document[objectName];
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddArray(const char* objectName)
//    {
//        // adding new attribute and value
//        rapidjson::Value array(rapidjson::kArrayType);
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(objectName, document.GetAllocator());
//
//        document.AddMember(attribute, array, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddObject(const char* objectName)
//    {
//        // adding new attribute and value
//        rapidjson::Value object(rapidjson::kObjectType);
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(objectName, document.GetAllocator());
//
//        document.AddMember(attribute, object, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, int attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kNumberType);
//        value.SetInt(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, long long attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kNumberType);
//        value.SetInt64(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, unsigned attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kNumberType);
//        value.SetUint(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, unsigned long long attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kNumberType);
//        value.SetUint64(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, bool attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kTrueType);
//        value.SetBool(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, double attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kNumberType);
//        value.SetDouble(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, float attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kNumberType);
//        value.SetFloat(attributeValue);
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::AddMember(const char* attributeName, const char* attributeValue)
//    {
//        // adding new attribute and value
//        rapidjson::Value attribute(rapidjson::kStringType);
//        attribute.SetString(attributeName, document.GetAllocator());
//        rapidjson::Value value(rapidjson::kStringType);
//        value.SetString(attributeValue, document.GetAllocator());
//
//        _member.AddMember(attribute, value, document.GetAllocator());
//
//        return *this;
//    }
//
//    JSONFile& JSONFile::CloseObject()
//    {
//        _object->PushBack(_member, document.GetAllocator());
//        _member = rapidjson::Type::kNullType;
//
//        return *this;
//    }
//}