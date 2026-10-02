#include <ENG2D/Main.hpp>

int main()
{
    ENG::Init();
    
    ENG::Window window = ENG::CreateWindow("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

    ENG::Camera camera = ENG::CreateCamera(&window);

    ENG::Texture test = ENG::CreateTexture(&window, "billGates.bmp");

    ENG::File file = ENG::CreateFile("test.json");

    ENG::Dingus ground = ENG::CreateDingus(&camera, &test);
    ENG::Dingus box = ENG::CreateDingus(&camera, &test);
    ENG::Dingus ball = ENG::CreateDingus(&camera, &test);

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::console.LogDebug(value);

    while (ENG::Update())
    {
        SDL_Delay(1);

        // ENG::draw.DrawTexture(&camera, &test, ENG::input.GetMouseWorldPos(&camera));
    }
    file.writeProperty("value", ENG::timer.FPS);
    ENG::Shutdown();
    return 0;
}