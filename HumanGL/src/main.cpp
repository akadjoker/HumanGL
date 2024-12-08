

#include <string>

#include <map>
#include "Core.hpp"

struct Vertex
{
    float position[3];
};

// Função para criar um cubo
MeshBuffer *CreateCube()
{

    Vertex vertices[] = {
        // Front
        {{-0.5f, -0.5f, 0.5f}}, // 0
        {{0.5f, -0.5f, 0.5f}},  // 1
        {{0.5f, 0.5f, 0.5f}},   // 2
        {{-0.5f, 0.5f, 0.5f}},  // 3

        // Bakc
        {{-0.5f, -0.5f, -0.5f}}, // 4
        {{0.5f, -0.5f, -0.5f}},  // 5
        {{0.5f, 0.5f, -0.5f}},   // 6
        {{-0.5f, 0.5f, -0.5f}}   // 7
    };

    // Definir os índices para desenhar as faces do cubo (36 índices: 6 faces * 2 triângulos * 3 vértices)
    unsigned int indices[] = {
        // Frente
        0, 1, 2,
        0, 2, 3,
        // Direita
        1, 5, 6,
        1, 6, 2,
        // Trás
        5, 4, 7,
        5, 7, 6,
        // Esquerda
        4, 0, 3,
        4, 3, 7,
        // Topo
        3, 2, 6,
        3, 6, 7,
        // Base
        4, 5, 1,
        4, 1, 0};

    VertexFormat format;
    format.addElement(VertexType::POSITION, 3); // x, y, z
    // format.addElement(VertexType::COLOR, 4);    // r, g, b, a

    MeshBuffer *mesh = new MeshBuffer();

    mesh->CreateVertexBuffer(format, 8);
    mesh->SetVertexData(vertices);

    mesh->CreateIndexBuffer(36);
    mesh->SetIndexData(indices);

    return mesh;
}

struct Pose
{
    Vec3 position;
    float torsoRotation;
    float headRotation;
    float upperArmRotation[2]; // [0] = esquerdo, [1] = direito
    float forearmRotation[2];  // [0] = esquerdo, [1] = direito
    float thighRotation[2];    // [0] = esquerdo, [1] = direito
    float calfRotation[2];     // [0] = esquerdo, [1] = direito

    Pose()
    {
        torsoRotation = 0.0f;
        headRotation = 0.0f;
        for (int i = 0; i < 2; i++)
        {
            upperArmRotation[i] = 0.0f;
            forearmRotation[i] = 0.0f;
            thighRotation[i] = 0.0f;
            calfRotation[i] = 0.0f;
        }
    }

    static Pose lerp(const Pose &a, const Pose &b, float t)
    {
        Pose result;
        result.torsoRotation = a.torsoRotation * (1 - t) + b.torsoRotation * t;
        result.headRotation = a.headRotation * (1 - t) + b.headRotation * t;
        result.position.x = a.position.x  * (1 - t) + b.position.x * t;
        result.position.y = a.position.y  * (1 - t) + b.position.y * t;
        result.position.z = a.position.z  * (1 - t) + b.position.z * t;

        for (int i = 0; i < 2; i++)
        {
            result.upperArmRotation[i] = a.upperArmRotation[i] * (1 - t) + b.upperArmRotation[i] * t;
            result.forearmRotation[i] = a.forearmRotation[i] * (1 - t) + b.forearmRotation[i] * t;
            result.thighRotation[i] = a.thighRotation[i] * (1 - t) + b.thighRotation[i] * t;
            result.calfRotation[i] = a.calfRotation[i] * (1 - t) + b.calfRotation[i] * t;
        }
        return result;
    }
};

struct Keyframe
{
    Pose pose;
    float time; // Tempo em segundos
};

class Animation
{
public:
    std::string name;
    std::vector<Keyframe> keyframes;
    bool loop;
    float duration;

    Animation()
    {
    }

    Animation(const std::string &name, bool loop = true)
        : name(name), loop(loop), duration(0.0f) {}

    void addKeyframe(const Pose &pose, float time)
    {
        Keyframe kf = {pose, time};
        keyframes.push_back(kf);
        duration = std::max(duration, time);
    }

