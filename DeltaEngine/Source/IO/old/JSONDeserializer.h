//#pragma once
//
//#include "DE_API.h"
//#include "IO/Deserializer.h"
//#include "rapidjson/document.h"
//#include <fstream>
//#include <sstream>
//
//namespace DeltaEngine
//{
//	class DE_API JSONDeserializer : public Deserializer
//	{
//    private:
//	public:
//        const rapidjson::Value& JsonValue;
//        rapidjson::Document document;
//
//        virtual void Open(const char* filename) override;
//
//        // read a 64-bit integer
//        virtual void Read(const char* attributeName, long long value) override;
//        // read a 32-bit integer
//        virtual void Read(const char* attributeName, int value) override;
//        // read a 64-bit unsigned integer
//        virtual void Read(const char* attributeName, unsigned long long value) override;
//        // read a 32-bit unsigned integer
//        virtual void Read(const char* attributeName, unsigned value) override;
//        // read a bool
//        virtual void Read(const char* attributeName, bool value) override;
//        // read a float
//        virtual void Read(const char* attributeName, float value) override;
//        // read a double
//        virtual void Read(const char* attributeName, double value) override;
//
//        // read a null-terminated string
//        virtual void Read(const char* attributeName, std::string& value) override;
//
//        // read a Color
//        virtual void Read(const char* attributeName, const Color& value) override;
//        // read a Matrix4
//        virtual void Read(const char* attributeName, const Matrix4x4& value) override;
//        // read a Point
//        virtual void Read(const char* attributeName, const Point& value) override;
//        // read a Vector2
//        virtual void Read(const char* attributeName, const Vector2& value) override;
//        // read a Vector3
//        virtual void Read(const char* attributeName, const Vector3& value) override;
//        // read a Vector4
//        virtual void Read(const char* attributeName, const Vector4& value) override;
//        // read a quaternion
//        virtual void Read(const char* attributeName, const Quaternion& value) override;
//
//        // ---------------------------------------------------------------------------------------------------------------- //
//		// ---------------------------------------------------------------------------------------------------------------- //
//		// ---------------------------------------------------------------------------------------------------------------- //
//
//		//std::ifstream stream;
//		//
//		//virtual void DeserializeInt(const char* fileName, const char* objectName, const char* attributeName, int& i) override;
//		//virtual void DeserializeFloat(const char* fileName, const char* objectName, const char* attributeName, float& f) override;
//		//virtual void DeserializeString(const char* fileName, const char* objectName, const char* attributeName, std::string& str) override;
//	};
//}