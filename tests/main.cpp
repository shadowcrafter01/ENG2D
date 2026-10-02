#include <ENG2D/Main.hpp>

int main()
{
    ENG::Init();

    ENG::Window window = ENG::CreateWindow("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

    ENG::Camera camera = ENG::CreateCamera(&window);

    ENG::Texture test = ENG::CreateTexture(&window, "billGates.bmp");

    ENG::File file = ENG::CreateFile("test.json");

    ENG::Dingus ground = ENG::CreateDingus(&camera, &test);
    // ground.B2D.def.type = b2_staticBody;
    // ground.B2D.def.position = {0, -500};
    b2Body_SetType(ground.B2D.ID, b2_staticBody);
    b2Body_SetTransform(ground.B2D.ID, {0, -500}, b2Body_GetRotation(ground.B2D.ID));

    ENG::Dingus box = ENG::CreateDingus(&camera, &test);

    // ENG::Dingus ball = ENG::CreateDingus(&camera, &test);

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::console.LogDebug(value);

    camera.zoom = 0.5;

    while (ENG::Update())
    {
        SDL_Delay(1);

        // ENG::draw.DrawTexture(&camera, &test, ENG::input.GetMouseWorldPos(&camera));

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
            b2Body_ApplyForceToCenter(box.B2D.ID, (ENG::input.GetMouseWorldPos(&camera) - ENG::Vector2<double>(b2Body_GetTransform(box.B2D.ID).p.x, b2Body_GetTransform(box.B2D.ID).p.y)).Scale(1000, true), true);
        }
    
    }

    file.writeProperty("value", ENG::timer.FPS);
    ENG::Shutdown();
    return 0;
}