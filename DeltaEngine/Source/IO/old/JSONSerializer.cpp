//#include "IO/JSONSerializer.h"
//
//namespace DeltaEngine
//{
//    void JSONSerializer::Open(const char* filename)
//    {
//        std::ifstream inputFile(filename);
//        assert(filename);
//    }
//
//    //// write bytes to the stream. Return number of bytes actually written
//    //unsigned JSONSerializer::Write(const void* data, unsigned size) 
//    //{
//    //
//    //}
//
//    // write a 64-bit integer
//    void JSONSerializer::Write(const char* attributeName, long long value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a 32-bit integer
//    void JSONSerializer::Write(const char* attributeName, int value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a 64-bit unsigned integer
//    void JSONSerializer::Write(const char* attributeName, unsigned long long value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a 32-bit unsigned integer
//    void JSONSerializer::Write(const char* attributeName, unsigned value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a bool
//    void JSONSerializer::Write(const char* attributeName, bool value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a float
//    void JSONSerializer::Write(const char* attributeName, float value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a double
//    void JSONSerializer::Write(const char* attributeName, double value)
//    {
//        JsonValue[attributeName] = value;
//    }
//
//    // write a null-terminated string
//    void JSONSerializer::Write(const char* attributeName, std::string& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//
//    // write a Color
//    void JSONSerializer::Write(const char* attributeName, const Color& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//    // write a Matrix4
//    void JSONSerializer::Write(const char* attributeName, const Matrix4x4& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//    // write a Point
//    void JSONSerializer::Write(const char* attributeName, const Point& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//    // write a Vector2
//    void JSONSerializer::Write(const char* attributeName, const Vector2& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//    // write a Vector3
//    void JSONSerializer::Write(const char* attributeName, const Vector3& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//    // write a Vector4
//    void JSONSerializer::Write(const char* attributeName, const Vector4& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//    // write a quaternion
//    void JSONSerializer::Write(const char* attributeName, const Quaternion& value)
//    {
//        //JsonValue[attributeName] = value;
//    }
//
//
//    // ---------------------------------------------------------------------------------------------------------------- //
//    // ---------------------------------------------------------------------------------------------------------------- //
//    // ---------------------------------------------------------------------------------------------------------------- //
//
//    //bool JSONSerializer::Open(const std::string& file)
//	//{
//	//	stream.open(file.c_str());
//	//	return stream.is_open();
//	//}
//	//
//	//bool JSONSerializer::IsGood()
//	//{
//	//	return stream.good();
//	//}
//	//
//	//void JSONSerializer::SerializeInt(const char* fileName, const char* attributeName, int& i)
//	//{
//	//	// temp fake
//	//	stream >> i;
//	//}
//	//
//	//void JSONSerializer::SerializeFloat(const char* fileName, const char* attributeName, float& f)
//	//{
//	//	// temp fake
//	//	stream >> f;
//	//}
//	//
//	//void JSONSerializer::SerializeString(const char* fileName, const char* attributeName, std::string& str)
//	//{
//	//	// temp fake
//	//	stream >> str;
//	//}
//}