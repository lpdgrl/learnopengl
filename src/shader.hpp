#include <glad.h>
#include <string>
#include <glm/gtc/type_ptr.hpp>
// TODO: 
//      1. private method print errors of compile and link shader's

class Shader 
{
    public:
        // the program ID
        unsigned int ID;

        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);
        
        // use/activate the shader
        void use();

        // utility uniform functions 
        void setBool (const std::string &name, bool value) const;
        void setInt (const std::string &name, int value) const;
        void setFloat (const std::string &name, float value) const;
};
