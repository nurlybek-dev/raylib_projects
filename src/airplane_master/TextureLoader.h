#ifndef _AIRPLANE_MASTER_TEXTURE_LOADER_H_
#define _AIRPLANE_MASTER_TEXTURE_LOADER_H_

#include <map>
#include <string>

#include "raylib.h"
#if !defined(RAYGUI_IMPLEMENTATION)
    #include "raygui.h"
#endif

namespace AirplaneMaster
{
    class TextureLoader  {
        public:
            static Texture2D GetTexture(std::string name)
            {
                if(textures.count(name) > 0) {
                    return textures[name];
                }
                const char *currentDir = GetWorkingDirectory();
                const char *texturePath = TextFormat("%s/resources/Airplane Master/%s", currentDir, name.c_str()); 
                Texture2D texture = LoadTexture(texturePath);
                textures[name] = texture;
                return textures[name];
            }

            static void Release()
            {
                for (const auto& [key, texture] : textures) { 
                    UnloadTexture(texture);
                }
                textures.clear();
            }
        private:
            inline static std::map<std::string, Texture2D> textures;
    };
} // namespace AirplaneMaster


#endif