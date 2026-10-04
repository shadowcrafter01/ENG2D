#include <ENG2D/Main.hpp>
#include <memory>
#include <stdexcept>

int main()
{
    ENG::Init();

    ENG::Window window = ENG::CreateWindow("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

    ENG::Camera camera = ENG::CreateCamera(&window);

    ENG::Texture test = ENG::CreateTexture(&window, "billGates.bmp");

    ENG::File file = ENG::CreateFile("test.json");

    ENG::Dingus ground = ENG::Dingus(&camera, &test, ENG::Transform2<float>(0, -500, 5000, test.size.y));
    ground.AssignCollisionShape(ENG::CollisionShape::Box(5000, test.size.y))->SetStatic();
    ENG::Dingus wall1 = ENG::Dingus(&camera, &test, ENG::Transform2<float>(2500, 2000, test.size.x, 5000));
    wall1.AssignCollisionShape(ENG::CollisionShape::Box(test.size.x, 5000))->SetStatic();
    ENG::Dingus wall2 = ENG::Dingus(&camera, &test, ENG::Transform2<float>(-2500, 2000, test.size.x, 5000));
    wall2.AssignCollisionShape(ENG::CollisionShape::Box(test.size.x, 5000))->SetStatic();

    ENG::Dingus box = ENG::Dingus(&camera, &test);
    box.AssignCollisionShape(ENG::CollisionShape::Box(test.size.x, test.size.y));
    box.SetPosition(0, 200);

    std::vector<std::unique_ptr<ENG::Dingus>> balls;
    for (size_t i = 0; i < 100; ++i)
    {
        auto ball = std::make_unique<ENG::Dingus>(&camera, &test, ENG::Transform2<float>(0, 128 * i, test.size));
        ball->AssignCollisionShape(ENG::CollisionShape::Circle(test.size.x / 2.0));
        balls.push_back(std::move(ball));
    }

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
            box.ApplyForce((ENG::Input::GetMouseWorldPos(&camera) - box.GetTransform().position).Scale(1000, true));
        }
    }

    file.writeProperty("value", ENG::timer.FPS);
    ENG::Shutdown();
    return 0;
}