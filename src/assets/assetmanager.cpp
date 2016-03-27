#include "assets/assetmanager.h"

#include <soil/soil.h>

#include <gl/glew.h>

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <winbase.h>
using namespace std;

map<string, GLuint> AssetManager::textures;

GLuint AssetManager::getTexture(std::string name, bool mipmapping, bool wrap, bool flipVertical)
{
    int width;
    int height;
    unsigned char *data;
    unsigned char *temp;
    int i;
    GLuint result = 0;

	if(name.length() > 0)
	{
		if(textures.find(name) != textures.end())
		{
			result = textures[name];
		}
		else
		{
			// much faster than using SOIL_load_OGL_texture(), plus this doesn't use any deprecated functionality that is illegal in 3.2 core contexts
			data = SOIL_load_image(name.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

			// make sure the load was successful
			if(data)
			{
				// if the pixel data is flipped vertically, so we need to flip it back; this uses an in-place reversal
				if(flipVertical)
				{
					temp = new unsigned char[width * 4];												// enough space for one row of RGBA pixels
					for(i = 0; i < height / 2; i ++)
					{
						memcpy(temp, &data[i * width * 4], (width * 4));								// copy row into temp array
						memcpy(&data[i * width * 4], &data[(height - i - 1) * width * 4], (width * 4));	// copy other side of array into this row
						memcpy(&data[(height - i - 1) * width * 4], temp, (width * 4));					// copy temp into other side of array
					}
					delete[] temp;
				}

				// we can generate a texture object since we had a successful load
				glGenTextures(1, &result);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, result);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

				// some texture UVs should not wrap
				if(wrap)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				}

				// configure minification and magnification filters for this texture
				if(mipmapping)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				}

				// release the memory used to perform the loading
				SOIL_free_image_data(data);

				// store the resulting texture and stop the load timer
				textures[name] = result;
			}
			else
			{
				cout << "AssetManager::getTexture could not load '" << name << "'" << endl;
				result = 0;
			}
		}
    }

    return result;
}
