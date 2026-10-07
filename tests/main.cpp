#include <ENG2D/Main.hpp>
#include <memory>
#include <stdexcept>

ENG::Window window = ENG::Window("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

ENG::Camera camera = ENG::Camera(&window);

ENG::Texture test = ENG::Texture(&window, "billGates.bmp");
ENG::Texture cursor = ENG::Texture(&window, "cursor.png");
ENG::Texture cursor2 = ENG::Texture(&window, "cursorpressed.png");

ENG::File file = ENG::File("test.json");

ENG::Timer timer;

std::vector<ENG::Dingus *> balls;

ENG::Dingus *nearest = nullptr;
bool flag_onMouseDown = false;
void onMouseDownL()
{
    nearest->SetRenderLayer(0);
    window.SetCustomCursor(cursor2);
    nearest = ENG::Dingus::GetNearest(ENG::Input::GetMouseWorldPos(&camera));
    nearest->SetRenderLayer(1);
}

void onMouseDownR()
{
    // ENG::Dingus::GetNearest(ENG::Input::GetMouseWorldPos(&camera))->Destroy();
    if (balls.size() > 0)
    {
        balls.at(0)->Destroy();
        balls.erase(balls.begin());
    }
}

void onMouseUpL()
{
    window.SetCustomCursor(cursor);
}

int main()
{
    ENG::Init();

    window.SetCustomCursor(cursor);
    window.cursorScale = {2, 2};

    ENG::Dingus *ground = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(0, -500, 5000, test.size.y));
    ground->AssignCollisionShape(ENG::CollisionShape::Box(ground->GetTransform().size, 1, 2), 1, 0.25, 0.5)->SetStatic();
    ENG::Dingus *wall1 = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(2500, 2000, test.size.x, 5000));
    wall1->AssignCollisionShape(ENG::CollisionShape::Box(wall1->GetTransform().size, 1, 2), 1, 0.25, 0.5)->SetStatic();
    ENG::Dingus *wall2 = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(-2500, 2000, test.size.x, 5000));
    wall2->AssignCollisionShape(ENG::CollisionShape::Box(wall2->GetTransform().size, 1, 2), 1, 0.25, 0.5)->SetStatic();

    for (size_t i = 0; i < 100; ++i)
    {
        ENG::Dingus *ball = ENG::Dingus::Create(&camera, &test, ENG::Transform2<float>(0, 128 * i, test.size));
        ball->AssignCollisionShape(ENG::CollisionShape::Box(test.size, 2, 1), 1, 0.25, 0.5);
        balls.push_back(ball);
    }

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::Console::LogDebug(value);

    camera.zoom = 0.5;

    ENG::Input::RegisterMouseDown_L(onMouseDownL);
    ENG::Input::RegisterMouseDown_R(onMouseDownR);
    ENG::Input::RegisterMouseUp_L(onMouseUpL);

    while (ENG::Update())
    {
        timer.Update();

        SDL_Delay(1);

        if (ENG::Input::keyState(SDL_SCANCODE_W))
        {
            camera.position.y += 200.0 / camera.zoom * timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_A))
        {
            camera.position.x -= 200.0 / camera.zoom * timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_S))
        {
            camera.position.y -= 200.0 / camera.zoom * timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_D))
        {
            camera.position.x += 200.0 / camera.zoom * timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_UP))
        {
            camera.zoom += 1.0 * camera.zoom * timer.delta;
        }
        if (ENG::Input::keyState(SDL_SCANCODE_DOWN))
        {
            camera.zoom -= 1.0 * camera.zoom * timer.delta;
        }

        if (ENG::Input::GetMouseState(SDL_BUTTON_LEFT))
        {
            nearest->ApplyForce((ENG::Input::GetMouseWorldPos(&camera) - nearest->GetPosition()).Scale(500, true));
        }
        // window.cursorScale = ENG::Vector2<float>((cosf(ENG::timer.now_s() * 10.0f) * 0.5f) + 2.0f, (sinf(ENG::timer.now_s() * 10.0f) * 0.5f) + 2.0f);
    }

    file.writeProperty("value", timer.fps);
    ENG::Shutdown();
    return 0;
}