#pragma once


#include "Config.hpp"
#include "Pixmap.hpp"
#include "Texture.hpp"



enum WrapMode
{
	ClampToEdge       = 0x812F,
	ClampToBorder     = 0x812D,
	MirroredRepeat    = 0x8370,
	Repeat            = 0x2901 // Default
	
};



enum FilterMode
{
	Nearest           = 0x2600,
	Linear            = 0x2601,
	NearestMipNearest = 0x2700,
	LinearMipNearest  = 0x2701,
	NearestMipLinear  = 0x2702, // This is the default setting
	LinearMipLinear   = 0x2703
};




class  Texture 
{
    public:
        Texture();
        virtual ~Texture();


           
        u32 GetID() {return id;}

	FilterMode GetMinFilter() const { return MinificationFilter; }
	FilterMode GetMagFilter() const { return MagnificationFilter; }
	WrapMode GetWrapS() const { return HorizontalWrap; }
	WrapMode GetWrapT() const { return VerticalWrap; }

    int GetWidth() {return width;}
    int GetHeight() {return height;}
	
	void SetMinFilter(FilterMode filter);
	void SetMagFilter(FilterMode filter);
	void SetWrapS(WrapMode mode);
	void SetWrapT(WrapMode mode);
	void SetAnisotropicFiltering(float level = -1.0f);

    void Use(u32 unit=0);
    void Update(const Pixmap &pixmap);
    void Update(const unsigned char *buffer, u16 components, int width, int height);

    virtual    void Release();


        
    protected:
        u32 id;   
        WrapMode       HorizontalWrap;
        WrapMode       VerticalWrap;
	    FilterMode      MinificationFilter;
	    FilterMode      MagnificationFilter;
	    float          MaxAnisotropic;
        int width;          
        int height;    

        void createTexture();

        Texture& operator=(const Texture& other) = delete;
        Texture(const Texture& other) = delete;
        Texture(Texture&&) = delete;
        Texture& operator=(Texture&&) = delete;

   



     
};


class   Texture2D  : public Texture
{
    public:
    Texture2D();

    
    Texture2D(int w, int h);
    Texture2D(const Pixmap &pixmap);
    Texture2D(const char* file_name);

    bool Load(const Pixmap &pixmap);
    bool Load(const char* file_name);
    bool LoadFromMemory(const unsigned char *buffer,u16 components, int width, int height);
    u32 GetID() {return id;}

    static Texture2D * GetDefaultTexture();


    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&&) = delete;
    Texture2D& operator=(Texture2D&&) = delete;


    private:
        friend class Texture;
        s32 components{0};     
        static Texture2D * defaultTexture;
     
};
