#pragma once

#include <string>
#include <map>
#include "Core.hpp"

struct Vertex
{
    float position[3];
};

MeshBuffer *CreateCube();

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

    
    Pose getPoseAtTime(float time);
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

    void playAnimation(const std::string &name, bool shouldLoop = true, bool resetTime = true);

    void update(float deltaTime);

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

    Pose getCurrentPose();
    void createDanceAnimation();
    void createFighterAnimation();
    void createDefaultAnimations();
    Pose getDefaultPose();

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

    void render(Shader &shader);
    

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

    void animate(float deltaTime);
   
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
