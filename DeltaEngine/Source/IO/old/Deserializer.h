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
//    // abstracts stream for reading
//	class DE_API Deserializer
//	{
//    protected:
//        unsigned _streamPosition;
//        unsigned _streamSize;
//
//	public:
//        // construct with zero size.
//        Deserializer() :
//            _streamPosition{ 0 },
//            _streamSize{ 0 }
//        {
//
//        }
//        // construct with defined size.
//        explicit Deserializer(unsigned size) :
//            _streamPosition{ 0 },
//            _streamSize{ size }
//        {
//
//        }
//        // destruct
//        virtual ~Deserializer() = default;
//
//        // return size.
//        unsigned GetSize() const { return _streamSize; }
//
//        virtual void Open(const char* filename) = 0;
//
//        // read a 64-bit integer
//        virtual void Read(const char* attributeName, long long value) = 0;
//        // read a 32-bit integer
//        virtual void Read(const char* attributeName, int value) = 0;
//        // read a 64-bit unsigned integer
//        virtual void Read(const char* attributeName, unsigned long long value) = 0;
//        // read a 32-bit unsigned integer
//        virtual void Read(const char* attributeName, unsigned value) = 0;
//        // read a bool
//        virtual void Read(const char* attributeName, bool value) = 0;
//        // read a float
//        virtual void Read(const char* attributeName, float value) = 0;
//        // read a double
//        virtual void Read(const char* attributeName, double value) = 0;
//
//        // read a null-terminated string
//        virtual void Read(const char* attributeName, std::string& value) = 0;
//
//        // read a Color
//        virtual void Read(const char* attributeName, const Color& value) = 0;
//        // read a Matrix4
//        virtual void Read(const char* attributeName, const Matrix4x4& value) = 0;
//        // read a Point
//        virtual void Read(const char* attributeName, const Point& value) = 0;
//        // read a Vector2
//        virtual void Read(const char* attributeName, const Vector2& value) = 0;
//        // read a Vector3
//        virtual void Read(const char* attributeName, const Vector3& value) = 0;
//        // read a Vector4
//        virtual void Read(const char* attributeName, const Vector4& value) = 0;
//        // read a quaternion
//        virtual void Read(const char* attributeName, const Quaternion& value) = 0;
//
//        // ---------------------------------------------------------------------------------------------------------------- //
//		// ---------------------------------------------------------------------------------------------------------------- //
//		// ---------------------------------------------------------------------------------------------------------------- //
//
//		//// pure virtual functions ---------------------------------------
//		//virtual void DeserializeInt(const char* fileName, const char* objectName, const char* attributeName, int& i) = 0;
//		//virtual void DeserializeFloat(const char* fileName, const char* objectName, const char* attributeName, float& f) = 0;
//		//virtual void DeserializeString(const char* fileName, const char* objectName, const char* attributeName, std::string& str) = 0;
//	};
//
//	//// deserialization operators ------------------------------------------
//
//	//// base case of serialization is that the object Deserializes itself
//	//template<typename type>
//	//inline void StreamWrite(Deserializer& stream, type& typeInstance)
//	//{
//	//	typeInstance.Deserialize(stream);
//	//}
//
//	//// base non recursive stream operators of fundamental types
//
//	//inline void StreamWrite(Deserializer& stream, const char* filename, const char* objectName, const char* attributeName, int& i)
//	//{
//	//	stream.DeserializeInt(filename, attributeName, objectName, i);
//	//}
//
//	//inline void StreamWrite(Deserializer& stream, const char* filename, const char* objectName, const char* attributeName, float& f)
//	//{
//	//	stream.DeserializeFloat(filename, attributeName, objectName, f);
//	//}
//
//	//inline void StreamWrite(Deserializer& stream, const char* filename, const char* objectName, const char* attributeName, std::string& str)
//	//{
//	//	stream.DeserializeString(filename, attributeName, objectName, str);
//	//}
//}