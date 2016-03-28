#include "util/images.h"

#include <soil.h>

#include <gl/glew.h>

#include <stdio.h>
#include <stdlib.h>

GLuint loadOpenGLTexture(const char *filename)
{
    int width;
    int height;
    unsigned char *data;
    //unsigned char *temp;
    //int i;
    GLuint result = 0;

	// much faster than using SOIL_load_OGL_texture(), plus this doesn't use any deprecated functionality that is illegal in 3.2 core contexts
	data = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

	// make sure the load was successful
	if(data)
	{
		// if the pixel data is flipped vertically, so we need to flip it back; this uses an in-place reversal
		/*
		if(flipVertical)
		{
			temp = (unsigned char*)malloc(sizeof(unsigned char) * width * 4);					// enough space for one row of RGBA pixels
			for(i = 0; i < height / 2; i ++)
			{
				memcpy(temp, &data[i * width * 4], (width * 4));								// copy row into temp array
				memcpy(&data[i * width * 4], &data[(height - i - 1) * width * 4], (width * 4));	// copy other side of array into this row
				memcpy(&data[(height - i - 1) * width * 4], temp, (width * 4));					// copy temp into other side of array
			}
			free(temp);
		}*/

		// we can generate a texture object since we had a successful load
		glGenTextures(1, &result);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, result);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// we want textures to be wrappable
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// we also want mipmapping on for maximum prettiness
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// release the memory used to perform the loading
		SOIL_free_image_data(data);
	}
	else
	{
		fprintf(stderr, "loadOpenGLTexture() could not load '%s'\n", filename);
		exit(1);
	}

    return result;
}
