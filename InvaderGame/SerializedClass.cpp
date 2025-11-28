#include "SerializedClass.h"

void SerializedClass::InputValue3(const std::vector<std::string>& instanceDataVector, const std::vector<SerializeFuncData*>& functionVector)
{
	bool isPacking = false;
	std::vector<InstanceData*> subInstanceDataVector;
	for (std::string instanceData : instanceDataVector)
	{
		std::smatch m;

		if (isPacking)
		{
			// リスト
			if (std::regex_match(instanceData, m, std::regex(R"(-\s(.+))")))
			{
				subInstanceDataVector.back()->isVector = true;

				std::string value = m[1].str();

				// instanceIDをもつかどうか
				if (std::regex_match(value, m, std::regex(R"(\(instanceID\)(.+))")))
				{
					std::string instanceID = m[1].str();
					subInstanceDataVector.back()->hasInstanceID = true;
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
			// 値
			else if (std::regex_match(instanceData, m, std::regex(R"(\s{2}(\s*\w+:\s*.*))")))
			{
				// リストの格納
				subInstanceDataVector.back()->memberVector.push_back(m[1].str());
			}
			// クラス、構造体はスルー
		}


		// 空白があるかないかの判別
		std::regex re(R"(^([^\s]+):(\s*)(.*))");
		if (std::regex_search(instanceData, m, re))
		{
			subInstanceDataVector.push_back(new InstanceData());

			std::smatch smatch;

			// クラス, 構造体
			if (m[3].str() == "")
			{
				isPacking = true;
			}
			// vector
			else if (std::regex_match(instanceData, smatch, std::regex(R"(\(vector\)\w+:\s(\d+))")))
			{
				isPacking = true;

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
					subInstanceDataVector.back()->hasInstanceID = true;
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
		functionVector[i]->deserializeFunc(subInstanceDataVector[i]);
	}
}