#include "SerializedClass.h"

//std::vector<std::string> SerializedClass::Serialize(int indentNum)
//{
//	std::vector<std::string> r;
//	std::vector<SerializeFuncData*> sfdVector = GetSerializeFuncData();
//	for (auto& sfd : sfdVector)
//	{
//		std::vector<std::string> s = sfd->serializeFunc(indentNum);
//		r.insert(r.end(), s.begin(), s.end());
//	}
//	return r;
//}
//
//void SerializedClass::Deserialize(std::vector<std::string> v)
//{
//	std::vector<SerializeFuncData*> sfdVector = GetSerializeFuncData();
//	InputValue2(v, sfdVector);
//}

std::vector<SerializedClass::FieldInfo> SerializedClass::GetFields()
{
	std::vector<FieldInfo> fieldInfoVector;

	std::vector<SerializeFuncData*> sfdVector = GetSerializeFuncData();
	for (auto& sfd : sfdVector)
	{
		fieldInfoVector.push_back(sfd->getFieldFunc());
	}
	return fieldInfoVector;
}

void SerializedClass::SetField()
{

}