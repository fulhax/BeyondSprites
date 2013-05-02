#ifndef _MESH_OBJ_H
#define _MESH_OBJ_H

#include <GL/glfw.h>
/*
struct obj_vec3f
{
	float x,y,z;
};

struct obj_vec2f
{
	float x,y;
};*/
struct vec2
{
float x,y;
};
struct vec3
{
    float x,y,z;
};
struct obj_face
{
	unsigned int vert[4];
	unsigned int normal[4];
	unsigned int uv[4];
	unsigned char numverts;
};
struct obj_group
{	
	obj_face faces[512];
	unsigned int numFaces;
	unsigned int numTris;
	char name[256];
	obj_group();
};

class Mesh_Obj
{
public:
//	vec3 BBox[2];
	float bbox_min_x;
	float bbox_min_y;
	float bbox_min_z;
	float bbox_max_x;
	float bbox_max_y;
	float bbox_max_z;
	vec3 verts[512];
	vec2 uvs[512];
	obj_group groups[8];
	unsigned int numverts;
	unsigned int numnormals;
	unsigned int numuvs;
	unsigned int numgroups;
	Mesh_Obj();
	~Mesh_Obj();
	int Load(char *filename);
};

#endif