    // pose para um determinado momento da animação
    Pose getPoseAtTime(float time)
    {
        if (keyframes.empty())
            return Pose();
        if (keyframes.size() == 1)
            return keyframes[0].pose;

        // Ajusta o tempo se a animação for em loop
        if (loop)
        {
            time = fmod(time, duration);
        }
        else
        {
            time = std::min(time, duration);
        }

        // Encontra os keyframes anterior e posterior
        size_t nextFrame = 0;
        for (; nextFrame < keyframes.size(); nextFrame++)
        {
            if (keyframes[nextFrame].time > time)
                break;
        }

        if (nextFrame == 0)
            return keyframes[0].pose;
        if (nextFrame == keyframes.size())
            return keyframes.back().pose;

        // Interpolacao entre os dois keyframes
        size_t prevFrame = nextFrame - 1;
        float frameDuration = keyframes[nextFrame].time - keyframes[prevFrame].time;
        float t = (time - keyframes[prevFrame].time) / frameDuration;

        return Pose::lerp(keyframes[prevFrame].pose, keyframes[nextFrame].pose, t);
    }
};

class AnimationManager
{
private:
    std::map<std::string, Animation> animations;
    std::string currentAnimation;
    std::string nextAnimation;
    float currentTime;
    float playbackSpeed;
    bool playing;
    bool looping;

public:
    AnimationManager() : currentTime(0.0f),
                         playbackSpeed(1.0f),
                         playing(false),
                         looping(true) {}

    void reset()
    {
        currentTime = 0.0f;
        playing = false;
        currentAnimation = "";
        nextAnimation = "";
    }

    std::string const CurrentAnimation() {return currentAnimation;}

    void addAnimation(const Animation &anim)
    {
        animations[anim.name] = anim;
    }

    void playAnimation(const std::string &name, bool shouldLoop = true, bool resetTime = true)
    {
        if (animations.find(name) != animations.end())
        {
            currentAnimation = name;
            looping = shouldLoop;

            if (resetTime)
            {
                currentTime = 0.0f;
            }

            playing = true;
        }
    }

    void update(float deltaTime)
    {
        if (!playing)
            return;

        // Atualiza o tempo atual
        currentTime += deltaTime * playbackSpeed;

        // Verifica se chegou ao fim da animação
        if (animations.find(currentAnimation) != animations.end())
        {
            Animation &anim = animations[currentAnimation];

            if (currentTime >= anim.duration)
            {
                if (looping)
                {
                    // Volta ao início se estiver em loop
                    currentTime = fmod(currentTime, anim.duration);
                }
                else
                {
                    // Para a animação se não estiver em loop
                    currentTime = anim.duration;
                    playing = false;
                }
            }
        }
    }
    void setPlaybackSpeed(float speed) { playbackSpeed = speed; }
    float getPlaybackSpeed() const { return playbackSpeed; }

    void pause() { playing = false; }
    void resume() { playing = true; }
    bool isPlaying() const { return playing; }

    float getCurrentTime() const { return currentTime; }
    void setCurrentTime(float time) { currentTime = time; }

    const std::string &getCurrentAnimationName() const { return currentAnimation; }

    bool isLooping() const { return looping; }
    void setLooping(bool loop) { looping = loop; }

    Pose getCurrentPose()
    {
        if (!playing || animations.find(currentAnimation) == animations.end())
        {
            return Pose();
        }
        return animations[currentAnimation].getPoseAtTime(currentTime);
    }

    void createDanceAnimation()
    {
        Animation dance("dance", true);
        const float PI = 3.14159f;

        // Pose 1: Braços para cima, quadril para um lado
        Pose pose1;
  
        pose1.torsoRotation = 15.0f * PI / 180.0f;
        pose1.upperArmRotation[0] = pose1.upperArmRotation[1] = -90.0f * PI / 180.0f; // Braços levantados
        pose1.forearmRotation[0] = pose1.forearmRotation[1] = -45.0f * PI / 180.0f;   // Cotovelos dobrados
        pose1.thighRotation[0] = -20.0f * PI / 180.0f;                                // Perna esquerda flexionada
        dance.addKeyframe(pose1, 0.0f);

        // Pose 2: Movimento de disco
        Pose pose2;
        pose2.torsoRotation = -15.0f * PI / 180.0f;
        pose2.upperArmRotation[0] = -45.0f * PI / 180.0f;  // Braço esquerdo diagonal
        pose2.upperArmRotation[1] = -135.0f * PI / 180.0f; // Braço direito diagonal
        pose2.forearmRotation[0] = pose2.forearmRotation[1] = -45.0f * PI / 180.0f;
        pose2.thighRotation[1] = -20.0f * PI / 180.0f; // Perna direita flexionada
        dance.addKeyframe(pose2, 0.5f);

        // Pose 3: Giro e movimento dos braços
        Pose pose3;
        pose3.torsoRotation = 180.0f * PI / 180.0f;
        pose3.upperArmRotation[0] = pose3.upperArmRotation[1] = -90.0f * PI / 180.0f;
        pose3.forearmRotation[0] = -90.0f * PI / 180.0f;
        pose3.forearmRotation[1] = 90.0f * PI / 180.0f;
        dance.addKeyframe(pose3, 1.0f);

        // Volta para pose inicial
        dance.addKeyframe(pose1, 1.5f);

        addAnimation(dance);
    }

