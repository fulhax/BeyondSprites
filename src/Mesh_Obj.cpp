#include "Mesh_Obj.h"
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

obj_group::obj_group()
{
	numFaces=0;
	numTris=0;
	for(int i=0;i<256;i++)
		name[i]=0;

}
Mesh_Obj::Mesh_Obj()
{


	
	bbox_max_x=FLT_MIN;
	bbox_max_y=FLT_MIN;
	bbox_max_z=FLT_MIN;
	bbox_min_x=FLT_MAX;
	bbox_min_y=FLT_MAX;
	bbox_min_z=FLT_MAX;
	numverts=0;
	numnormals=0;
	numuvs=0;
	numgroups=0;
}
Mesh_Obj::~Mesh_Obj()
{

}
#define FACETYPE_POS			0
#define FACETYPE_POSUV			1
#define FACETYPE_POSNORMAL		2
#define FACETYPE_POSUVNORMAL	3
#define FACETYPE_UNKNOWN		255

int Mesh_Obj::Load(char *filename)
{
	printf("starting to load:%s\n",filename);
	/*UpdateTime();
	float timestartofload=timesincestart;*/
	FILE *f=fopen(filename,"rb");
	
	if(!f)
		return 0;
	fseek(f,0,SEEK_END);
	size_t end=ftell(f);
	fseek(f,0,SEEK_SET);
	char line[256];
	size_t pos=0;
	int groupnum=-1;
	unsigned char facetype=FACETYPE_UNKNOWN;
	obj_group *g;
	numverts=0;
	numgroups=0;
	numnormals=0;
	numuvs=0;
	int lastpro=-100;
	while(pos=ftell(f),fgets(line,256,f),feof(f)==0)
	{
		if((line[0]=='v')  && line[1]==' ')
		{
			sscanf(line,"v %f %f %f",&verts[numverts].x,&verts[numverts].y,&verts[numverts].z);
			if(verts[numverts].x>bbox_max_x)
				bbox_max_x=verts[numverts].x;
			if(verts[numverts].y>bbox_max_y)
				bbox_max_y=verts[numverts].y;
			if(verts[numverts].z>bbox_max_z)
				bbox_max_z=verts[numverts].z;

			if(verts[numverts].x<bbox_min_x)
				bbox_min_x=verts[numverts].x;
			if(verts[numverts].y<bbox_min_y)
				bbox_min_y=verts[numverts].y;
			if(verts[numverts].z<bbox_min_z)
				bbox_min_z=verts[numverts].z;
			numverts++;
		}
		if((line[0]=='v') && (line[1]=='t')  && line[2]==' ')
		{
			sscanf(line,"vt %f %f",&uvs[numuvs].x,&uvs[numuvs].y);
			numuvs++;
		}
		if((line[0]=='g')  && line[1]==' ')
		{	
			groupnum++;
			groups[groupnum].numFaces=0;
			groups[groupnum].numTris=0;
			sscanf(line,"g %s",&groups[groupnum].name);
			g=&groups[groupnum];
			numgroups++;
		}
		if((line[0]=='o')  && line[1]==' ')
		{	
			groupnum++;
			groups[groupnum].numFaces=0;
			groups[groupnum].numTris=0;
			sscanf(line,"g %s",&groups[groupnum].name);
			g=&groups[groupnum];
			numgroups++;
		}
		if((line[0]=='f')  && line[1]==' ')
		{
			if(groupnum==-1)
			{
				groupnum++;
				groups[groupnum].numFaces=0;
				groups[groupnum].numTris=0;
				sprintf(groups[groupnum].name,"defaultgroup");
				g=&groups[groupnum];
				numgroups++;
			}
			if(facetype==FACETYPE_UNKNOWN)
			{
				int tmp[3];
				if(strstr(line,"//"))
					facetype=FACETYPE_POSNORMAL;
				else if(sscanf(line, "f %i/%i/%i ", &tmp[0], &tmp[1], &tmp[2])==3)
					facetype=FACETYPE_POSUVNORMAL;				
				else if(sscanf(line, "f %i/%i ", &tmp[0], &tmp[1])==2)
					facetype=FACETYPE_POSUV;
				else 
					facetype=FACETYPE_POS;
			}
			obj_face &face=g->faces[g->numFaces];
			switch( facetype)
			{
			case FACETYPE_POSUVNORMAL:
				{
					face.numverts=sscanf(line,"f %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i",
						&face.vert[0],&face.uv[0],&face.normal[0],
						&face.vert[1],&face.uv[1],&face.normal[1],
						&face.vert[2],&face.uv[2],&face.normal[2],
						&face.vert[3],&face.uv[3],&face.normal[3]
						)/3;
						face.vert[0]--; 
						face.vert[1]--;
						face.vert[2]--;
						face.vert[3]--;
						face.uv[0]--;
						face.uv[1]--;
						face.uv[2]--;
						face.uv[3]--;
						face.normal[0]--;
						face.normal[1]--;
						face.normal[2]--;
						face.normal[3]--;
					break;
				}
			case FACETYPE_POSUV:
				{
					face.numverts=sscanf(line,"f %i/%i %i/%i %i/%i %i/%i",
						&face.vert[0],&face.uv[0],
						&face.vert[1],&face.uv[1],
						&face.vert[2],&face.uv[2],
						&face.vert[3],&face.uv[3]
						)/2;
						face.vert[0]--; 
						face.vert[1]--;
						face.vert[2]--;
						face.vert[3]--;
						face.uv[0]--;
						face.uv[1]--;
						face.uv[2]--;
						face.uv[3]--;
					break;
				}
			case FACETYPE_POSNORMAL:
				{
					face.numverts=sscanf(line,"f %i//%i %i//%i %i//%i %i//%i",
						&face.vert[0],&face.normal[0],
						&face.vert[1],&face.normal[1],
						&face.vert[2],&face.normal[2],
						&face.vert[3],&face.normal[3]
						)/2;
						face.vert[0]--; 
						face.vert[1]--;
						face.vert[2]--;
						face.vert[3]--;
						face.normal[0]--;
						face.normal[1]--;
						face.normal[2]--;
						face.normal[3]--;
					break;
				}
			case FACETYPE_POS:
				{
					face.numverts=sscanf(line,"f %i %i %i %i",
						&face.vert[0],
						&face.vert[1],
						&face.vert[2],
						&face.vert[3]
						);
						face.vert[0]--; 
						face.vert[1]--;
						face.vert[2]--;
						face.vert[3]--;
					break;
				}
			}
			g->numFaces++;
			g->numTris++;
			if(face.numverts==3)
				g->numTris++;

		}
		int pro=pos/(end/1000);
		if(pro!=lastpro)
		{
			lastpro=pro;
			printf("\rloading obj:%i.%i%%  ",pro/10,pro%10);
		}
	}
	printf("\rloading obj:100%%       \n");
	fclose(f);
	/*UpdateTime();
	printf("done in %f seconds\n",timesincestart-timestartofload);*/
return 1;
}

