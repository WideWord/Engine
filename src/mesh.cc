#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"
#include <assimp.h>
#include <cmath>
#include <aiScene.h>
#include <aiMesh.h>

Mesh::Mesh(MeshData& dat) : radius(_radius) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(4, vbo);
	// vbo array: 0 - coor, 1 - nor, 2 - tex, 3 - ind
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, dat.verts * sizeof(float) * 3, dat.coor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, dat.verts * sizeof(float) * 3, dat.nor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, dat.verts * sizeof(float) * 2, dat.tex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dat.faces * sizeof(unsigned) * 3, dat.ind, GL_STATIC_DRAW);
	
	this->faces = dat.faces;
	
	_radius = 0;
	for(int i = 0; i < dat.verts; i++) {
	    float len = sqrt(dat.coor[i * 3] * dat.coor[i * 3] + dat.coor[i * 3 + 1] * dat.coor[i * 3 + 1] + dat.coor[i * 3 + 2] * dat.coor[i * 3 + 2]); 
	    if (len > _radius)_radius = len;
	}
	
	material = nullptr;
}

Mesh::~Mesh () {
	glDeleteBuffers(4, vbo);
	glDeleteVertexArrays(1, &vao);
}


SuperMesh::SuperMesh () : radius(_radius) {
    
}

void SuperMesh::bake () {
    _radius = 0;
    for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
        if (_radius < (*it)->radius)_radius = (*it)->radius;
    }
}


SuperMesh* ENGINE_API quby::loadModel(const char* filename,Material** mats, unsigned mats_size) {
	
	
	
	const aiScene* scene = aiImportFile(filename, 0);
	
	if (!scene) return nullptr;
	
	SuperMesh* res = new SuperMesh();
	
	
	
	for (unsigned n = 0; n < scene->mNumMeshes; ++n) {
		aiMesh* aimesh = scene->mMeshes[n];
		
		
		MeshData mdata;
		
		mdata.verts = aimesh->mNumVertices;
		mdata.coor = new float[mdata.verts*3];
		mdata.nor = new float[mdata.verts*3];
		mdata.tex = new float[mdata.verts*2];
		
		
		unsigned tc = 0;
		for (unsigned t = 0; t < aimesh->mNumFaces; ++t) {
			if(aimesh->mFaces[t].mNumIndices == 3) {
				tc += 1;
			} else {
				tc += 2;
			}
		}
		
		mdata.faces = tc;
		mdata.ind = new unsigned[mdata.faces * 3];
		
		
		for (unsigned t = 0; t < aimesh->mNumVertices; ++t) {
			mdata.coor[t * 3] = aimesh->mVertices[t].x; 
			mdata.coor[t * 3 + 1] = aimesh->mVertices[t].y; 
			mdata.coor[t * 3 + 2] = aimesh->mVertices[t].z; 
			
			mdata.nor[t * 3] = aimesh->mNormals[t].x; 
			mdata.nor[t * 3 + 1] = aimesh->mNormals[t].y; 
			mdata.nor[t * 3 + 2] = aimesh->mNormals[t].z; 
			
			mdata.tex[t * 2] = aimesh->mTextureCoords[0][t].x; 
			mdata.tex[t * 2 + 1] = aimesh->mTextureCoords[0][t].y;
			
		}
		
		tc = 0;
		for (unsigned t = 0; t < aimesh->mNumFaces; ++t) {
			if(aimesh->mFaces[t].mNumIndices == 3) {
				mdata.ind[tc] = aimesh->mFaces[t].mIndices[0];
				mdata.ind[tc + 1] = aimesh->mFaces[t].mIndices[1];
				mdata.ind[tc + 2] =	aimesh->mFaces[t].mIndices[2];
				tc += 3;
			} else {
				mdata.ind[tc] = aimesh->mFaces[t].mIndices[0];
				mdata.ind[tc + 1] = aimesh->mFaces[t].mIndices[1];
				mdata.ind[tc + 2] =	aimesh->mFaces[t].mIndices[2];
				mdata.ind[tc + 3] = aimesh->mFaces[t].mIndices[0];
				mdata.ind[tc + 4] = aimesh->mFaces[t].mIndices[2];
				mdata.ind[tc + 5] = aimesh->mFaces[t].mIndices[3];
				tc += 4;
			}
		}
		
		
		Mesh* mesh = new Mesh(mdata);
		
		if (aimesh->mMaterialIndex < mats_size) mesh->material = mats[aimesh->mMaterialIndex];
		
		res->meshes.push_back(mesh);
		
		
		
		delete mdata.coor;
		delete mdata.nor;
		delete mdata.tex;
		
	}
	res->bake();
	return res;
}


