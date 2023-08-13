#include "TextureManager.h"

#include <SDL2/SDL_image.h>

/**
 * Loading a texture from png file.
 *
 * @param fileName name of the png file
 *
 * @return texture from the png file
*/
SDL_Texture * TextureManager::LoadTexture ( const char * fileName )
{
    SDL_Surface * tempSurface = IMG_Load ( fileName );
    SDL_Texture * tex = SDL_CreateTextureFromSurface ( Game::renderer, tempSurface );
    SDL_FreeSurface ( tempSurface );

    return tex;
}

/**
 * Drawing a texture throw game render.
 *
 * @param text pointer to the texture
 * @param src coorinates on the texture
 * @param dest coordinates on the window
 *
*/
void TextureManager::Draw ( SDL_Texture * tex, SDL_Rect src, SDL_Rect dest )
{
    SDL_RenderCopy ( Game::renderer, tex, &src, &dest );
}