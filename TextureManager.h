#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using namespace std;
using namespace sf;

class TextureManager
{
	//static == one and only one of these in memory , ever
	static unordered_map<string, Texture> textures;
	static void LoadTexture(string textureName);
public:
	static Texture& GetTexture(string textureName);
	static void Clear(); // Call this once, at the end of main()


};