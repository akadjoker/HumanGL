#pragma once


#include "Config.hpp"




class  Shader 
{
    public:
           Shader();
            virtual ~Shader();
        

            bool Create(const char* vShaderCode, const char* fShaderCode);
            bool Create(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
            bool Load(const char* vShaderCode, const char* fShaderCode);
            bool Load(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
            bool operator ==(const Shader&      other) const { return m_program == other.m_program; }
            bool operator !=(const Shader&      other) const { return m_program != other.m_program; }


   

        u32 GetID()    {        return m_program;    }

        void Use() const;



   

        void SetInt(const std::string &name, int value) ;
 
        void SetMatrix4(const std::string &name, const float *value) ;
        void SetMatrix3(const std::string &name, const float *value) ;

        void SetFloat(const std::string& name, float v);
        void SetFloat(const std::string& name, float x, float y);
        void SetFloat(const std::string& name, float x, float y, float z);
        void SetFloat(const std::string& name, float x, float y, float z,float w);



        void Release();

        


    void print();

    bool findUniform(const std::string &name)const;

    int  getUniform(const std::string &name);
    int  getAttribute(const std::string &name);


    int  getUniformLocation(const std::string &uniformName) const;
    int  getAttribLocation( const std::string &attribName) const;
    
    bool addUniform(const std::string &name);
    bool addAttribute(const std::string &name);

    bool ContainsUniform(const std::string &name) const;

    void LoadDefaults();

private:
        u32 m_program;
        int m_numAttributes;
        int m_numUniforms;
        int success;

    private:
         void checkCompileErrors(unsigned int shader, const std::string &type);
 
           Shader& operator =(const Shader& other) = delete;
           Shader(const Shader& other) = delete;
          
};