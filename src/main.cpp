#include <SDL2/SDL.h>
#include <iostream>


int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "SDL initialization failed : " << SDL_GetError() << std::endl;
        return 1;
    }

    // Check for joysticks
    int numJoysticks = SDL_NumJoysticks();
    std::cout << "Number of joysticks connected: " << numJoysticks << std::endl;

    if (numJoysticks < 1) {
        std::cout << "No joysticks connected!" << std::endl;
        SDL_Quit();
        return 1;
    }

    // Open the first joystick
    SDL_Joystick* joystick = SDL_JoystickOpen(0);
    if (joystick == nullptr) {
        std::cerr << "Failed to open joystick: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    std::cout << "Joystick Name: " << SDL_JoystickName(joystick) << std::endl;
    std::cout << "Number of Axes: " << SDL_JoystickNumAxes(joystick) << std::endl;
    std::cout << "Number of Buttons: " << SDL_JoystickNumButtons(joystick) << std::endl;
    std::cout << "Number of Hats: " << SDL_JoystickNumHats(joystick) << std::endl;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_JOYBUTTONDOWN:
                    std::cout << "Button " << (int)event.jbutton.button << " pressed" << std::endl;
                    break;
                case SDL_JOYAXISMOTION:
                    if (abs(event.jaxis.value) > 3200) { // Add dead zone
                        std::cout << "Axis " << (int)event.jaxis.axis << " moved to " 
                                << event.jaxis.value << std::endl;
                    }
                    break;
                case SDL_JOYHATMOTION:
                    std::cout << "Hat " << (int)event.jhat.hat << " moved to " 
                            << (int)event.jhat.value << std::endl;
                    break;
            }
        }

        SDL_Delay(16); // Cap at ~60 FPS
    }

    SDL_JoystickClose(joystick);
    SDL_Quit();
    return 0;
}