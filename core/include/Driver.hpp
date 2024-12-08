#pragma once
#include "Config.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Color.hpp"
#include "Math.hpp"


enum class TRANSFORM_STATE
{
    PROJECTION=0,
    VIEW,
    MODEL
};

enum class CullMode
{
    FRONT=0x0404,
    BACK,
    FRONT_AND_BACK
};



enum class BlendMode
{
    
    BLEND=0,
    ADD,
    MULTIPLY,
    SCREEN,
    SUBTRACT,
    REPLACE,
    LIGHTEN,
    DARKEN,
    LIGHT,
    COUNT
    
};




class    Driver
{
    public:
    

     void SetCubeTexture(u32 unit, u32 texture);
     void SetDepthTest(bool enable);
     void SetDepthWrite(bool enable);
     void SetCullFace(bool enable);
     void SetBlend(bool enable);
     void SetBlendMode(BlendMode blendMode);
     void SetScissorTest(bool enable);
     void SetStencilTest(bool enable);
     void SetColorMask(bool r, bool g, bool b, bool a);
     void SetDepthMask(bool enable);
     void SetStencilMask(bool enable);
     void SetBlendFunc(u32 src, u32 dst);
     void SetCullFaceMode(u32 mode);
     void SetFrontFace(u32 mode);
     void SetDepthFunc(u32 func);
     void SetStencilFunc(u32 func, u32 ref, u32 mask);
     void SetStencilOp(u32 sfail, u32 dpfail, u32 dppass);
     void SetScissor(u32 x, u32 y, u32 width, u32 height);
 
     void SetViewport(u32 x, u32 y, u32 width, u32 height);
     void SetClearColor(u8 r, u8 g, u8 b, u8 a);
     void SetClearColor(f32 r, f32 g, f32 b, f32 a);
 
     void Clear();

     void ResetStats();
     
     void FlipImageOnLoad(bool flip);


     void SetShader(u32 shader);
     void SetShader(Shader *shader);
     
     void SetTexture(Texture2D *texture, u32 unit);
     void SetTextureId(u32 unit, u32 texture);



     u32 GetTotalTriangles();
     u32 GetTotalDrawCalls();
     u32 GetTotalVertex();
     u32 GetTotalTextures();
     u32 GetTotalPrograms();


     void  DrawElements(GLenum mode, GLsizei count, GLenum type,const void *indices);
     void  DrawArrays(GLenum mode, GLint first, GLsizei count);

     void Init();
     void Resize (int width, int height);
     void Release();



     static Driver &Instance();

 private:
     
     Driver();
     virtual ~Driver();

     int m_width;
     int m_height;

     Shader *m_currentShader;
     Texture2D *m_currentTexture[8];
     u32 currentShader;
     u32 currentTexture[8];
     u32 currentCubeTexture[8];
     bool depthTest;
     bool depthWrite;
     bool cullFace;
     bool blend;
     bool scissorTest;
     bool stencilTest;
     bool colorMask[4];
     bool depthMask;
     bool stencilMask;
     bool justDepth;
     u32 blendSrc;
     u32 blendDst;
     u32 cullFaceMode;
     u32 frontFace;
     u32 depthFunc;
     u32 stencilFunc;
     u32 stencilRef;
     u32 stencilMaskRef;
     IntRect scissor;
     IntRect viewport;
     BlendMode currentMode;
     u32 totalTextures;
     u32 totalCubeTextures;
     u32 totalShaders;
     u32 totalTraingles;
     u32 totalDrawCalls;
     u32 totalVertex;

     Color clearColor;
 
     bool stateMode;


};