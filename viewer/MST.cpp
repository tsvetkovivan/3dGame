#include "MST.h"

GLuint loadTexturesForText(const char *path)
{
	GLuint temp = SOIL_load_OGL_texture(path, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_MIPMAPS);
	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return temp;
}

void ConsoleLog(const char * str)
{
	std::cout << str << std::endl;
}

template<class T> void consoleLog(T a)
{
	std::cout << a << std::endl;
}

void Texture::ShowTexture()
{
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0); glVertex3f(10, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(10, 10, 0);
	glTexCoord2f(0, 1); glVertex3f(0, 10, 0);
	glEnd();
}

Texture::Texture(const char * path)
{
	fileName = path;
}

void Texture::LoadTexture()
{
	id = SOIL_load_OGL_texture(fileName.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_MIPMAPS);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::BindTexture() {
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::loadPicture()
{
	this->BindTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-400, -400, 0);
	glTexCoord2f(1, 0); glVertex3f(400, -400, 0);
	glTexCoord2f(1, 1); glVertex3f(400, 400, 0);
	glTexCoord2f(0, 1); glVertex3f(-400, 400, 0);
	glEnd();
}

void Mesh::LoadMesh(const std::string &FileName)
{
	Clear();

	std::string temp = "model/Tree.obj";
	if (FileName == temp)transparency = true;

	Assimp::Importer importer;
	const aiScene *pScene = importer.ReadFile(FileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (pScene) { InitFromScene(pScene, FileName); }
	else { ConsoleLog("error of loading scene..."); return; }
}

void Mesh::InitFromScene(const aiScene *pScene, const std::string &FileName)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh *paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh);
		ConsoleLog("init mesh");
	}

	InitMaterials(pScene, FileName);
}

void Mesh::InitMesh(unsigned int Index, const aiMesh *paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D *pPos = &(paiMesh->mVertices[i]);
		const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D *pTexcoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(pPos->x, pPos->y, pPos->z,
			pTexcoord->x, pTexcoord->y,
			pNormal->x, pNormal->y, pNormal->z);

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace &Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);

		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices, Indices);
}

void Mesh::InitMaterials(const aiScene *pScene, const std::string &FileName)
{
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial *pMaterial = pScene->mMaterials[i];
		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string FullPath = directory + path.data;

				m_Textures[i] = new Texture(FullPath.c_str());
				m_Textures[i]->LoadTexture();

				ConsoleLog("load material...");
			}
		}
	}
}

void Mesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i - 1);

		if (transparency && i == 1){ glEnable(GL_ALPHA_TEST); glAlphaFunc(GL_GREATER, 0.3f); }

		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coordN));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, coordT));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex])
		{
			m_Textures[MaterialIndex]->BindTexture();
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
		if (transparency && i == 1){ glDisable(GL_ALPHA_TEST); }
	}

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//glActiveTexture(GL_TEXTURE0);
}

Mesh::MeshEntry::MeshEntry()
{
	VB = INVALID_OGL_VALUE;
	IB = INVALID_OGL_VALUE;
	NumIndices = 0;
	MaterialIndex = INVALID_MATERIAL;
}

Mesh::MeshEntry::~MeshEntry()
{
	if (VB != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &VB);
	}

	if (IB != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &IB);
	}
}

void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
	const std::vector<unsigned int>& Indices)
{
	NumIndices = Indices.size();

	glGenBuffers(1, &VB);
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*Vertices.size(),
		&Vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*NumIndices,
		&Indices[0], GL_STATIC_DRAW);
}

void Mesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		SAFE_DELETE(m_Textures[i]);
	}
}
