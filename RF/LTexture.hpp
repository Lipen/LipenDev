#ifndef L_TEXTURE_HPP
#define L_TEXTURE_HPP

#include "main.hpp"


class LTexture {
	SDL_Texture* texture;
	int width, height;

 public:
	LTexture();

	~LTexture();

	void free();
	bool load_from_file(string path);
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);
	int getWidth();
	int getHeight();
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif // L_TEXTURE_HPP
