#pragma once
#include <android\asset_manager.h>
#include <string>
struct AssetFile {
	char* Data = nullptr;
	int Size = 0;
};

#define g_AssetManager AssetManager::GetInstance()
class AssetManager {
public:
	AssetManager();
	~AssetManager();
	static AssetManager& GetInstance();
	void SetAssetManager(AAssetManager* manager);
	bool LoadAssetFile(const std::string& filename, AssetFile** outAsset);
	void FreeAssetFile(AssetFile* file);
private:
	AAssetManager* m_Assets;
};