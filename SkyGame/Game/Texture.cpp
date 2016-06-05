#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
Texture::Texture() {

}

Texture::~Texture() {

}

void Texture::Load(const std::string& filename) {
	AssetFile* file;
	if (g_AssetManager.LoadAssetFile(filename, &file)) {
		unsigned char* imageBuffer = stbi_load_from_memory((unsigned char*)file->Data, file->Size, &m_Width, &m_Height, &m_Channels, 4);
		if (imageBuffer) {
			glGenTextures(1, &m_Handle);
			glBindTexture(GL_TEXTURE_2D, m_Handle);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(imageBuffer);
		}
		g_AssetManager.FreeAssetFile(file);
	}
}