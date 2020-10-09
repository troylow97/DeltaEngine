//#include "IO/Serializer.h"
//#include "IO/Deserializer.h"
//
//namespace DeltaEngine
//{
//	// a common root class for objects that implements both serializer and serializer
//	class DE_API AbstractFile : public Serializer, public Deserializer
//	{
//    public:
//        AbstractFile() {}
//        ~AbstractFile() {}
//
//        virtual void ReadFile(const char* fileName) = 0;
//        virtual void ParseFile() = 0;
//        virtual void addArray(const char* objectName) = 0;
//        virtual std::string ToString() = 0;
//        virtual std::string ToString(std::string& indendation) = 0;
//        virtual void Reload(const char* fileName) = 0;
//
//        virtual int getInt(const char* objectName, const char* memberName) = 0;
//        virtual long long getInt64(const char* objectName, const char* memberName) = 0;
//        virtual unsigned getUInt(const char* objectName, const char* memberName) = 0;
//        virtual unsigned long long getUInt64(const char* objectName, const char* memberName) = 0;
//        virtual bool getBool(const char* objectName, const char* memberName) = 0;
//        virtual double getDouble(const char* objectName, const char* memberName) = 0;
//        virtual float getFloat(const char* objectName, const char* memberName) = 0;
//        virtual const char* getString(const char* objectName, const char* memberName) = 0;
//
//        // method chaining ---------------------------------------------------------
//        virtual AbstractFile& OpenObject(const char* objectName) = 0;
//        virtual AbstractFile& OpenMember(const char* memberName) = 0;
//        virtual AbstractFile& AddArray(const char* objectName) = 0;
//        virtual AbstractFile& AddObject(const char* objectName) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, int attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, long long attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, unsigned attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, unsigned long long attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, bool attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, double attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, float attributeValue) = 0;
//        virtual AbstractFile& AddMember(const char* attributeName, const char* attributeValue) = 0;
//        virtual AbstractFile& CloseObject() = 0;
//	};
//}