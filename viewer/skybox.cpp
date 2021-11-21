#include "skybox.h"

GLfloat skybox::vertices[] = {
	50.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	50.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
};

GLuint skybox::indices[] = {
	0, 1, 3,
	1, 2, 3
};

void skybox::load_images()
{
	img_box[0] = new Texture(tpath);
	img_box[0]->LoadTexture();

	//img_box[1] = new Texture(bpath);
	//img_box[1]->LoadTexture();

	img_box[2] = new Texture(lpath);
	img_box[2]->LoadTexture();

	img_box[3] = new Texture(rpath);
	img_box[3]->LoadTexture();

	img_box[4] = new Texture(fpath);
	img_box[4]->LoadTexture();

	img_box[5] = new Texture(bcpath);
	img_box[5]->LoadTexture();
}

void skybox::LoadData()
{
	load_images();

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

void skybox::draw(int i)
{
	img_box[i]->BindTexture();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void skybox::Cube(glm::vec3 &bias, player &p1, shader &shad)
{
	glm::vec3 scale(3.7f, 3.7f, 3.7f);
	glm::vec3 rotate(1.0f, 0.0f, 0.0f); // 0 1 0

	//SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);//bottom
	//this->draw(1);
	//SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);

	bias.y += 60;
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);//top
	this->draw(0);
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);
	bias.y -= 60;

	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 1);//left
	this->draw(2);
	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 0);

	bias.z += 60;
	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 1);//right
	this->draw(3);
	SetShaderModification(p1, shad, bias, scale, rotate, 0.0f, 0);
	bias.z -= 60;

	rotate.x = 0.0f; rotate.y = 1.0f; bias.z += 60;
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);// front
	this->draw(4);
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);

	bias.x += 60;
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 1);// back
	this->draw(5);
	SetShaderModification(p1, shad, bias, scale, rotate, 90.0f, 0);
	bias.x -= 60; bias.z -= 60;
}