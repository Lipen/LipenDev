#include "LTexture.hpp"
#include "main.hpp"


LTexture::LTexture(): texture(NULL), width(0), height(0) {}

LTexture::~LTexture() {
	free();
}

void LTexture::free() {
	// Free texture if it exists
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

bool LTexture::load_from_file(string path) {
	// Get rid of preexisting texture
	free();

	// The final texture
	SDL_Texture* new_texture = NULL;

	// Load image at specified path
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == NULL) {
		cout << "Bad load image(\"" << path << "\"): " << IMG_GetError() << endl;
	}
	else {
		// Color key image
		// SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0xFF, 0, 0xFF));

		// Create texture from surface pixels
		new_texture = SDL_CreateTextureFromSurface(gRenderer, loaded_surface);
		if (new_texture == NULL) {
			cout << "Bad texture create (\"" << path << "\"): " << SDL_GetError() << endl;
		}
		else {
			// Get image dimensions
			width = loaded_surface->w;
			height = loaded_surface->h;
		}

		// Get rid of old loaded surface
		SDL_FreeSurface(loaded_surface);
	}

	// Return success
	texture = new_texture;
	return texture != NULL;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	// Modulate texture rgb
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
	// Set blending function
	SDL_SetTextureBlendMode(texture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
	// Modulate texture alpha
	SDL_SetTextureAlphaMod(texture, alpha);
}

int LTexture::getWidth() {
	return width;
}

int LTexture::getHeight() {
	return height;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Set rendering space and render to screen
	SDL_Rect render_quad = { x-width/2, y-width/2, width, height };

	// Set clip rendering dimensions
	if (clip != NULL) {
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	// Render to screen
	SDL_RenderCopyEx(gRenderer, texture, clip, &render_quad, angle, center, flip);
}
