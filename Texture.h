#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "CommonFunc.h"

class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile(SDL_Renderer* gRenderer,std::string path);

		//Deallocates texture
		void free();


		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor,TTF_Font* gFont,SDL_Renderer* gRenderer );


		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render(const int& x,const int& y,SDL_Renderer* gRenderer,SDL_Rect* clip = NULL );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
		SDL_Rect default_rect;
};

#endif // TEXTURE_H_
