//#include "IO/JSONDeserializer.h"
//
//namespace DeltaEngine
//{
//    void JSONDeserializer::Open(const char* filename)
//    {
//        std::ifstream inputFile(filename);
//        assert(filename);
//    }
//
//    // read a 64-bit integer
//    void JSONDeserializer::Read(const char* attributeName, long long _value)
//    {
//        rapidjson::Value::ConstMemberIterator itr = document.FindMember(attributeName);
//        if (itr != document.MemberEnd())
//        {
//            _value = itr->value.GetInt64();
//            //value = JsonValue[attributeName].GetInt64();
//        }
//    }
//
//    // read a 32-bit integer
//    void JSONDeserializer::Read(const char* attributeName, int value)
//    {
//        value = JsonValue[attributeName].GetInt();
//    }
//
//    // read a 64-bit unsigned integer
//    void JSONDeserializer::Read(const char* attributeName, unsigned long long value)
//    {
//        value = JsonValue[attributeName].GetUint64();
//    }
//
//    // read a 32-bit unsigned integer
//    void JSONDeserializer::Read(const char* attributeName, unsigned value)
//    {
//        value = JsonValue[attributeName].GetUint();
//    }
//
//    // read a bool
//    void JSONDeserializer::Read(const char* attributeName, bool value)
//    {
//        value = JsonValue[attributeName].GetBool();
//    }
//
//    // read a float
//    void JSONDeserializer::Read(const char* attributeName, float value)
//    {
//        value = JsonValue[attributeName].GetFloat();
//    }
//
//    // read a double
//    void JSONDeserializer::Read(const char* attributeName, double value)
//    {
//        value = JsonValue[attributeName].GetDouble();
//    }
//
//    // read a null-terminated string
//    void JSONDeserializer::Read(const char* attributeName, std::string& value)
//    {
//        value = JsonValue[attributeName].GetString();
//    }
//
//
//    // read a Color
//    void JSONDeserializer::Read(const char* attributeName, const Color& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//    // read a Matrix4
//    void JSONDeserializer::Read(const char* attributeName, const Matrix4x4& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//    // read a Point
//    void JSONDeserializer::Read(const char* attributeName, const Point& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//    // read a Vector2
//    void JSONDeserializer::Read(const char* attributeName, const Vector2& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//    // read a Vector3
//    void JSONDeserializer::Read(const char* attributeName, const Vector3& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//    // read a Vector4
//    void JSONDeserializer::Read(const char* attributeName, const Vector4& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//    // read a quaternion
//    void JSONDeserializer::Read(const char* attributeName, const Quaternion& value)
//    {
//        ///////////////////////////////////////////////////////////
//    }
//
//
//    // ---------------------------------------------------------------------------------------------------------------- //
//    // ---------------------------------------------------------------------------------------------------------------- //
//    // ---------------------------------------------------------------------------------------------------------------- //
//
//    //void JSONDeserializer::DeserializeInt(const char* fileName, const char* objectName, const char* attributeName, int& i)
//	//{
//	//	// temp fake
//	//	stream >> i;
//	//}
//	//
//	//void JSONDeserializer::DeserializeFloat(const char* fileName, const char* objectName, const char* attributeName, float& f)
//	//{
//	//	// temp fake
//	//	stream >> f;
//	//}
//	//
//	//void JSONDeserializer::DeserializeString(const char* fileName, const char* objectName, const char* attributeName, std::string& str)
//	//{
//	//	// deserialization
//	//	std::ifstream inputFile(fileName);
//	//	assert(fileName);
//	//
//	//	std::stringstream jsonDocumentBuffer;
//	//	std::string inputLine;
//	//
//	//	while (std::getline(inputFile, inputLine))
//	//	{
//	//		jsonDocumentBuffer << inputLine << "\n";
//	//	}
//	//
//	//	rapidjson::Document document;
//	//	document.Parse(jsonDocumentBuffer.str().c_str());
//	//
//	//	const rapidjson::Value& attributes = document[objectName];
//	//	// attributes are in an object
//	//	assert(attributes.IsArray());
//	//
//	//	for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
//	//	{
//	//		const rapidjson::Value& attribute = *itr;
//	//		assert(attribute.IsObject()); // each attribute is an object
//	//
//	//		//std::cout << attribute[attributeName].GetString() << std::endl;
//	//	}
//	//}
//}