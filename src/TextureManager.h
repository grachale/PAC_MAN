#ifndef PAC_MAN_TEXTUREMANAGER_H
#define PAC_MAN_TEXTUREMANAGER_H

#include "Game.h"


class TextureManager
{
public:
    static SDL_Texture * LoadTexture ( const char * fileName );
    static void Draw ( SDL_Texture * tex, SDL_Rect src, SDL_Rect dest );
};


#endif //PAC_MAN_TEXTUREMANAGER_H
