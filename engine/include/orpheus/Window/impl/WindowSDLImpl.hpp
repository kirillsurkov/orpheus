#pragma once

#include "orpheus/Window/WindowSDL.hpp"
#include "orpheus/Command/Engine/CommandQuit.hpp"
#include "orpheus/Command/Engine/CommandKeyboard.hpp"
#include "orpheus/Command/Engine/CommandMouse.hpp"

#include <SDL2/SDL.h>

namespace Orpheus::Window {
    class SDL::Impl {
    private:
        SDL_Window* m_window;
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;

        Input::Key s2eKey(SDL_Keysym sym) {
            switch (sym.sym) {
                case SDLK_ESCAPE: return Input::Key::ESC;
                case SDLK_F1:     return Input::Key::F1;
                case SDLK_F2:     return Input::Key::F2;
                case SDLK_F3:     return Input::Key::F3;
                case SDLK_F4:     return Input::Key::F4;
                case SDLK_F5:     return Input::Key::F5;
                case SDLK_F6:     return Input::Key::F6;
                case SDLK_F7:     return Input::Key::F7;
                case SDLK_F8:     return Input::Key::F8;
                case SDLK_F9:     return Input::Key::F9;
                case SDLK_F10:    return Input::Key::F10;
                case SDLK_F11:    return Input::Key::F11;
                case SDLK_F12:    return Input::Key::F12;
                case SDLK_1:      return Input::Key::N1;
                case SDLK_2:      return Input::Key::N2;
                case SDLK_3:      return Input::Key::N3;
                case SDLK_4:      return Input::Key::N4;
                case SDLK_5:      return Input::Key::N5;
                case SDLK_6:      return Input::Key::N6;
                case SDLK_7:      return Input::Key::N7;
                case SDLK_8:      return Input::Key::N8;
                case SDLK_9:      return Input::Key::N9;
                case SDLK_0:      return Input::Key::N0;
                case SDLK_KP_1:   return Input::Key::NP1;
                case SDLK_KP_2:   return Input::Key::NP2;
                case SDLK_KP_3:   return Input::Key::NP3;
                case SDLK_KP_4:   return Input::Key::NP4;
                case SDLK_KP_5:   return Input::Key::NP5;
                case SDLK_KP_6:   return Input::Key::NP6;
                case SDLK_KP_7:   return Input::Key::NP7;
                case SDLK_KP_8:   return Input::Key::NP8;
                case SDLK_KP_9:   return Input::Key::NP9;
                case SDLK_KP_0:   return Input::Key::NP0;
                case SDLK_q:      return Input::Key::Q;
                case SDLK_w:      return Input::Key::W;
                case SDLK_e:      return Input::Key::E;
                case SDLK_r:      return Input::Key::R;
                case SDLK_t:      return Input::Key::T;
                case SDLK_y:      return Input::Key::Y;
                case SDLK_u:      return Input::Key::U;
                case SDLK_i:      return Input::Key::I;
                case SDLK_o:      return Input::Key::O;
                case SDLK_p:      return Input::Key::P;
                case SDLK_a:      return Input::Key::A;
                case SDLK_s:      return Input::Key::S;
                case SDLK_d:      return Input::Key::D;
                case SDLK_f:      return Input::Key::F;
                case SDLK_g:      return Input::Key::G;
                case SDLK_h:      return Input::Key::H;
                case SDLK_j:      return Input::Key::J;
                case SDLK_k:      return Input::Key::K;
                case SDLK_l:      return Input::Key::L;
                case SDLK_z:      return Input::Key::Z;
                case SDLK_x:      return Input::Key::X;
                case SDLK_c:      return Input::Key::C;
                case SDLK_v:      return Input::Key::V;
                case SDLK_b:      return Input::Key::B;
                case SDLK_n:      return Input::Key::N;
                case SDLK_m:      return Input::Key::M;
                case SDLK_SPACE:  return Input::Key::SPACE;
                case SDLK_RETURN: return Input::Key::ENTER;
                case SDLK_UP:     return Input::Key::UP;
                case SDLK_DOWN:   return Input::Key::DOWN;
                case SDLK_LEFT:   return Input::Key::LEFT;
                case SDLK_RIGHT:  return Input::Key::RIGHT;
                case SDLK_LCTRL:  return Input::Key::L_CTRL;
                case SDLK_LSHIFT: return Input::Key::L_SHIFT;
                case SDLK_LALT:   return Input::Key::L_ALT;
                case SDLK_RCTRL:  return Input::Key::R_CTRL;
                case SDLK_RSHIFT: return Input::Key::R_SHIFT;
                case SDLK_RALT:   return Input::Key::R_ALT;
            }
            return Input::Key::UNKNOWN;
        }

    public:
        Impl(const std::string& title, unsigned int width, unsigned int height);
        ~Impl();

        std::size_t getWidth() const;
        std::size_t getHeight() const;

        void init();
        void createContext(Render::OpenGL::ContextPtr& context);
        void swapBuffers();

        template<class T>
        void pollEvents(T&& receiver) {
            SDL_Event sdlEvent;
            while (SDL_PollEvent(&sdlEvent)) {
                switch (sdlEvent.type) {
                case SDL_QUIT: {
                    receiver(Command::Engine::CommandQuit());
                    break;
                }
                case SDL_KEYDOWN: {
                    receiver(Command::Engine::CommandKeyboard(s2eKey(sdlEvent.key.keysym), true));
                    break;
                }
                case SDL_KEYUP: {
                    receiver(Command::Engine::CommandKeyboard(s2eKey(sdlEvent.key.keysym), false));
                    break;
                }
                case SDL_MOUSEMOTION: {
                    receiver(Command::Engine::CommandMouse(sdlEvent.motion.x, sdlEvent.motion.y, sdlEvent.motion.xrel, sdlEvent.motion.yrel));
                    break;
                }
                }
            }
        }
    };
}
