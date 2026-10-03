#include <ENG2D/Main.hpp>

int main()
{
    ENG::Init();

    ENG::Window window = ENG::CreateWindow("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

    ENG::Camera camera = ENG::CreateCamera(&window);

    ENG::Texture test = ENG::CreateTexture(&window, "billGates.bmp");

    ENG::File file = ENG::CreateFile("test.json");

    ENG::Dingus ground = ENG::CreateDingus(&camera, &test);
    ground.SetState(false);
    ground.SetPosition({0, -500});

    ENG::Dingus box = ENG::CreateDingus(&camera, &test);

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::console.LogDebug(value);

    camera.zoom = 0.5;

    while (ENG::Update())
    {
        SDL_Delay(1);

        if (ENG::input.keyState(SDL_SCANCODE_W))
        {
            camera.position.y += 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::input.keyState(SDL_SCANCODE_A))
        {
            camera.position.x -= 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::input.keyState(SDL_SCANCODE_S))
        {
            camera.position.y -= 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::input.keyState(SDL_SCANCODE_D))
        {
            camera.position.x += 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::input.keyState(SDL_SCANCODE_UP))
        {
            camera.zoom += 1.0 * camera.zoom * ENG::timer.delta;
        }
        if (ENG::input.keyState(SDL_SCANCODE_DOWN))
        {
            camera.zoom -= 1.0 * camera.zoom * ENG::timer.delta;
        }

        if (ENG::input.GetMouseState(SDL_BUTTON_LEFT))
        {
            box.ApplyForce((ENG::input.GetMouseWorldPos(&camera) - ENG::Vector2<double>(b2Body_GetTransform(box.B2D.ID).p.x, b2Body_GetTransform(box.B2D.ID).p.y)).Scale(1000, true));
        }
    
    }

    file.writeProperty("value", ENG::timer.FPS);
    ENG::Shutdown();
    return 0;
}