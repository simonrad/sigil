#pragma once

#include "../util/transform.h"
#include "../util/shader.h"

extern Shader *sliBasicShader;
extern Shader *sliPointShader;
extern Shader *sliTextureShader;
extern Shader *sliTextShader;

void sliShadersInit(Mat4 *projection);
void sliShadersDestroy();