    void createFighterAnimation()
    {
        Animation fight("fight", true);
        const float PI = 3.14159f;

        // Pose inicial de luta
        Pose fightStance;
        fightStance.torsoRotation = 45.0f * PI / 180.0f;        // Virado em diagonal
        fightStance.upperArmRotation[0] = -90.0f * PI / 180.0f; // Braço esquerdo em guarda
        fightStance.upperArmRotation[1] = -45.0f * PI / 180.0f; // Braço direito em guarda
        fightStance.forearmRotation[0] = -90.0f * PI / 180.0f;
        fightStance.forearmRotation[1] = -90.0f * PI / 180.0f;
        fightStance.thighRotation[0] = -30.0f * PI / 180.0f; // Pernas flexionadas
        fightStance.thighRotation[1] = -30.0f * PI / 180.0f;
        fightStance.calfRotation[0] = 30.0f * PI / 180.0f;
        fightStance.calfRotation[1] = 30.0f * PI / 180.0f;
        fight.addKeyframe(fightStance, 0.0f);

        // Soco direito
        Pose punch;
        punch.torsoRotation = 30.0f * PI / 180.0f;
        punch.upperArmRotation[1] = 45.0f * PI / 180.0f; // Braço direito estendido
        punch.forearmRotation[1] = 0.0f;
        punch.upperArmRotation[0] = -90.0f * PI / 180.0f; // Braço esquerdo mantém guarda
        punch.forearmRotation[0] = -90.0f * PI / 180.0f;
        fight.addKeyframe(punch, 0.2f);

        // Volta para posição de guarda
        fight.addKeyframe(fightStance, 0.4f);

        //  perna esquerda
        Pose kick;
        kick.torsoRotation = 60.0f * PI / 180.0f;
        kick.thighRotation[0] = 90.0f * PI / 180.0f;  // Perna esquerda levantada
        kick.calfRotation[0] = 0.0f;                  // Perna esticada
        kick.thighRotation[1] = -45.0f * PI / 180.0f; // Perna direita como base
        kick.calfRotation[1] = 45.0f * PI / 180.0f;
        fight.addKeyframe(kick, 0.6f);

        // Retorno à posição de guarda
        fight.addKeyframe(fightStance, 1.0f);

        addAnimation(fight);
    }
    // Cria algumas animações predefinidas
    void createDefaultAnimations()
    {

        Animation walk("walk", true);

        Pose pose1; // Pose inicial da caminhada
        pose1.upperArmRotation[0] = 45.0f * (3.14159f / 180.0f);
        pose1.upperArmRotation[1] = -45.0f * (3.14159f / 180.0f);
        pose1.thighRotation[0] = -30.0f * (3.14159f / 180.0f);
        pose1.thighRotation[1] = 30.0f * (3.14159f / 180.0f);
        walk.addKeyframe(pose1, 0.0f);

        Pose pose2; // Pose intermediária
        pose2.upperArmRotation[0] = -45.0f * (3.14159f / 180.0f);
        pose2.upperArmRotation[1] = 45.0f * (3.14159f / 180.0f);
        pose2.thighRotation[0] = 30.0f * (3.14159f / 180.0f);
        pose2.thighRotation[1] = -30.0f * (3.14159f / 180.0f);
        walk.addKeyframe(pose2, 0.5f);

        walk.addKeyframe(pose1, 1.0f); // Volta à pose inicial

        addAnimation(walk);

        Animation jump("jump", false);

        Pose jumpStart; // Agachado
        jumpStart.position.y = 2;
        jumpStart.thighRotation[0] = jumpStart.thighRotation[1] = 45.0f * (3.14159f / 180.0f);
        jumpStart.calfRotation[0] = jumpStart.calfRotation[1] = -90.0f * (3.14159f / 180.0f);
        jump.addKeyframe(jumpStart, 0.0f);

        Pose jumpMiddle; // No ar
        jumpMiddle.position.y = 5;
        jumpMiddle.thighRotation[0] = jumpMiddle.thighRotation[1] = -30.0f * (3.14159f / 180.0f);
        jumpMiddle.upperArmRotation[0] = jumpMiddle.upperArmRotation[1] = -180.0f * (3.14159f / 180.0f);
        jump.addKeyframe(jumpMiddle, 0.5f);

        Pose jumpEnd = jumpStart; // Aterrissagem
        jump.addKeyframe(jumpEnd, 1.0f);

        addAnimation(jump);
    }
    Pose getDefaultPose()
    {
        Pose pose;
        pose.torsoRotation = 0.0f;
        pose.headRotation = 0.0f;
        for (int i = 0; i < 2; i++)
        {
            pose.upperArmRotation[i] = 0.0f;
            pose.forearmRotation[i] = 0.0f;
            pose.thighRotation[i] = 0.0f;
            pose.calfRotation[i] = 0.0f;
        }
        return pose;
    }
};

