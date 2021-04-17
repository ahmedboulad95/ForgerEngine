#ifndef MEDIA_HPP
#define MEDIA_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "input.hpp"
#include "texture.hpp"

const int WALKING_ANIMATION_FRAMES = 4;

class Media {
    private:
        Media() {}
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        Texture texture_;
        SDL_Rect clips_[WALKING_ANIMATION_FRAMES];
        bool isRunning_;
        int frame;
        
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

            media->frame = 0;
            
            if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                std::cout << "SDL initialization failed. SDL_ERROR: " << SDL_GetError() << std::endl;
            } else {
                media->window_ = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
                
                if(media->window_ == nullptr) {
                    std::cout << "Failed to create window. SDL_ERROR: " << SDL_GetError() << std::endl;
                } else {
                    media->renderer_ = SDL_CreateRenderer(media->window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                    
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

        void loadMedia() {
            if(!texture_.loadFromFile(renderer_, "boy.png")) {
                std::cout << "Failed to load texture image!" << std::endl;
            } else {
                clips_[0].x = 0;
                clips_[0].y = 0;
                clips_[0].w = 64;
                clips_[0].h = 64;

                clips_[1].x = 64;
                clips_[1].y = 0;
                clips_[1].w = 64;
                clips_[1].h = 64;

                clips_[2].x = 128;
                clips_[2].y = 0;
                clips_[2].w = 64;
                clips_[2].h = 64;

                clips_[3].x = 196;
                clips_[3].y = 0;
                clips_[3].w = 64;
                clips_[3].h = 64;
            }
        }
        
        void render() {
            SDL_RenderClear(renderer_);
            SDL_Rect* currentClip = &clips_[frame/4];
            texture_.render(renderer_, 0, 0, currentClip);
            SDL_RenderPresent(renderer_);
            ++frame;
            if(frame / 4 >= WALKING_ANIMATION_FRAMES)
                frame = 0;
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