#include "Objects.h"

void Box::set_size()
{
	vertices[0] = vertices[1] = vertices[9] = vertices[24] = size;
}

GLfloat Box::vertices[] = {
	1.5f, 1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 1.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

GLuint Box::indices[] = {  
	0, 1, 3, 
	1, 2, 3  
};

void Box::LoadData()
{
	this->set_size();

	img = new Texture(path);
	img->LoadTexture();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Box::Draw()
{
	img->BindTexture();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Box::Cube(glm::vec3 &bias, player &p1, shader &shad)
{
	glm::vec3 scale(2, 2, 1);
	glm::vec3 rotate(1, 0, 0); // 0 1 0

	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);
	this->Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);
	
	bias.y++;
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);
	this->Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);
	bias.y--;

	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 1);
	this->Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 0);

	bias.z++;
	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 1);
	this->Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 0);
	bias.z--;

	rotate.x = 0.0f; rotate.y = 1.0f; bias.z++;
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);
	this->Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);

	bias.x++;
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);
	this->Draw();
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);
	bias.x--; bias.z--;
}
