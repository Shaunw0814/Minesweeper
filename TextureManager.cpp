#include "TextureManager.h"

//"redeclare" the variable, so this file knows it exists
unordered_map<string, Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName) {
	string path = "images/";
	path += fileName + ".png";

	textures[fileName].loadFromFile(path);
}

Texture& TextureManager::GetTexture(string textureName) {
	//if the texture DOESN'T exist..
	if (textures.find(textureName) == textures.end()) { //we didn't find it
		LoadTexture(textureName);
	}

	return textures[textureName];
}


void TextureManager::Clear() {
	textures.clear();
}