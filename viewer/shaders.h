#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#include "Ad.h"
#include <soil\SOIL.h>

#include <assimp\include\assimp\Importer.hpp>
#include <assimp\include\assimp\scene.h>
#include <assimp\include\assimp\postprocess.h>

#include "Player.h"



extern std::string directory;
extern const char *pVS, *pFS;
extern glm::mat4 model, projection, view;

class shader{
private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
public:
	GLuint m_shaderProg;

	bool AddShader(GLenum ShaderType, const char* pShaderText);
	bool Finalize();
	GLint GetUniformLocation(const char* pUniformName);
	shader();
	~shader();
	virtual bool Init();
	void Enable();
	void Setup();
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

void ConsoleLog(const char *name);
void SetShaderModification(player temp, shader &temps, glm::vec3 bias, glm::vec3 scale, glm::vec3 angleRot, float angle, int mode);
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);