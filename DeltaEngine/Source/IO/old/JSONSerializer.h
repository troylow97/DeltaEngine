//#pragma once
//
//#include "DE_API.h"
//#include "rapidjson/document.h"
//#include "IO/Serializer.h"
//#include <fstream>
//#include <sstream>
//
//namespace DeltaEngine
//{
//	class DE_API JSONSerializer : public Serializer
//	{
//	public:
//        rapidjson::Value JsonValue;
//
//        virtual void Open(const char* filename) override;
//
//        //// write bytes to the stream. Return number of bytes actually written
//        //virtual unsigned Write(const void* data, unsigned size) override;
//
//        // write a 64-bit integer
//        virtual void Write(const char* attributeName, long long value) override;
//        // write a 32-bit integer
//        virtual void Write(const char* attributeName, int value) override;
//        // write a 64-bit unsigned integer
//        virtual void Write(const char* attributeName, unsigned long long value) override;
//        // write a 32-bit unsigned integer
//        virtual void Write(const char* attributeName, unsigned value) override;
//        // write a bool
//        virtual void Write(const char* attributeName, bool value) override;
//        // write a float
//        virtual void Write(const char* attributeName, float value) override;
//        // write a double
//        virtual void Write(const char* attributeName, double value) override;
//
//        // write a null-terminated string
//        virtual void Write(const char* attributeName, std::string& value) override;
//
//        // write a Color
//        virtual void Write(const char* attributeName, const Color& value) override;
//        // write a Matrix4
//        virtual void Write(const char* attributeName, const Matrix4x4& value) override;
//        // write a Point
//        virtual void Write(const char* attributeName, const Point& value) override;
//        // write a Vector2
//        virtual void Write(const char* attributeName, const Vector2& value) override;
//        // write a Vector3
//        virtual void Write(const char* attributeName, const Vector3& value) override;
//        // write a Vector4
//        virtual void Write(const char* attributeName, const Vector4& value) override;
//        // write a quaternion
//        virtual void Write(const char* attributeName, const Quaternion& value) override;
//
//		// ---------------------------------------------------------------------------------------------------------------- //
//		// ---------------------------------------------------------------------------------------------------------------- //
//		// ---------------------------------------------------------------------------------------------------------------- //
//
//	    //std::ifstream stream;
//		//
//		////virtual bool Open(const std::string& filename) override;
//		////virtual bool IsGood() override;
//		//// serializing ------------------------------------------------------------------------------------------
//		//virtual void SerializeInt(const char* fileName, const char* attributeName, int& i) override;
//		//virtual void SerializeFloat(const char* fileName, const char* attributeName, float& f) override;
//		//virtual void SerializeString(const char* fileName, const char* attributeName, std::string& str) override;
//	};
//}