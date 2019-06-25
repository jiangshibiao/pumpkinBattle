#include "model.h"

vector<OurModel*> ModelManager;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;
	printf("%s\n", filename.c_str());
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

OurModel::OurModel(//centerpos is the difference between pos and actual bottom-right-down
			 //z is front, x is right, y is up
	const char *s, Point3D pos, double size, int typ,
	Point3D centerpos, double xlength, double ylength, double zlength,
	Point3D Front, Point3D Left, Point3D Up
) : Model(s), pos(pos), size(size), typ(typ), Front(Front), Left(Left), Up(Up) {
	allface = vector<Point3D>();
	if (sgn(xlength) == 0 && sgn(ylength) == 0 && sgn(zlength) == 0)return;
	Point3D A = Front * centerpos.z + Left * centerpos.x + Up * centerpos.y,
		B = A + Front * zlength, C = B + Left * xlength, D = A + Left * xlength,
		E = A + Up * ylength, F = B + Up * ylength, G = C + Up * ylength, H = D + Up * ylength;
#define	afpb(A, B, C, D) \
	allface.emplace_back(A), allface.emplace_back(B), allface.emplace_back(C), \
	allface.emplace_back(C), allface.emplace_back(D), allface.emplace_back(A);

	afpb(D, C, B, A);
	afpb(A, B, F, E);
	afpb(B, C, G, F);
	afpb(C, D, H, G);
	afpb(D, A, E, H);
	afpb(E, F, G, H);

	//CuboidManager.emplace_back(new Cuboid(A + pos, D + pos, E + pos, B + pos));//for testing
}