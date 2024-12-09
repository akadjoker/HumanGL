
#include "Animation.hpp"

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

    //  índices para desenhar as faces do cubo (36 índices: 6 faces * 2 triângulos * 3 vértices)
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


Pose Animation::getPoseAtTime(float time)
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

    // Interpolacao linear
    size_t prevFrame = nextFrame - 1;
    float frameDuration = keyframes[nextFrame].time - keyframes[prevFrame].time;
    float t = (time - keyframes[prevFrame].time) / frameDuration;

    return Pose::lerp(keyframes[prevFrame].pose, keyframes[nextFrame].pose, t);
}

void AnimationManager::playAnimation(const std::string &name, bool shouldLoop, bool resetTime )
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

void AnimationManager::update(float deltaTime)
{
    if (!playing)
        return;

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

void AnimationManager::createDanceAnimation()
{
    Animation dance("dance", true);
    const float PI = 3.14159f;

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
    pose2.thighRotation[1] = -20.0f * PI / 180.0f;
    dance.addKeyframe(pose2, 0.5f);

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

void AnimationManager::createFighterAnimation()
{
    Animation fight("fight", true);
    const float PI = 3.14159f;

    Pose fightStance;
    fightStance.torsoRotation = 45.0f * PI / 180.0f;
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

void AnimationManager::createDefaultAnimations()
{

    Animation walk("walk", true);

    Pose pose1; // Pose inicial
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

    Pose jumpStart;
    jumpStart.position.y = 2;
    jumpStart.thighRotation[0] = jumpStart.thighRotation[1] = 45.0f * (3.14159f / 180.0f);
    jumpStart.calfRotation[0] = jumpStart.calfRotation[1] = -90.0f * (3.14159f / 180.0f);
    jump.addKeyframe(jumpStart, 0.0f);

    Pose jumpMiddle;
    jumpMiddle.position.y = 5;
    jumpMiddle.thighRotation[0] = jumpMiddle.thighRotation[1] = -30.0f * (3.14159f / 180.0f);
    jumpMiddle.upperArmRotation[0] = jumpMiddle.upperArmRotation[1] = -180.0f * (3.14159f / 180.0f);
    jump.addKeyframe(jumpMiddle, 0.5f);

    Pose jumpEnd = jumpStart;
    jump.addKeyframe(jumpEnd, 1.0f);

    addAnimation(jump);
}
Pose AnimationManager::getDefaultPose()
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

Pose AnimationManager::getCurrentPose()
{
    if (!playing || animations.find(currentAnimation) == animations.end())
    {
        return Pose();
    }
    return animations[currentAnimation].getPoseAtTime(currentTime);
}

void Humanoid::render(Shader &shader)
{
    matrixStack.identity();

    matrixStack.translate(position.x, position.y, position.z);

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

void Humanoid::animate(float deltaTime)
{

    animManager.update(deltaTime);
    Pose currentPose = animManager.getCurrentPose();

    // Aplica a pose atual ao modelo
    setTorsoRotation(currentPose.torsoRotation);
    setHeadRotation(currentPose.headRotation);
    if (animManager.CurrentAnimation() == "jump")
    {
        position = currentPose.position;
    }
    else
    {
        position.y = 2;
    }

    for (int i = 0; i < 2; i++)
    {
        setUpperArmRotation(i == 1, currentPose.upperArmRotation[i]);
        setForearmRotation(i == 1, currentPose.forearmRotation[i]);
        setThighRotation(i == 1, currentPose.thighRotation[i]);
        setCalfRotation(i == 1, currentPose.calfRotation[i]);
    }
}