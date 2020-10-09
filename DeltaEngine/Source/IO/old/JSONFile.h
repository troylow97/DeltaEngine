//#pragma once
//
//#include "IO/AbstractFile.h"
//#include "IO/JSONSerializer.h"
//#include "IO/JSONDeserializer.h"
//#include "rapidjson/prettywriter.h"
//#include "rapidjson/stream.h"
//
//namespace DeltaEngine
//{
//	class DE_API JSONFile : public AbstractFile
//	{
//    private:
//        rapidjson::Value* _object;
//        rapidjson::Value _member;
//
//        rapidjson::Document document;
//        std::string stringFromStream;
//        std::ifstream in;
//        rapidjson::StringBuffer jsonDocumentBuffer;
//    public:
//        JSONFile() {};
//        ~JSONFile() {};
//
//        virtual void ReadFile(const char* fileName) override;
//        virtual void ParseFile() override;
//        virtual void addArray(const char* objectName) override;
//        virtual std::string ToString() override;
//        virtual std::string ToString(std::string& indendation) override;
//        virtual void Reload(const char* fileName) override;
//
//        virtual int getInt(const char* objectName, const char* memberName) override;
//        virtual long long getInt64(const char* objectName, const char* memberName) override;
//        virtual unsigned getUInt(const char* objectName, const char* memberName) override;
//        virtual unsigned long long getUInt64(const char* objectName, const char* memberName) override;
//        virtual bool getBool(const char* objectName, const char* memberName) override;
//        virtual double getDouble(const char* objectName, const char* memberName) override;
//        virtual float getFloat(const char* objectName, const char* memberName) override;
//        virtual const char* getString(const char* objectName, const char* memberName) override;
//
//        // method chaining ---------------------------------------------------------
//        virtual JSONFile& OpenObject(const char* objectName) override;
//        virtual JSONFile& OpenMember(const char* memberName) override;
//        virtual JSONFile& AddArray(const char* objectName) override;
//        virtual JSONFile& AddObject(const char* objectName) override;
//        virtual JSONFile& AddMember(const char* attributeName, int attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, long long attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, unsigned attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, unsigned long long attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, bool attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, double attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, float attributeValue) override;
//        virtual JSONFile& AddMember(const char* attributeName, const char* attributeValue) override;
//        virtual JSONFile& CloseObject() override;
//	};
//}