class Humanoid
{
private:
    MeshBuffer *cubeMesh;
    MatrixStack matrixStack;
    Vec3 position;

    AnimationManager animManager;

    // Ângulos de rotação para cada parte
    float torsoRotation = 0.0f;
    float headRotation = 0.0f;

    // Braços
    float upperArmRotation[2] = {0.0f, 0.0f}; // [0] = esquerdo, [1] = direito
    float forearmRotation[2] = {0.0f, 0.0f};  // [0] = esquerdo, [1] = direito

    // Pernas
    float thighRotation[2] = {0.0f, 0.0f}; // [0] = esquerda, [1] = direita
    float calfRotation[2] = {0.0f, 0.0f};  // [0] = esquerda, [1] = direita

public:
    Humanoid()
    {
        cubeMesh = CreateCube();
        animManager.createDefaultAnimations();
        animManager.createDanceAnimation();
        animManager.createFighterAnimation();
        animManager.playAnimation("jump");
    }

    ~Humanoid()
    {
        if (cubeMesh)
        {
            delete cubeMesh;
        }
    }

    void render(Shader &shader)
    {
        matrixStack.identity();

        matrixStack.translate(position.x,position.y,position.z);

        // Rotação principal do torso
        matrixStack.rotateY(torsoRotation);

        // Torso - todas as outras partes serão relativas a ele
        matrixStack.push();
        matrixStack.scale(1.0f, 1.6f, 0.5f);
        renderCube(shader, matrixStack.top(), Color(45, 100, 25));
        matrixStack.pop();

        // Cabeça - relativa ao torso
        matrixStack.push();
        matrixStack.translate(0.0f, 1.4f, 0.0f);
        matrixStack.rotateY(headRotation);
        matrixStack.scale(0.7f, 0.7f, 0.7f);
        renderCube(shader, matrixStack.top(), Color(255, 224, 185));
        matrixStack.pop();

        // Braços
        for (int side = 0; side < 2; side++)
        { // 0 = esquerdo, 1 = direito
            float sideSign = (side == 0) ? -1.0f : 1.0f;

            // Upper arm - relativo ao torso
            matrixStack.push();
            matrixStack.translate(sideSign * 0.7f, 0.75f, 0.0f);
            matrixStack.rotateX(upperArmRotation[side]);

            // Renderiza upper arm
            matrixStack.push();
            matrixStack.translate(0.0f, -0.3f, 0.0f);
            matrixStack.scale(0.4f, 0.6f, 0.4f);
            renderCube(shader, matrixStack.top(), Color(255, 224, 185));
            matrixStack.pop();

            // Forearm - relativo ao upper arm
            matrixStack.translate(0.0f, -0.6f, 0.0f);
            matrixStack.rotateX(forearmRotation[side]);
            matrixStack.translate(0.0f, -0.3f, 0.0f);
            matrixStack.scale(0.3f, 0.6f, 0.3f);
            renderCube(shader, matrixStack.top(), Color(255, 224, 185));

            matrixStack.pop();
        }

        // Pernas
        for (int side = 0; side < 2; side++)
        { // 0 = esquerda, 1 = direita
            float sideSign = (side == 0) ? -1.0f : 1.0f;

            // Thigh - relativo ao torso
            matrixStack.push();
            matrixStack.translate(sideSign * 0.3f, -0.85f, 0.0f);
            matrixStack.rotateX(thighRotation[side]);

            // Renderiza thigh
            matrixStack.push();
            matrixStack.translate(0.0f, -0.3f, 0.0f);
            matrixStack.scale(0.4f, 0.8f, 0.4f);
            renderCube(shader, matrixStack.top(), Color::BLUE);
            matrixStack.pop();

            // Calf - relativo ao thigh
            matrixStack.translate(0.0f, -0.6f, 0.0f);
            matrixStack.rotateX(calfRotation[side]);
            matrixStack.translate(0.0f, -0.3f, 0.0f);
            matrixStack.scale(0.35f, 0.6f, 0.35f);
            renderCube(shader, matrixStack.top(), Color::BLUE);

            matrixStack.pop();
        }
    }

