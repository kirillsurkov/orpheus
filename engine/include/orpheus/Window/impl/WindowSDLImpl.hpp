#pragma once

#include "orpheus/Window/WindowSDL.hpp"
#include "orpheus/Event/EventQuit.hpp"
#include "orpheus/Event/EventKeyboard.hpp"
#include "orpheus/Event/EventMouse.hpp"

#include <SDL2/SDL.h>

namespace Orpheus::Window {
    class SDL::Impl {
    private:
        SDL_Window* m_window;
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

        Event::EventKeyboard::Key s2eKey(SDL_Keysym sym) {
            switch (sym.sym) {
                case SDLK_ESCAPE: return Event::EventKeyboard::Key::ESC;
                case SDLK_F1:     return Event::EventKeyboard::Key::F1;
                case SDLK_F2:     return Event::EventKeyboard::Key::F2;
                case SDLK_F3:     return Event::EventKeyboard::Key::F3;
                case SDLK_F4:     return Event::EventKeyboard::Key::F4;
                case SDLK_F5:     return Event::EventKeyboard::Key::F5;
                case SDLK_F6:     return Event::EventKeyboard::Key::F6;
                case SDLK_F7:     return Event::EventKeyboard::Key::F7;
                case SDLK_F8:     return Event::EventKeyboard::Key::F8;
                case SDLK_F9:     return Event::EventKeyboard::Key::F9;
                case SDLK_F10:    return Event::EventKeyboard::Key::F10;
                case SDLK_F11:    return Event::EventKeyboard::Key::F11;
                case SDLK_F12:    return Event::EventKeyboard::Key::F12;
                case SDLK_1:      return Event::EventKeyboard::Key::N1;
                case SDLK_2:      return Event::EventKeyboard::Key::N2;
                case SDLK_3:      return Event::EventKeyboard::Key::N3;
                case SDLK_4:      return Event::EventKeyboard::Key::N4;
                case SDLK_5:      return Event::EventKeyboard::Key::N5;
                case SDLK_6:      return Event::EventKeyboard::Key::N6;
                case SDLK_7:      return Event::EventKeyboard::Key::N7;
                case SDLK_8:      return Event::EventKeyboard::Key::N8;
                case SDLK_9:      return Event::EventKeyboard::Key::N9;
                case SDLK_0:      return Event::EventKeyboard::Key::N0;
                case SDLK_q:      return Event::EventKeyboard::Key::Q;
                case SDLK_w:      return Event::EventKeyboard::Key::W;
                case SDLK_e:      return Event::EventKeyboard::Key::E;
                case SDLK_r:      return Event::EventKeyboard::Key::R;
                case SDLK_t:      return Event::EventKeyboard::Key::T;
                case SDLK_y:      return Event::EventKeyboard::Key::Y;
                case SDLK_u:      return Event::EventKeyboard::Key::U;
                case SDLK_i:      return Event::EventKeyboard::Key::I;
                case SDLK_o:      return Event::EventKeyboard::Key::O;
                case SDLK_p:      return Event::EventKeyboard::Key::P;
                case SDLK_a:      return Event::EventKeyboard::Key::A;
                case SDLK_s:      return Event::EventKeyboard::Key::S;
                case SDLK_d:      return Event::EventKeyboard::Key::D;
                case SDLK_f:      return Event::EventKeyboard::Key::F;
                case SDLK_g:      return Event::EventKeyboard::Key::G;
                case SDLK_h:      return Event::EventKeyboard::Key::H;
                case SDLK_j:      return Event::EventKeyboard::Key::J;
                case SDLK_k:      return Event::EventKeyboard::Key::K;
                case SDLK_l:      return Event::EventKeyboard::Key::L;
                case SDLK_z:      return Event::EventKeyboard::Key::Z;
                case SDLK_x:      return Event::EventKeyboard::Key::X;
                case SDLK_c:      return Event::EventKeyboard::Key::C;
                case SDLK_v:      return Event::EventKeyboard::Key::V;
                case SDLK_b:      return Event::EventKeyboard::Key::B;
                case SDLK_n:      return Event::EventKeyboard::Key::N;
                case SDLK_m:      return Event::EventKeyboard::Key::M;
                case SDLK_SPACE:  return Event::EventKeyboard::Key::SPACE;
                case SDLK_RETURN: return Event::EventKeyboard::Key::ENTER;
                case SDLK_UP:     return Event::EventKeyboard::Key::UP;
                case SDLK_DOWN:   return Event::EventKeyboard::Key::DOWN;
                case SDLK_LEFT:   return Event::EventKeyboard::Key::LEFT;
                case SDLK_RIGHT:  return Event::EventKeyboard::Key::RIGHT;
                case SDLK_LCTRL:  return Event::EventKeyboard::Key::L_CTRL;
                case SDLK_LSHIFT: return Event::EventKeyboard::Key::L_SHIFT;
                case SDLK_LALT:   return Event::EventKeyboard::Key::L_ALT;
                case SDLK_RCTRL:  return Event::EventKeyboard::Key::R_CTRL;
                case SDLK_RSHIFT: return Event::EventKeyboard::Key::R_SHIFT;
                case SDLK_RALT:   return Event::EventKeyboard::Key::R_ALT;
            }
            return Event::EventKeyboard::Key::UNKNOWN;
        }

    public:
        Impl(const std::string& title, unsigned int width, unsigned int height);
        ~Impl();

        void init();
        void createContext(Render::OpenGL::ContextPtr& context);
        void swapBuffers();

        template<class T>
        void pollEvents(T&& receiver) {
            SDL_Event sdlEvent;
            while (SDL_PollEvent(&sdlEvent)) {
                switch (sdlEvent.type) {
                case SDL_QUIT: {
                    receiver(std::make_shared<Event::EventQuit>());
                    break;
                }
                case SDL_KEYDOWN: {
                    receiver(std::make_shared<Event::EventKeyboard>(s2eKey(sdlEvent.key.keysym), true));
                    break;
                }
                case SDL_KEYUP: {
                    receiver(std::make_shared<Event::EventKeyboard>(s2eKey(sdlEvent.key.keysym), false));
                    break;
                }
                }
            }
        }
    };
}
