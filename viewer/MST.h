#pragma once
#include  "shaders.h"

#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_OGL_VALUE 0xFFFFFFFF
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

GLuint loadTexturesForText(const char *path);

struct Vertex {
	glm::vec3 coordV, coordN;
	glm::vec2 coordT;

	Vertex() : coordV(), coordT(), coordN() {};
	Vertex(float a, float b, float c, float d, float e, float f, float g, float k) :
		coordV(a, b, c), coordT(d, e), coordN(f, g, k) {};
};

class Texture {
public:
	GLuint id;
	std::string fileName;
	Texture(const char * path);
	Texture() {};
	void LoadTexture();
	void BindTexture();
	void ShowTexture();
	void loadPicture();
};

class Mesh {
private:
	void InitFromScene(const aiScene *pScene, const std::string &FileName);
	void InitMesh(unsigned int Index, const aiMesh *paiMesh);
	void InitMaterials(const aiScene *pScene, const std::string &FileName);
	void Clear();

	struct MeshEntry {
		MeshEntry();
		~MeshEntry();
		void Init(const std::vector<Vertex> &Vertices, const std::vector<unsigned int> &Indices);

		GLuint VB;
		GLuint IB;
		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
	bool transparency;
public:
	glm::vec3 scale;
	explicit Mesh(glm::vec3 sc) : scale(sc) { transparency = false; };
	~Mesh() { Clear(); }
	void LoadMesh(const std::string &FileName);
	void Render();
};