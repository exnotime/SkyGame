#include "AssetManager.h"

AssetManager::AssetManager() {

}
AssetManager::~AssetManager() {

}
AssetManager& AssetManager::GetInstance() {
	static AssetManager m_Instance;
	return m_Instance;
}

void AssetManager::SetAssetManager(AAssetManager* manager) {
	m_Assets = manager;
}

bool AssetManager::LoadAssetFile(const std::string& filename, AssetFile** outAsset) {
	AAsset* asset = AAssetManager_open(m_Assets, filename.c_str(), AASSET_MODE_BUFFER);
	AssetFile* file = new AssetFile();
	if (asset) {
		file->Size = AAsset_getLength(asset);
		file->Data = (char*)malloc(file->Size);
		//copy data to buffer so we dont hold the file open for too long
		memcpy(file->Data, AAsset_getBuffer(asset), file->Size);
	} else {
		outAsset = nullptr;
		return false;
	}
	AAsset_close(asset);
	*outAsset = file;
	return true;
}

void AssetManager::FreeAssetFile(AssetFile* file) {
	//if (file) {
	//	if (file->Data) free(file->Data);
	//	delete(file);
	//}
}