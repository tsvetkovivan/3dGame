#include "shaders.h"
#include <stdio.h>

glm::mat4 model, projection, view;

const char *pVS = "\n\
				  #version 330 core\n\
				  layout(location = 0) in vec3 aPos;\n\
				  layout(location = 1) in vec3 aNormal;\n\
				  layout(location = 2) in vec2 aTexCoords;\n\
				  \n\
				  out vec2 TexCoords;\n\
				  \n\
				  uniform mat4 model;\n\
				  uniform mat4 view;\n\
				  uniform mat4 projection;\n\
				  \n\
				  void main()\n\
				  {\n\
				  	TexCoords = aTexCoords;\n\
						gl_Position = projection * view * model * vec4(aPos, 3.0);\n\
						}";

const char *pFS = "\n\
				  #version 330 core\n\
				  out vec4 FragColor;\n\
				  \n\
				  in vec2 TexCoords;\n\
				  \n\
				  uniform sampler2D texture_diffuse1;\n\
				  \n\
				  void main()\n\
				  {\n\
				  	FragColor = texture2D(texture_diffuse1, TexCoords.xy);\n\
					}";

shader::shader(){
	m_shaderProg = 0;
}

shader::~shader(){
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++){
		glDeleteShader(*it);
	}

	if (m_shaderProg != 0){
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool shader::Init(){
	m_shaderProg = glCreateProgram();

	if (m_shaderProg == 0){
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}

	return true;
}

bool shader::AddShader(GLenum ShaderType, const char* pShaderText){
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0){
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		return false;
	}

	// Сохраним объект шейдера - он будет удален в декструкторе
	m_shaderObjList.push_back(ShaderObj);

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success){
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		return false;
	}

	glAttachShader(m_shaderProg, ShaderObj);

	return true;
}

bool shader::Finalize(){
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_shaderProg);

	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (Success == 0){
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		return false;
	}

	// Удаляем промежуточные объекты шейдеров, которые были добавлены в программу
	for (ShaderObjList::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++){
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return true;
}

void shader::Enable(){
	glUseProgram(m_shaderProg);
}

GLint shader::GetUniformLocation(const char* pUniformName){
	GLint Location = glGetUniformLocation(m_shaderProg, pUniformName);

	if (Location == 0xFFFFFFFF){
		fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
	}

	return Location;
}

void shader::Setup(){
	if (!Init()) { ConsoleLog("error init shader..."); }
	else { ConsoleLog("init shader"); }

	if (!AddShader(GL_VERTEX_SHADER, pVS)) { ConsoleLog("error init vertex shader"); }
	else { ConsoleLog("init vertex shader"); }

	if (!AddShader(GL_FRAGMENT_SHADER, pFS)) { ConsoleLog("error init fragment shader"); }
	else { ConsoleLog("init fragment shader"); }

	if (!Finalize()) { ConsoleLog("incorectly full init shader"); }
	else { ConsoleLog("successful full init shader"); }
}

void shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProg, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void SetShaderModification(player temp, shader &temps, glm::vec3 bias, glm::vec3 scale, glm::vec3 angleRot, float angle, int mode)
{
	glm::mat4 projection = glm::perspective(glm::radians(90.0), 1.0, 0.1, 500.0);
	glm::mat4 view = temp.GetView();
	temps.setMat4("projection", projection);
	temps.setMat4("view", view);

	glm::mat4 model;

	if (mode == 1){
		model = glm::translate(model, glm::vec3(bias.x, bias.y, bias.z));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(angleRot.x, angleRot.y, angleRot.z));
	}
	else if (mode == 0){
		model = glm::rotate(model, glm::radians(-angle), glm::vec3(angleRot.x, angleRot.y, angleRot.z));
		model = glm::translate(model, glm::vec3(-bias.x, -bias.y, -bias.z));
	}

	model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));
	temps.setMat4("model", model);
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}