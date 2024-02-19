#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <iostream>
SDL_bool isColliding(const SDL_Rect *A, const SDL_Rect *B) {
  return SDL_HasIntersection(A, B);
}
struct GameState {
  int score{0};
  bool wasColliding{false};
};
int main(int argc, char *argv[]) {

  srand(time(NULL)); // Initialize random seed
  int maxFrameRate = 60;
  // GameState *gameState;
  GameState *gameState = new GameState();
  SDL_Window *window = nullptr;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not be initialized.\n";
  } else {
    std::cout << "SDL Video System is ready to go.\n";
  }
  // create window
  window =
      SDL_CreateWindow("cross the road", 20, 20, 640, 480, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer{nullptr};
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (TTF_Init() == -1) {
    std::cerr << "Could not initialize SDL_ttf. Error: " << SDL_GetError()
              << '\n';
  } else {
    std::cout << "SDL_ttf is ready.\n";
  }

  // font initialization
  TTF_Font *font{
      TTF_OpenFont("./assets/fonts/8bitOperatorPlus8-Regular.ttf", 32)};

  SDL_Surface *text_surface =
      TTF_RenderText_Solid(font, "score: ", {255, 255, 255});
  SDL_Surface *road_surface = SDL_LoadBMP("./assets/images/road.bmp");
  SDL_Surface *player_surface = SDL_LoadBMP("./assets/images/ball.bmp");
  SDL_Surface *stone_surface = SDL_LoadBMP("./assets/images/stone.bmp");

  // to make transparency
  SDL_SetColorKey(road_surface, SDL_TRUE,
                  SDL_MapRGB(road_surface->format, 0xFF, 0, 0xFF));

  SDL_SetColorKey(player_surface, SDL_TRUE,
                  SDL_MapRGB(player_surface->format, 0xFF, 0, 0xFF));

  SDL_SetColorKey(stone_surface, SDL_TRUE,
                  SDL_MapRGB(stone_surface->format, 0xFF, 0, 0xFF));
  //  create surfaces
  SDL_Texture *road_texture =
      SDL_CreateTextureFromSurface(renderer, road_surface);

  SDL_Texture *player_texture =
      SDL_CreateTextureFromSurface(renderer, player_surface);

  SDL_Texture *stone_texture =
      SDL_CreateTextureFromSurface(renderer, stone_surface);

  SDL_Texture *font_texture =
      SDL_CreateTextureFromSurface(renderer, text_surface);

  // clean up memory
  SDL_FreeSurface(player_surface);
  SDL_FreeSurface(road_surface);
  SDL_FreeSurface(stone_surface);
  SDL_FreeSurface(text_surface);

  SDL_Rect road1;
  road1.x = 0;
  road1.y = 0;
  road1.w = 640;
  road1.h = 480;

  SDL_Rect road2;
  road2.x = -639;
  road2.y = 0;
  road2.w = 640;
  road2.h = 480;

  SDL_Rect stone1;
  stone1.x = -200;
  stone1.y = 300;
  stone1.w = 50;
  stone1.h = 70;

  SDL_Rect stone2;
  stone2.x = -500;
  stone2.y = 50;
  stone2.w = 50;
  stone2.h = 70;

  SDL_Rect stone3;
  stone3.x = -300;
  stone3.y = 400;
  stone3.w = 50;
  stone3.h = 70;

  SDL_Rect stone4;
  stone4.x = -200;
  stone4.y = 70;
  stone4.w = 50;
  stone4.h = 70;

  SDL_Rect stone5;
  stone5.x = -600;
  stone5.y = 500;
  stone5.w = 50;
  stone5.h = 70;

  SDL_Rect text;
  text.x = 10;
  text.y = 10;
  text.w = 100;
  text.h = 100;

  SDL_Rect player;
  bool gameIsRunning{true};
  // Initialize game state
  gameState->score = 0;

  while (gameIsRunning) {

    Uint32 startTime{SDL_GetTicks()};
    SDL_Event event;

    int mouseX{};
    int mouseY{};
    Uint32 buttons{SDL_GetMouseState(&mouseX, &mouseY)};

    while (SDL_PollEvent(&event)) {
      const Uint8 *state{SDL_GetKeyboardState(NULL)};
      if (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) {
        gameIsRunning = false;
        std::cout << "Escaped key pressed.\n";
      }

      // Collision detection with stones
      bool isCollidingWithAnyStone =
          isColliding(&player, &stone1) || isColliding(&player, &stone2) ||
          isColliding(&player, &stone3) || isColliding(&player, &stone4) ||
          isColliding(&player, &stone5);

      if (isCollidingWithAnyStone) {
        if (!gameState->wasColliding) {
          std::cout << "Collision detected!\n";
          gameState->score++; // Increment score only when collision starts
        }
      } else {
        gameState->wasColliding = false; // Reset collision flag
      }

      gameState->wasColliding =
          isCollidingWithAnyStone; // Update collision flag for next frame
    }

    SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    player.x = mouseX;
    player.y = mouseY;
    player.w = 50;
    player.h = 50;

    // handle texture scrolling left and right

    road1.x += 10;
    road2.x += 10;
    stone1.x += 10;
    stone2.x += 15;
    stone3.x += 10;
    stone4.x += 15;
    stone5.x += 5;

    if (road1.x > 639)
      road1.x = -639;

    if (road2.x > 639)
      road2.x = -639;

    if (stone1.x > 639) {
      stone1.x = -rand() % 300; // Randomize the position off-screen
      stone1.y =
          rand() % 400; // Randomize the y position within the window height
    }

    if (stone2.x > 639) {
      stone2.x = -rand() % 300; // Randomize the position off-screen
      stone2.y =
          rand() % 400; // Randomize the y position within the window height
    }

    if (stone3.x > 639) {

      stone3.x = -rand() % 300; // Randomize the position off-screen
      stone3.y =
          rand() % 400; // Randomize the y position within the window height
    }

    if (stone4.x > 639) {
      stone4.x = -rand() % 300; // Randomize the position off-screen
      stone4.y =
          rand() % 400; // Randomize the y position within the window height
    }

    if (stone5.x > 639) {
      stone5.x = -rand() % 300; // Randomize the position off-screen
      stone5.y =
          rand() % 400; // Randomize the y position within the window height
    }

    SDL_RenderCopy(renderer, road_texture, NULL, &road1);
    SDL_RenderCopy(renderer, road_texture, NULL, &road2);
    SDL_RenderCopy(renderer, player_texture, NULL, &player);
    SDL_RenderCopy(renderer, stone_texture, NULL, &stone1);
    SDL_RenderCopy(renderer, stone_texture, NULL, &stone2);
    SDL_RenderCopy(renderer, stone_texture, NULL, &stone3);
    SDL_RenderCopy(renderer, stone_texture, NULL, &stone4);
    SDL_RenderCopy(renderer, stone_texture, NULL, &stone5);
    // SDL_RenderCopy(renderer, font_texture, NULL, &text);
    // Render the score
    std::string scoreText = "Score: " + std::to_string(gameState->score);
    text_surface =
        TTF_RenderText_Solid(font, scoreText.c_str(), {255, 255, 255});
    font_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_RenderCopy(renderer, font_texture, NULL, &text);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(font_texture);
    SDL_RenderPresent(renderer);

    Uint32 elapsedTime{SDL_GetTicks() - startTime};
    if (elapsedTime < maxFrameRate) {
      // Eventually set a frame cap
      SDL_Delay(maxFrameRate - elapsedTime);
    }
  }

  SDL_DestroyTexture(player_texture);
  SDL_DestroyTexture(road_texture);
  SDL_DestroyTexture(stone_texture);
  SDL_DestroyTexture(font_texture);

  SDL_DestroyWindow(window);

  delete gameState;
  SDL_Quit();

  return 0;
}
