#pragma once

#include "Config.hpp"
#include "Texture.hpp"
#include "Math.hpp"
#include "Color.hpp"



struct VertexBuffer 
{
    int elementCount;          
    std::vector<float>  vertices;        
    std::vector<float>  texcoords;           
    std::vector<unsigned char> colors;     
    std::vector<unsigned int>  indices;    
    unsigned int vaoId;         
    unsigned int vboId[4];
} ;

struct DrawCall 
{
    int mode;                   
    int vertexCount;          
    int vertexAlignment;       
   unsigned int textureId;
};



class  RenderBatch 
{
public:
        
    RenderBatch( );
    virtual ~RenderBatch();



    void Release();
    

    void Init(int numBuffers, int bufferElements);
 

    void SetColor(const Color &color);
    void SetColor(float r , float g , float b);
    void SetColor(u8 r, u8 g, u8 b, u8 a);


      void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, const Color &color);
      void DrawCircle(int centerX, int centerY, float radius, const Color &color, bool fill = false);

      void DrawRectangle(int posX, int posY, int width, int height, const Color &color,bool fill = false);
      void DrawRectangle(const Rectangle &rect, const Color &color, bool fill = false);
   
      
    

    void Line2D(int startPosX, int startPosY, int endPosX, int endPosY);
    void Line2D(const Vec2 &start,const Vec2 &end);

    void Line3D(const Vec3 &start, const Vec3 &end);
    void Line3D(float startX, float startY, float startZ, float endX, float endY, float endZ);

  
    void Cube(const Vec3 &position, float width, float height, float depth,bool wire=true);
    void Sphere(const Vec3 &position, float radius, int rings, int slices,bool wire=true);
    void Cone(const Vec3& position, float radius, float height, int segments, bool wire);
    void Cylinder(const Vec3& position, float radius, float height, int segments, bool wire);
    void Capsule(const Vec3& position, float radius, float height, int segments, bool wire);


    void Grid(int slices, float spacing,bool axes=true);
    
    void Quad(const Vec2 *coords, const Vec2 *texcoords);
    void Quad(Texture2D *texture, float x, float y,float width, float height);
    void Quad(Texture2D *texture, const FloatRect &src,float x, float y,float width, float height);
    void Quad(u32 texture, float x, float y,float width, float height);

    void BeginTransform(const Mat4 &transform);
    void EndTransform();

   
    void Render();

    void SetMode(int mode);                        
       

          
    void Vertex2f(float x, float y);          
    void Vertex3f(float x, float y, float z);     
    void TexCoord2f(float x, float y);          

    void SetTexture(unsigned int id);


    private:
        bool CheckRenderBatchLimit(int vCount);

    RenderBatch(const RenderBatch&) = delete;
    RenderBatch& operator=(const RenderBatch&) = delete;
    RenderBatch(RenderBatch&&) = delete;
    RenderBatch& operator=(RenderBatch&&) = delete;


    int bufferCount;            
    int currentBuffer;         
    int drawCounter;           
    float currentDepth;         
    int vertexCounter;
    s32 defaultTextureId;
    bool use_matrix;
    Mat4 modelMatrix;
 

   Texture2D m_defaultTexture ;

    std::vector<DrawCall*> draws;
    std::vector<VertexBuffer*> vertexBuffer;

    float texcoordx, texcoordy;         
    u8 colorr, colorg, colorb, colora;

private:




    

};



struct  Glyph
{
    int value;    
    int offsetX;  
    int offsetY;  
    int advanceX;     
} ;

class  Font 
{
    public:

        Font();
        ~Font();


        bool Load(const std::string& filePath);

        void SetClip(int x, int y, int w, int h);
        void EnableClip(bool enable);


        Vec2      GetTextSize(const char *text);
        float     GetTextWidth(const char *text);
        float     GetHeight();
          

        void SetSize(float size);
        void SetSpacing(float spacing);
        void SetColor(const Color &color);
        void SetColor(float r , float g , float b);

        void Print(const char *text, float x, float y);
        void Print(float x, float y, const char *text, ...);
        
        void SetTexture(Texture2D *texture) {this->texture = texture;}
        void SetBatch(RenderBatch *batch) {this->batch = batch;}

        void DrawText(RenderBatch *batch,const char *text, float x, float y);
        void DrawText(RenderBatch *batch,const char *text, float x, float y,const Color &c);
        void DrawText(RenderBatch *batch, float x, float y, const Color &color, const char *text, ...);
        


        bool LoadDefaultFont();

        void Release();

    private:
        friend class RenderBatch;


    struct Character
        {
            int id;
            u16 x;
            u16 y;
            u16 width;
            u16 height;
            u16 xoffset;
            u16 yoffset;
          };


        Font(const Font& other) = delete;
        Font& operator=(const Font& other) = delete;

        bool Reset();


        RenderBatch *batch;
        float fontSize;
        float spacing;
        Vec2 coords[4];
        Vec2 texcoords[4];
        Color color;
        Texture2D *texture;
        bool enableClip;
        IntRect clip;
        int m_baseSize;          
        int m_glyphCount;        
        int m_glyphPadding;    
        int maxHeight;  
   
        std::vector<IntRect> m_recs;  
        std::vector<Glyph> m_glyphs;   
        int textLineSpacing{15};

        void drawTextCodepoint(int codepoint,float x, float y);
        void drawTextCodepoint(RenderBatch *batch,int codepoint, float x, float y);
        int  getGlyphIndex( int codepoint);
        void drawTexture(const FloatRect &src,float x, float y,float w, float h);
        void drawTexture(RenderBatch *batch,const FloatRect &src, float x, float y, float width, float height);

};