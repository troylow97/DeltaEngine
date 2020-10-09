//#pragma once
//
//#include "DE_API.h"
//#include <string>
//
//#include "Core/Math/Color.h"
//#include "Core/Math/Matrix.h"
//#include "Core/Math/Point.h"
//#include "Core/Math/Quaternion.h"
//#include "Core/Math/Vector.h"
//
//namespace DeltaEngine
//{
//    class Color;
//    union Matrix4x4;
//    class Vector2;
//    class Vector3;
//    class Vector4;
//    class Quaternion;
//
//	// abstract stream for writing
//	class DE_API Serializer
//	{
//	public:
//		virtual ~Serializer() = default;
//
//        virtual void Open(const char* filename) = 0;
//
//        //// write bytes to the stream. Return number of bytes actually written
//        //virtual unsigned Write(const void* data, unsigned size) = 0;
//
//        // write a 64-bit integer
//        virtual void Write(const char* attributeName, long long value) = 0;
//        // write a 32-bit integer
//        virtual void Write(const char* attributeName, int value) = 0;
//        // write a 64-bit unsigned integer
//        virtual void Write(const char* attributeName, unsigned long long value) = 0;
//        // write a 32-bit unsigned integer
//        virtual void Write(const char* attributeName, unsigned value) = 0;
//        // write a bool
//        virtual void Write(const char* attributeName, bool value) = 0;
//        // write a float
//        virtual void Write(const char* attributeName, float value) = 0;
//        // write a double
//        virtual void Write(const char* attributeName, double value) = 0;
//
//        // write a null-terminated string
//        virtual void Write(const char* attributeName, std::string& value) = 0;
//
//        // write a Color
//        virtual void Write(const char* attributeName, const Color& value) = 0;
//        // write a Matrix4
//        virtual void Write(const char* attributeName, const Matrix4x4& value) = 0;
//        // write a Point
//        virtual void Write(const char* attributeName, const Point& value) = 0;
//        // write a Vector2
//        virtual void Write(const char* attributeName, const Vector2& value) = 0;
//        // write a Vector3
//        virtual void Write(const char* attributeName, const Vector3& value) = 0;
//        // write a Vector4
//        virtual void Write(const char* attributeName, const Vector4& value) = 0;
//        // write a quaternion
//        virtual void Write(const char* attributeName, const Quaternion& value) = 0;
//
//         // ---------------------------------------------------------------------------------------------------------------- //
//         // ---------------------------------------------------------------------------------------------------------------- //
//         // ---------------------------------------------------------------------------------------------------------------- //
//
//		// pure virtual functions ---------------------------------------
//		//virtual bool Open(const std::string& filename) = 0;
//		//virtual bool IsGood() = 0;
//		//virtual void SerializeInt(const char* fileName, const char* attributeName, int& i) = 0;
//		//virtual void SerializeFloat(const char* fileName, const char* attributeName, float& f) = 0;
//		//virtual void SerializeString(const char* fileName, const char* attributeName, std::string& str) = 0;
//	};
//
//	//// serialization operators ------------------------------------------
//	//
//	//// base case of serialization is that the object serializes itself
//	//template<typename type>
//	//inline void StreamRead(Serializer& stream, type& typeInstance)
//	//{
//	//	typeInstance.Serialize(stream);
//	//}
//	//
//	//// base non recursive stream operators of fundamental types
//	//
//	//inline void StreamRead(Serializer& stream, const char* filename, const char* attributeName, int& i)
//	//{
//	//	stream.SerializeInt(filename, attributeName, i);
//	//}
//	//
//	//inline void StreamRead(Serializer& stream, const char* filename, const char* attributeName, float& f)
//	//{
//	//	stream.SerializeFloat(filename, attributeName, f);
//	//}
//	//
//	//inline void StreamRead(Serializer& stream, const char* filename, const char* attributeName, std::string& str)
//	//{
//	//	stream.SerializeString(filename, attributeName, str);
//	//}
//}