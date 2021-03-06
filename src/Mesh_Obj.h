#ifndef _MESH_OBJ_H
#define _MESH_OBJ_H

struct vec2
{
    float x, y;
};
struct vec3
{
    float x, y, z;
};
struct obj_face
{
    unsigned int vert[3];
    unsigned int normal[3];
    unsigned int uv[3];
    unsigned char numverts;
};
struct obj_group
{
    obj_face faces[8192];
    unsigned int numFaces;
    char name[256];
    obj_group();
};

class Mesh_Obj
{
public:
    //  vec3 BBox[2];
    float bbox_min_x;
    float bbox_min_y;
    float bbox_min_z;
    float bbox_max_x;
    float bbox_max_y;
    float bbox_max_z;
    vec3 verts[8192];
    vec2 uvs[8192];
    obj_group groups[8];
    unsigned int numverts;
    unsigned int numnormals;
    unsigned int numuvs;
    unsigned int numgroups;
    Mesh_Obj();
    ~Mesh_Obj();
    int Load(const char* filename);
};

#endif
