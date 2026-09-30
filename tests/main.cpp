#include <ENG2D/ENG_Main.hpp>

int main()
{
    ENG::Init();
    ENG_Window window = ENG::CreateWindow("le wrapler rawr", Vector2(640, 480), SDL_WINDOW_RESIZABLE);

    ENG_Camera camera = ENG::CreateCamera(&window);

    ENG_Texture test = ENG::CreateTexture(&window, "billGates.bmp");

    ENG_File file = ENG::CreateFile("test.json");

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::console.LogDebug(value);

    while (ENG::Update())
    {
        SDL_Delay(1);
        
        ENG::draw.DrawTexture(&camera, &test, ENG::input.GetMouseWorldPos(&camera));
    }
    file.writeProperty("value", ENG::timer.FPS);
    ENG::Shutdown();
    return 0;
}