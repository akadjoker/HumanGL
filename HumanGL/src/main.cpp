
#include "Core.hpp"
#include "Animation.hpp"

int main(int argc, char *argv[])
{
    Device *window = Device::GetInstance();

    bool ABORT = false;

    if (!window->Init("HumanGL BY Luis Santos AKA DJOKER", 800, 600))
    {
        return 1;
    }

    RenderBatch batch;
    batch.Init(1, 1024);
    Shader shader;
    Shader shaderCube;
    Font font;


    {
        const char *vShader = GLSL(
            layout(location = 0) in vec3 position;
            layout(location = 1) in vec2 texCoord;
            layout(location = 2) in vec4 color;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            out vec2 TexCoord;
            out vec4 vertexColor;
            void main() {
                gl_Position = projection * view * model * vec4(position, 1.0);
                TexCoord = texCoord;
                vertexColor = color;
            });

        const char *fShader = GLSL(
            in vec2 TexCoord;
            out vec4 color;
            in vec4 vertexColor;
            uniform sampler2D texture0;
            void main() {
                color = texture(texture0, TexCoord) * vertexColor;
            });

        shader.Create(vShader, fShader);
        shader.LoadDefaults();
    }

    {
        const char *vShader = GLSL(
            layout(location = 0) in vec3 position;

            uniform mat4 model;
            uniform mat4 view;
            uniform mat4 projection;

            void main() {
                gl_Position = projection * view * model * vec4(position, 1.0);
            });

        const char *fShader = GLSL(
            out vec4 color;
            uniform vec3 difusse;
            void main() {
                color = vec4(difusse, 1.0);
            });

        if (!shaderCube.Create(vShader, fShader))
        {
            ABORT = true;
        }
        shaderCube.LoadDefaults();
    }

    font.LoadDefaultFont();
    font.SetBatch(&batch);
    font.SetSize(12);

    Camera camera(800.0f / 600.0f);

    Driver::Instance().SetClearColor(0.2f, 0.3f, 0.3f, 1.0);

    float cameraSpeed = 0.5f;
    float mouseSensitivity = 0.1f;

    Humanoid human;


    GUI *widgets = GUI::Instance();

    Window *window1 = widgets->CreateWindow("Mesh Transform", 10, 360, 300, 350);

    window1->CreateLabel("yaw", 10, 30);
    window1->CreateLabel("pitch", 10, 74);
    window1->CreateLabel("roll", 10, 114);

    // Slider *sliderYaw   = window1->CreateSlider(false, 10, 50, 200, 20, 0, 360, 0);
    // Slider *sliderPitch = window1->CreateSlider(false, 10, 90, 200, 20, 0, 360, 0);
    // Slider *sliderRoll  = window1->CreateSlider(false, 10, 130, 200, 20, 0, 360, 0);

    // Slider *colorLerp = window1->CreateSlider(false, 10, 300, 160, 20, 0, 1, 0.5);
    // Button *buttonLerp = window1->CreateButton("Swicth", 200, 300, 80, 20);
    // buttonLerp->SetkeyMap(true,SDLK_t);

    // Button *moveLeft = window1->CreateButton("move - x", 10, 170, 100, 20);
    // moveLeft->SetkeyMap(true,SDLK_LEFT);
    // Button *moveRight = window1->CreateButton("move + x", 120, 170, 100, 20);
    // moveRight->SetkeyMap(true,SDLK_RIGHT);

    // Button *moveUp = window1->CreateButton("move - y", 10, 200, 100, 20);
    // moveUp->SetkeyMap(true,SDLK_UP);
    // Button *moveDown = window1->CreateButton("move + y", 120, 200, 100, 20);
    // moveDown->SetkeyMap(true,SDLK_DOWN);

    // Button *moveFront = window1->CreateButton("move - z", 10, 230, 100, 20);
    // moveFront->SetkeyMap(true,SDLK_PAGEUP);
    // Button *moveBack = window1->CreateButton("move + z", 120, 230, 100, 20);
    // moveBack->SetkeyMap(true,SDLK_PAGEDOWN);

    // Button *reset = window1->CreateButton("reset", 50, 260, 100, 20);

    // Window *window2 = widgets.CreateWindow("Mesh Tool", SCREEN_WIDTH-160, 24, 150, 190);

    // Button *buttonUv= window2->CreateButton("Uv", 10, 50, 100, 20);
    // Slider *sliderUv = window2->CreateSlider(false, 10, 20, 100, 20, 0.01f, 5.0f, 1.0f);

    // Button *buttonCenter= window2->CreateButton("Center Mesh", 10, 100, 100, 20);
    // Button *buttonShades= window2->CreateButton("Shades/Gray", 10, 130, 100, 20);
    // CheckBox *backFaces = window2->CreateCheckBox("Back Faces", true, 10, 160, 20, 20);

    while (window->Running())
    {
        if (ABORT)
            break;

        float delta = window->GetFrameTime();
        widgets->Update(delta);

        if (Input::IsKeyDown(SDLK_W))
        {
            camera.move(cameraSpeed);
        }
        else if (Input::IsKeyDown(SDLK_S))
        {
            camera.move(-cameraSpeed);
        }

        if (Input::IsKeyDown(SDLK_A))
        {
            camera.strafe(-cameraSpeed);
        }
        else if (Input::IsKeyDown(SDLK_D))
        {
            camera.strafe(cameraSpeed);
        }

        if (Input::IsMouseButtonDown(SDL_BUTTON_LEFT) && !(widgets->Focus()))
        {
            int x = Input::GetMouseDeltaX();
            int y = Input::GetMouseDeltaY();
            camera.rotate(x, -y, mouseSensitivity);
        }

        Driver::Instance().SetBlend(false);
        Driver::Instance().SetDepthTest(true);
        Driver::Instance().SetDepthWrite(true);
        Driver::Instance().Clear();

        glViewport(0, 0, window->GetWidth(), window->GetHeight());

        Mat4 projection = camera.getProjectionMatrix();
        Mat4 view = camera.getViewMatrix();
        camera.setAspectRatio(window->GetWidth() / window->GetHeight());

        Mat4 identity = Mat4::Identity();

        // Render Scene

        if (Input::IsKeyDown(SDLK_1))
        {
            human.playAnimation("walk");
        }
        else if (Input::IsKeyDown(SDLK_2))
        {
            human.playAnimation("jump");
        }else if (Input::IsKeyDown(SDLK_3))
        {
            human.playAnimation("dance");
        }
         else if (Input::IsKeyDown(SDLK_4))
        {
            human.playAnimation("fight");
        }


        shaderCube.Use();
        shaderCube.SetMatrix4("model", identity.m);
        shaderCube.SetMatrix4("view", view.m);
        shaderCube.SetMatrix4("projection", projection.m);

     
        human.render(shaderCube);

   

        human.animate(delta);

        // Render 3d Batch

        shader.Use();
        shader.SetMatrix4("model", identity.m);
        shader.SetMatrix4("view", view.m);
        shader.SetMatrix4("projection", projection.m);
        batch.SetColor(255, 255, 255, 255);

        batch.Grid(10, 10);
        batch.Render();

        // Render 2d STUFF

        projection = Mat4::Ortho(0.0f, window->GetWidth(), window->GetHeight(), 0.0f, -1.0f, 1.0f);
        Driver::Instance().SetBlend(true);
        Driver::Instance().SetBlendMode(BlendMode::BLEND);
        Driver::Instance().SetDepthTest(false);
        Driver::Instance().SetDepthWrite(false);

        shader.Use();
        shader.SetMatrix4("model", identity.m);
        shader.SetMatrix4("view", identity.m);
        shader.SetMatrix4("projection", projection.m);
        batch.SetColor(255, 255, 255, 255);

        widgets->Render(&batch);

        font.Print(10, 20, "FPS %d Focus %d", window->GetFPS(), (widgets->Focus() ? 1 : 0));

        batch.Render();

        window->Swap();
    }

    widgets->Clear();
    font.Release();
    batch.Release();
    shader.Release();
    shaderCube.Release();
    window->Cleanup();
    Device::DestroyInstance();
    return 0;
}