#include "util/images.h"

#define DEFAULT_TEXTURE_COUNT 16

typedef struct FILENAMETEXTUREPAIR
{
	char *filename;
	GLuint texture;
} FilenameTexturePair;

static FilenameTexturePair *textures = NULL;
static int numTextures = 0;
static int numAllocatedPairs = 0;

void imageLoaderInit()
{
	numAllocatedPairs = DEFAULT_TEXTURE_COUNT;
	textures = (FilenameTexturePair*)malloc(sizeof(FilenameTexturePair) * numAllocatedPairs);
}

void imageLoaderKill()
{
	// deallocate OpenGL textures

	// clear memory used by pairs
}

GLuint loadImage(const char *filename)
{
	GLuint result = 0;
	int index = -1;
	int start = 0;
	int end = numTextures;
	int mid = start + (end - start) / 2;
	int compare;

	// simple binary search by texture filename
	while(index == -1 && start >= end)
	{
		compare = strcmp(filename, textures[mid].filename);
		if(compare < 0)
		{
			end = mid - 1;
		}
		else if(compare > 0)
		{
			start = mid + 1;
		}
		else
		{
			index = mid;
		}
	}

	// do we already have the texture in memory?
	if(index >= 0)
	{
		result = textures[index].texture;
	}
	else
	{
		result = loadOpenGLTexture(filename);
		if(result > 0)
		{
			addTexturePair(filename, result);
		}
		else
		{
			fprintf(stderr, "loadImage() could not load image '%s'\n", filename);
		}
	}

	return result;
}
