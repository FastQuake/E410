#include "Box.hpp"
#include "../globals.hpp"
using namespace std;

//Generic box object
Box::Box(glm::vec2 pos, glm::ivec2 size, glm::vec4 colour){

		this->pos = pos;
		this->size = size;
		this->colour = colour;
		magic = GUIBOX_MAGIC;

		visible = true;
		updates = false;
		alive = true;
		locks = false;

		tex = resman.loadTexture("black.png");
}

void Box::draw(ShaderProgram *prg){
	glm::vec2 tpos = pos;
	if(pos.x < 0){
		tpos.x = width + pos.x;
	}
	if(pos.y < 0){
		tpos.y = height + pos.y;
	}
	
	glm::vec2 tScale;
	tScale.x = (float)size.x/width;
	tScale.y = (float)size.y/height;
	tScale *= scale;

	//cout << tScale.x << endl;

	tpos.x = tpos.x/width;
	tpos.y = tpos.y/height;
	tpos.x = remap(tpos.x,0,1,-1,1)+(tScale.x);
	tpos.y = -(remap(tpos.y,0,1,-1,1)+(tScale.y));

	glBindTexture(GL_TEXTURE_2D, tex.id);
	glUniform1i(prg->getUniform("inTexture"),0);
	glUniform2fv(prg->getUniform("pos"),1,glm::value_ptr(tpos));
	glUniform2fv(prg->getUniform("scale"),1,glm::value_ptr(tScale));
	glUniform4fv(prg->getUniform("colour"),1,glm::value_ptr(colour));
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glVertexAttribPointer(
			prg->getAttribute("coord2d"),
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GL_FLOAT)*2,
			0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
