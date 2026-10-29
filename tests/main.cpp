#include <ENG2D/Main.hpp>
#include <memory>
#include <stdexcept>

ENG::Window window = ENG::Window("le wrapler rawr", ENG::Vector2(640, 480), SDL_WINDOW_RESIZABLE);

ENG::Camera camera = ENG::Camera(&window);

ENG::Texture test = ENG::Texture(&window, "assets/textures/billGates.bmp");
ENG::Texture cursor = ENG::Texture(&window, "assets/textures/cursor.png");
ENG::Texture cursor2 = ENG::Texture(&window, "assets/textures/cursorpressed.png");
ENG::Texture chud = ENG::Texture(&window, "assets/textures/chud.png");

ENG::File file = ENG::File("test.json");

ENG::Font font = ENG::Font("assets/fonts/Retro.Gaming.Extended.ttf", 32);

ENG::Timer timer;

// std::vector<ENG::Dingus *> balls;

void WhenBallClicked(ENG::Dingus *me)
{
    if (me->GetName() == "ball")
    {
        me->Destroy();
    }
}

ENG::Dingus *nearest = nullptr;


void ResetCursor()
{
    window.SetCustomCursor(cursor);
}
ENG::RunOnce resetCursor(ResetCursor);
void LayerAndCalcNearest()
{
    nearest->SetRenderLayer(0);
    window.SetCustomCursor(cursor2);
    nearest = ENG::Dingus::GetNearest(ENG::Input::Mouse::GetMouseWorldPos(&camera), "ball");
    nearest->SetRenderLayer(1);
}
ENG::RunOnce layerAndCalcNearest(LayerAndCalcNearest);
void MouseLeft(bool buttonState)
{
    resetCursor.OnFalse(buttonState);
    layerAndCalcNearest.OnTrue(buttonState);
}


void MouseRight(bool buttonState)
{
    
}

void MoveUp(bool keyState)
{
    if (keyState)
        camera.position.y += 200.0 / camera.zoom * timer.delta;
}
void MoveDown(bool keyState)
{
    if (keyState)
        camera.position.y -= 200.0 / camera.zoom * timer.delta;
}
void MoveLeft(bool keyState)
{
    if (keyState)
        camera.position.x -= 200.0 / camera.zoom * timer.delta;
}
void MoveRight(bool keyState)
{
    if (keyState)
        camera.position.x += 200.0 / camera.zoom * timer.delta;
}
void ZoomIn(bool keyState)
{
    if (keyState)
        camera.zoom += 1.0 * camera.zoom * timer.delta;
}
void ZoomOut(bool keyState)
{
    if (keyState)
        camera.zoom -= 1.0 * camera.zoom * timer.delta;
}

ENG::Action zoomIn = ENG::Action(ZoomIn, SDL_SCANCODE_UP);
ENG::Action zoomOut = ENG::Action(ZoomOut, SDL_SCANCODE_DOWN);
ENG::Action moveRight = ENG::Action(MoveRight, SDL_SCANCODE_D);
ENG::Action moveLeft = ENG::Action(MoveLeft, SDL_SCANCODE_A);
ENG::Action moveUp = ENG::Action(MoveUp, SDL_SCANCODE_W);
ENG::Action moveDown = ENG::Action(MoveDown, SDL_SCANCODE_S);

ENG::Action mouseLeft = ENG::Action(MouseLeft, SDL_BUTTON_LEFT);
ENG::Action mouseRight = ENG::Action(MouseRight, SDL_BUTTON_RIGHT);

int main()
{
    ENG::Init();

    window.SetCustomCursor(cursor);
    // window.cursorScale = {2, 2};
    window.SetScale(2.0f);

    ENG::Dingus *ground = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(0, -500, 5000, chud.size.y));
    ground->AssignCollisionShape(ENG::CollisionShape::Box(ground->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    ENG::Dingus *wall1 = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(2500, 2000, chud.size.x, 5000));
    wall1->AssignCollisionShape(ENG::CollisionShape::Box(wall1->GetTransform().size), 1, 0.25, 0.5)->SetStatic();
    ENG::Dingus *wall2 = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(-2500, 2000, chud.size.x, 5000));
    wall2->AssignCollisionShape(ENG::CollisionShape::Box(wall2->GetTransform().size), 1, 0.25, 0.5)->SetStatic();

    for (size_t i = 0; i < 50; ++i)
    {
        ENG::Dingus *ball = ENG::Dingus::Create(&camera, &chud, ENG::Transform2<float>(0, 1280 * i, chud.size * ENG::Vector2<int>(1, 1)));
        ball->AssignCollisionShape(ENG::CollisionShape::Box(chud.size * ENG::Vector2<int>(1, 1)), 1, 0.25, 0.5);
        ball->SetName("ball");
        ball->AssignFunction_MouseClickR(WhenBallClicked);
        // balls.push_back(ball);
    }

    double value;
    file.readProperty("value", &value, 10.0);

    ENG::Console::LogDebug(value);

    camera.zoom = 0.1;

    //ENG::Input::Mouse::RegisterMouseDown_L(onMouseDownL);
    //ENG::Input::Mouse::RegisterMouseDown_R(onMouseDownR);
    //ENG::Input::Mouse::RegisterMouseUp_L(onMouseUpL);

    while (ENG::Update())
    {
        timer.Update();

        ENG::Dingus::SetWorldTimeStep(timer.delta * 10);

        if (ENG::Input::Mouse::Left)
        {
            nearest->ApplyForce((ENG::Input::Mouse::GetMouseWorldPos(&camera) - nearest->GetPosition()).Scale(500, true));
        }
        // window.cursorScale = ENG::Vector2<float>((cosf(ENG::timer.now_s() * 10.0f) * 0.5f) + 2.0f, (sinf(ENG::timer.now_s() * 10.0f) * 0.5f) + 2.0f);
        // ENG::DrawTools::DrawPixel(&camera, ENG::Input::GetMouseWorldPos(&camera));
        // ENG::DrawTools::DrawTexture(&camera, &chud, ENG::Input::GetMouseWorldPos(&camera));
        // ENG::DrawTools::DrawFont(&camera, &font, ENG::Input::GetMouseWorldPos(&camera), {0, 0}, 4);
    }

    file.writeProperty("value", timer.fps);
    ENG::Shutdown();
    return 0;
}