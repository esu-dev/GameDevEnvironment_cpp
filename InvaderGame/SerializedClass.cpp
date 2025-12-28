#include "SerializedClass.h"

void SerializedClass::InputValue3(const std::vector<std::string>& instanceDataVector, const std::vector<std::shared_ptr<SerializeFuncData>>& functionVector)
{
	bool isPacking = false;
	std::vector<InstanceData*> subInstanceDataVector;
	for (std::string instanceData : instanceDataVector)
	{
		std::smatch m;

		// うしろに持って行った方が良いのでは？
		if (isPacking)
		{
			// リストの要素
			if (std::regex_match(instanceData, m, std::regex(R"(-\s(.+))")))
			{
				std::string value = m[1].str();

				// instanceIDをもつかどうか
				if (std::regex_match(value, m, std::regex(R"(\(instanceID\)(.+))")))
				{
					std::string instanceID = m[1].str();
					subInstanceDataVector.back()->memberVector.push_back(instanceID);
				}
				// クラス，構造体
				else if (std::regex_match(value, m, std::regex(R"(-\s\w+:)")))
				{
					subInstanceDataVector.back()->memberVector.push_back(value);
				}
				// 値
				else
				{
					subInstanceDataVector.back()->memberVector.push_back(value);
				}
			}
			// 値、クラス、構造体
			else if (std::regex_match(instanceData, m, std::regex(R"(\s{2}(\s*\w+:\s*.*))")))
			{
				// リストの格納
				subInstanceDataVector.back()->memberVector.push_back(m[1].str());
			}
			// リストなど
			else if (std::regex_match(instanceData, m, std::regex(R"(\s{2}(.+))")))
			{
				subInstanceDataVector.back()->memberVector.push_back(m[1].str());
			}
			else
			{
				//Debug::Log("まとめることができません。(%s)[SerializedClass::InputValue3()]", instanceData.c_str());
			}
		}


		// 空白がないなら
		std::regex re(R"(^([^\s]+):(\s*)(.*))");
		if (std::regex_search(instanceData, m, re))
		{
			subInstanceDataVector.push_back(new InstanceData());

			std::smatch smatch;


			// 変数名の代入
			std::string label = m[1].str();
			if (std::regex_match(label, smatch, std::regex(R"((\(\w+\))?([^\s]+))")))
			{
				subInstanceDataVector.back()->variableName = smatch[2].str();
			}


			// クラス, 構造体
			if (m[3].str() == "")
			{
				isPacking = true;
			}
			// vector
			else if (std::regex_match(instanceData, smatch, std::regex(R"(\(vector\)\w+:\s(\d+))")))
			{
				// 要素があるならまとめる
				if (std::stoi(smatch[1].str()) > 0)
				{
					isPacking = true;
				}
				else
				{
					isPacking = false;
				}

				subInstanceDataVector.back()->memberVector.push_back(smatch[1].str());
			}
			// 値, ポインタ
			else
			{
				isPacking = false;

				// ポインタかどうか確認
				std::smatch match;
				std::string matchString = m[3].str();
				if (std::regex_match(matchString, match, std::regex(R"(\(\w+\)(.+))")))
				{
					std::string instanceID = match[1].str();
					subInstanceDataVector.back()->memberVector.push_back(instanceID);
				}
				else
				{
					subInstanceDataVector.back()->memberVector.push_back(matchString);
				}
			}
		}
	}


	// データを元に値を代入
	for (int i = 0; i < subInstanceDataVector.size(); i++)
	{
		// 同じ変数名を検索
		//std_extension::Select(functionVector, [](SerializeFuncData* x) -> FieldInfo { return x->getFieldFunc(); });
		for (int j = 0; j < functionVector.size(); j++)
		{
			if (functionVector[j]->getFieldFunc().name == subInstanceDataVector[i]->variableName)
			{
				functionVector[j]->deserializeFunc(subInstanceDataVector[i]);
				break;
			}
		}


		//functionVector[i]->deserializeFunc(subInstanceDataVector[i]);
	}
}