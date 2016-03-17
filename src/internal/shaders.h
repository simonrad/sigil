#pragma once

#include <glm/glm.hpp>

class Shader;

extern Shader *sliBasicShader;
extern Shader *sliPointShader;
extern Shader *sliTextureShader;

void sliShadersInit(glm::mat4 &projection);
void sliShadersDestroy();
