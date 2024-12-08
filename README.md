# HumanGL

# 3D Humanoid Animation System

A C++ OpenGL-based 3D humanoid modeling and animation system that provides a flexible framework for creating and animating articulated 3D characters.

## Features

### Mesh System
- Custom vertex buffer management
- Support for multiple vertex attributes (position, normal, color, etc.)
- Primitive types support (triangles, lines)
- Efficient rendering using VAOs and VBOs

### Matrix Stack
- Hierarchical transformation system
- Push/pop matrix operations
- Support for translation, rotation, and scaling
- Quaternion-based rotations

### Humanoid Model
- Fully articulated character model
- Hierarchical body structure:
  - Head
  - Torso
  - Arms (upper arm and forearm)
  - Legs (thigh and calf)
- Proper joint articulation and parenting

### Animation System
- Keyframe-based animation
- Support for multiple animations
- Animation controls:
  - Play/Pause/Resume
  - Speed control
  - Loop control
  - Reset functionality
- Pose interpolation
- Default pose handling


## Installation



2. Build the project:
```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Basic Setup

```cpp
// Create humanoid instance
Humanoid* humanoid = new Humanoid();

// Main loop
while (running) {
    float deltaTime = calculateDeltaTime();
    
    // Update animation
    humanoid->update(deltaTime);
    
    // Render
    humanoid->render(shader);
}
```

### Animation Control

```cpp
// Play an animation
humanoid->playAnimation("walk", true);  // true for looping

// Control playback
humanoid->pauseAnimation();
humanoid->resumeAnimation();
humanoid->resetAnimation();

// Adjust animation speed
humanoid->setAnimationSpeed(2.0f);  // 2x speed
```

### Creating Custom Animations

```cpp
Animation newAnim("custom_animation", true);  // true for looping

// Add keyframes
Pose pose1;
pose1.upperArmRotation[1] = 90.0f * (3.14159f/180.0f);
newAnim.addKeyframe(pose1, 0.0f);

Pose pose2;
pose2.forearmRotation[1] = 45.0f * (3.14159f/180.0f);
newAnim.addKeyframe(pose2, 0.5f);

animManager.addAnimation(newAnim);
```



## Future Improvements

- Smooth transitions between animations
- Animation blending system
- Event system for animation triggers
- Animation saving/loading system
- Additional pre-made animations
- Texture support
- Skeletal animation system
