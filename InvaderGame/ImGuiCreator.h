#pragma once

#include "EngineFramework.h"

#include "SerializedClass.h"

class ImGuiCreator
{
public:
	static void CreateAssetGui(const AssetManager::AssetFolder& assetFolder, const std::function<void(const std::string& assetName, AssetManager::AssetFile* assetFile)>& selectedAction);
	static bool PutPointerField(std::string& serializedData, const std::string& serializedVarName, const std::string& label, const std::string& instanceID);
	static void Create(SerializedClass* serializedObject);

private:
	static bool IsArithmetic(std::smatch& outSmatch, const std::string& value);
	static bool IsPointer(std::string& instanceID, const std::string& value);
	static bool ArithmeticField(std::string& serializedData, const std::string& indent, const std::string& label, const std::string& value);
};