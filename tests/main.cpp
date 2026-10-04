#include <ENG2D/Main.hpp>
#include <memory>
#include <stdexcept>

ENG::Window window = ENG::CreateWindow("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

ENG::Camera camera = ENG::CreateCamera(&window);

ENG::Texture test = ENG::CreateTexture(&window, "billGates.bmp");
ENG::Texture cursor = ENG::CreateTexture(&window, "cursor.png");

ENG::File file = ENG::CreateFile("test.json");

ENG::Dingus *nearest = nullptr;
bool flag_onMouseDown = false;
void onMouseDownL()
{
    nearest = ENG::Dingus::GetNearest(ENG::Input::GetMouseWorldPos(&camera));
}

void onMouseDownR()
{
    ENG::Dingus::GetNearest(ENG::Input::GetMouseWorldPos(&camera))->Destroy();
}

int main()
{
    ENG::Init();
    ENG::SetCustomCursor(&cursor, ENG::Vector2<float>(2, 2));

    ENG::Dingus *ground = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(0, -500, 5000, test.size.y));
    ground->AssignCollisionShape(ENG::CollisionShape::Box(ground->GetTransform().size))->SetStatic();
    ENG::Dingus *wall1 = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(2500, 2000, test.size.x, 5000));
    wall1->AssignCollisionShape(ENG::CollisionShape::Box(wall1->GetTransform().size))->SetStatic();
    ENG::Dingus *wall2 = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(-2500, 2000, test.size.x, 5000));
    wall2->AssignCollisionShape(ENG::CollisionShape::Box(wall2->GetTransform().size))->SetStatic();

    // for (size_t i = 0; i < 100; ++i)
    //{
    //     ENG::Dingus *ball = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(0, 128 * i, test.size));
    //     ball->AssignCollisionShape(ENG::CollisionShape::Box(test.size), 1, 0.25, 0.5);
    // }

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::console.LogDebug(value);

    camera.zoom = 0.5;

    ENG::Input::RegisterMouseDown_L(onMouseDownL);
    ENG::Input::RegisterMouseDown_R(onMouseDownR);

    while (ENG::Update())
    {
        SDL_Delay(1);

        if (ENG::Input::keyState(SDL_SCANCODE_W))
        {
            camera.position.y += 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_A))
        {
            camera.position.x -= 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_S))
        {
            camera.position.y -= 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_D))
        {
            camera.position.x += 200.0 / camera.zoom * ENG::timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_UP))
        {
            camera.zoom += 1.0 * camera.zoom * ENG::timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_DOWN))
        {
            camera.zoom -= 1.0 * camera.zoom * ENG::timer.delta;
        }

        if (ENG::Input::GetMouseState(SDL_BUTTON_LEFT))
        {
            nearest->ApplyForce((ENG::Input::GetMouseWorldPos(&camera) - nearest->GetPosition()).Scale(500, true));
        }
    }

    file.writeProperty("value", ENG::timer.FPS);
    ENG::Shutdown();
    return 0;
}