#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture {
    private:
        SDL_Texture* _texture;
        int _width;
        int _height;

    public:
        Texture() {
            _texture = NULL;
            _width = 0;
            _height = 0;
        }

        ~Texture() {
            free();
        }

        bool loadFromFile(SDL_Renderer* renderer, std::string path) {
            free();

            SDL_Texture* newTexture = NULL;
            SDL_Surface* loadedSurface = IMG_Load(path.c_str());
            if(loadedSurface == NULL) {
                std::cout << "Unable to load image " << path.c_str() << " SDL_image Error: " << IMG_GetError() << std::endl;
            } else {
                SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
                newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

                if(newTexture == NULL) {
                    std::cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << std::endl;
                } else {
                    _width = loadedSurface->w;
                    _height = loadedSurface->h;
                }

                SDL_FreeSurface(loadedSurface);
            }

            _texture = newTexture;
            return _texture != NULL;
        }

        void free() {
            if(_texture != NULL) {
                SDL_DestroyTexture(_texture);
                _width = 0;
                _height = 0;
            }
        }

        void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip=NULL) {
            SDL_Rect renderQuad = { x, y, _width, _height };

            if(clip != NULL) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

            SDL_RenderCopy(renderer, _texture, clip, &renderQuad);
        }

        int getWidth() { return _width; }
        int getHeight() { return _height; }
};

#endif