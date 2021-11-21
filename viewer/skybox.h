#pragma once
#include "MST.h"
#include "shaders.h"
#include "Player.h"

class skybox{
public:
	static GLfloat vertices[];
	static GLuint indices[];
	const char *tpath, *bpath, *lpath, *rpath, *fpath, *bcpath;
	GLuint VBO, VAO, EBO;
	Texture *img_box[6];

    explicit skybox(const char *tp, const char *bp, const char *lp, const char *rp, const char *fp, const char *bcp) : 
	   tpath(tp), bpath(bp), lpath(lp), rpath(rp), fpath(fp), bcpath(bcp) {};

	void load_images();
	void LoadData();
	void draw(int i);
	void Cube(glm::vec3 &bias, player &p1, shader &shad);
};