    void setTorsoRotation(float angle) { torsoRotation = angle; }
    void setHeadRotation(float angle) { headRotation = angle; }

    void setUpperArmRotation(bool isRight, float angle)
    {
        upperArmRotation[isRight ? 1 : 0] = angle;
    }

    void setForearmRotation(bool isRight, float angle)
    {
        forearmRotation[isRight ? 1 : 0] = angle;
    }

    void setThighRotation(bool isRight, float angle)
    {
        thighRotation[isRight ? 1 : 0] = angle;
    }

    void setCalfRotation(bool isRight, float angle)
    {
        calfRotation[isRight ? 1 : 0] = angle;
    }

    void animate(float deltaTime)
    {

        animManager.update(deltaTime);
        Pose currentPose = animManager.getCurrentPose();

        // Aplica a pose atual ao modelo
        setTorsoRotation(currentPose.torsoRotation);
        setHeadRotation(currentPose.headRotation);
        if (animManager.CurrentAnimation()=="jump")
        {
        position = currentPose.position;
        } else 
        {
            position.y=2;
        }

        for (int i = 0; i < 2; i++)
        {
            setUpperArmRotation(i == 1, currentPose.upperArmRotation[i]);
            setForearmRotation(i == 1, currentPose.forearmRotation[i]);
            setThighRotation(i == 1, currentPose.thighRotation[i]);
            setCalfRotation(i == 1, currentPose.calfRotation[i]);
        }
        // const float animSpeed = 2.0f;
        // const float maxRotation = 45.0f * (3.14159f / 180.0f);

        // float cycle = sin(deltaTime * animSpeed);

        // // Braços e pernas em movimento alternado
        // setUpperArmRotation(true, -cycle * maxRotation);
        // setUpperArmRotation(false, cycle * maxRotation);

        // setForearmRotation(true, -abs(cycle) * maxRotation * 0.5f);
        // setForearmRotation(false, -abs(cycle) * maxRotation * 0.5f);

        // setThighRotation(true, cycle * maxRotation);
        // setThighRotation(false, -cycle * maxRotation);

        // setCalfRotation(true, abs(cycle) * maxRotation * 0.5f);
        // setCalfRotation(false, abs(cycle) * maxRotation * 0.5f);
    }
    void playAnimation(const std::string &name)
    {
        animManager.playAnimation(name);
    }

private:
    void renderCube(Shader &shader, const Mat4 &modelMatrix, const Color &color)
    {
        shader.SetMatrix4("model", modelMatrix.m);
        float r = color.r / 255.0f;
        float g = color.g / 255.0f;
        float b = color.b / 255.0f;

        shader.SetFloat("difusse", r, g, b);
        cubeMesh->Render(static_cast<int>(PrimitiveType::TRIANGLES), 36);
    }
};

int main(int argc, char *argv[])
{
    Device *window = Device::GetInstance();

    bool ABORT = false;

    if (!window->Init("HumanGL BY Luis Santos AK DJOKER", 800, 600))
    {
        return 1;
    }

    RenderBatch batch;
    batch.Init(1, 1024);
    Shader shader;
    Shader shaderCube;
    Font font;
    MeshBuffer *cube = CreateCube();

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


    float time = 0.0f;

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

        //  cube->Render(static_cast<int>(PrimitiveType::TRIANGLES), 36);

        human.render(shaderCube);

        time += delta * 0.00001f; // Aproximadamente 60 FPS

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
    cube->Release();
    delete cube;
    font.Release();
    batch.Release();
    shader.Release();
    shaderCube.Release();

    Device::DestroyInstance();
    return 0;
}