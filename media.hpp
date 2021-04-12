#ifndef MEDIA_HPP
#define MEDIA_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "input.hpp"
#include "texture.hpp"

class Media {
    private:
        Media() {}
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        Texture texture_;
        bool isRunning_;
        
    public:
        ~Media() { this->clean(); }

        static Media* init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen) {
            std::cout << "Initializing media instance" << std::endl;
            
            Media* media = new Media();
            media->isRunning_ = false;
            
            int flags = 0;
            if(fullScreen) {
                flags = SDL_WINDOW_FULLSCREEN;
            }
            
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                std::cout << "SDL initialization failed. SDL_ERROR: " << SDL_GetError() << std::endl;
            } else {
                media->window_ = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
                
                if(media->window_ == nullptr) {
                    std::cout << "Failed to create window. SDL_ERROR: " << SDL_GetError() << std::endl;
                } else {
                    media->renderer_ = SDL_CreateRenderer(media->window_, -1, 0);
                    
                    if(media->renderer_ == nullptr) {
                        std::cout << "Failed to create renderer. SDL_Error: " << SDL_GetError() << std::endl;
                    } else {
                        SDL_SetRenderDrawColor(media->renderer_, 255, 255, 255, 255);
                        media->isRunning_ = true;
                    }
                }
            }

            return media;
        }
        
        void handleEvents() {
            SDL_Event event;
            while(SDL_PollEvent(&event) != 0) { 
                switch(event.type) {
                    case SDL_QUIT:
                        isRunning_ = false;
                        break;
                    default:
                        break;
                }
            }
        }

        /*SDL_Texture* loadTexture(std::string path) {
            SDL_Texture* newTexture = NULL;

            SDL_Surface* loadedSurface = IMG_Load(path.c_str());
            if(loadedSurface == NULL) {
                std::cout << "Unable to load image " << path.c_str() << " SDL_image error: " << IMG_GetError() << std::endl;
            } else {
                newTexture = SDL_CreateTextureFromSurface(renderer_, loadedSurface);
                if(newTexture == NULL) {
                    std::cout << "Unable to create texture from " << path.c_str() << " SDL Error: " << SDL_GetError() << std::endl;
                }
                SDL_FreeSurface(loadedSurface);
            }
            return newTexture;
        }*/

        void loadMedia() {
            texture_ = Texture();//this->loadTexture("boy.png");
            
            if(!texture_.loadFromFile(renderer_, "boy.png")) {
                std::cout << "Failed to load texture image!" << std::endl;
            }
        }
        
        void render() {
            SDL_RenderClear(renderer_);
            //SDL_RenderCopy(renderer_, texture_, NULL, NULL);
            texture_.render(renderer_, 0, 0);
            SDL_RenderPresent(renderer_);
        }
        
        bool running() {
            return isRunning_;
        }
        
        void clean() {
            std::cout << "Cleaning up SDL objects..." << std::endl;
            texture_.free();

            SDL_DestroyRenderer(renderer_);
            SDL_DestroyWindow(window_);
            window_ = NULL;
            renderer_ = NULL;

            IMG_Quit();
            SDL_Quit();
        }
};

#endif