#include "text.h"

unsigned int textTextureId;
unsigned int textVertexBufferId;
unsigned int textUVBufferId;
unsigned int textShaderId;
unsigned int textUniformId;

void initText(const char * texturePath){

	// Initialize texture
	textTextureId = loadTexturesForText(texturePath);

	// Initialize VBO
	glGenBuffers(1, &textVertexBufferId);
	glGenBuffers(1, &textUVBufferId);

	// Initialize Shader
	textShaderId = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");

	// Initialize uniforms' IDs
	textUniformId = glGetUniformLocation(textShaderId, "myTextureSampler");

}

void printText(const char * text, int x, int y, int size){

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i<length; i++){

		glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}
	glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, textUVBufferId);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	// Bind shader
	glUseProgram(textShaderId);

	// Bind texture
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textTextureId);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(textUniformId, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, textVertexBufferId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textUVBufferId);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnable(GL_ALPHA_TEST); glAlphaFunc(GL_GREATER, 0.3f);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisable(GL_ALPHA_TEST);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void cleanupText(){

	// Delete buffers
	glDeleteBuffers(1, &textVertexBufferId);
	glDeleteBuffers(1, &textUVBufferId);

	// Delete texture
	glDeleteTextures(1, &textTextureId);

	// Delete shader
	glDeleteProgram(textShaderId);
}

void timerUp(float &i)
{
	i -= 0.1;
}

std::string convert(float time)
{
	int temp = (int)time;
	std::stringstream ss;
	ss << temp;
	std::string tempStr;
	ss >> tempStr;
	return tempStr;
}