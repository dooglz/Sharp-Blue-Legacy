﻿#pragma once

#include "SDL_MeshLoader.h"
#include "../Mesh.h"
#include "../Maths.h"

#include "SDL_Platform.h"
#include "glew/glew.h"

namespace Engine {
	namespace SDL {

		CSDL_Meshloader::CSDL_Meshloader() {}

		Mesh* CSDL_Meshloader::openOBJFile(const std::string& filename) {
			printf("Loading object: %s\n", (filename.c_str()));
			const char* path = filename.c_str();

			std::vector<Vector3> vertices;
			std::vector<Vector2> uvs;
			std::vector<Vector3> normals;
			std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
			std::vector<Vector3> temp_vertices;
			std::vector<Vector2> temp_uvs;
			std::vector<Vector3> temp_normals;

			FILE* file = fopen(path, "r");
			if (file == NULL) {
				printf("Impossible to open the file! Are you in the right path ?\n");
				return NULL;
			}

			while (1) {
				char lineHeader[128];
				// read the first word of the line
				int res = fscanf(file, "%s", lineHeader);
				if (res == EOF) {
					break;
				}  // EOF = End Of File. Quit the loop.

				// else : parse lineHeader

				if (strcmp(lineHeader, "v") == 0) {
					Vector3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					Vector2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					uv.y = -uv.y;  // Invert V coordinate since we will only use DDS texture,
					// which are inverted. Remove if you want to use TGA or BMP
					// loaders.
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					Vector3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					std::string vertex1, vertex2, vertex3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					char data[64];
					fgets(data, 64, file);
					// THERE MAY BE QUADS, if there is, we only take the last 4.

					int matches = sscanf(data, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertexIndex[0], &uvIndex[0], &normalIndex[0],
						&vertexIndex[1], &uvIndex[1], &normalIndex[1],
						&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (matches != 9) {
						matches = sscanf(data, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1],
							&vertexIndex[2]);
						if (matches != 3) {
							printf(
								"File can't be read by our simple parser :-( Try exporting with "
								"other options %i\n",
								matches);
							return NULL;
						}
					}
					vertexIndices.push_back(vertexIndex[0]);
					vertexIndices.push_back(vertexIndex[1]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[0]);
					normalIndices.push_back(normalIndex[1]);
					normalIndices.push_back(normalIndex[2]);
				}
				else {
					// Probably a comment, eat up the rest of the line
					char stupidBuffer[1000];
					fgets(stupidBuffer, 1000, file);
				}
			}

			// For each vertex of each triangle
			for (unsigned int i = 0; i < vertexIndices.size(); i++) {
				// Get the indices of its attributes
				unsigned int vertexIndex = vertexIndices[i];
				unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				// Get the attributes thanks to the index
				// Put the attributes in buffers
				Vector3 vertex = temp_vertices[vertexIndex - 1];

				vertices.push_back(vertex);
				if (!temp_uvs.empty()) {
					Vector2 uv = temp_uvs[uvIndex - 1];
					uvs.push_back(uv);
				}
				if (!temp_normals.empty()) {
					Vector3 normal = temp_normals[normalIndex - 1];
					normals.push_back(normal);
				}
			}

			printf("file read success, vertices:%i\n", vertices.size());
			//--turn the obj into our stupid mesh class

			Mesh* m = new Mesh();
			m->numVerts = vertices.size();
			randomColor();
			randomColor();
			unsigned int col1 = randomColor();
			unsigned int col2 = randomColor();
			bool b = false;
			for (int i = 0; i < (m->numVerts); ++i) {
				b = !b;
				stVertex a;
				a.x = vertices[i].x;
				a.y = vertices[i].y;
				a.z = vertices[i].z;
				if (b) {
					a.rgba = col1;
				}
				else {
					a.rgba = col2;
				}

				m->vertexData.push_back(a);
			}
			m->loadedMain = true;
			m->strip = false;

			return m;
		}

		void CSDL_Meshloader::loadOnGPU(Mesh* msh) {
			// check to see if we are already loaded.
			if (msh->loadedLocal) {
				return;
			}

			// Generate VAO
			glGenVertexArrays(1, &msh->gVAO);
			SDL_Platform::CheckGL();

			// Bind VAO
			glBindVertexArray(msh->gVAO);
			SDL_Platform::CheckGL();

			// Generate VBO
			glGenBuffers(1, &(msh->gVBO));
			SDL_Platform::CheckGL();

			// Bind VBO
			glBindBuffer(GL_ARRAY_BUFFER, (msh->gVBO));
			SDL_Platform::CheckGL();

			// put the data in it
			glBufferData(GL_ARRAY_BUFFER, msh->vertexData.size() * sizeof(stVertex),
				&msh->vertexData[0], GL_STATIC_DRAW);
			SDL_Platform::CheckGL();

			/* stVertex layout:
			[x,y,z,color]
			[float,float,float,uint]
			Color is really 4 chars [rgba]
			So effectivly, this is what ogl will see:
			[float,float,float,char,char,char,char]
			*/
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,  // index
				3,  // size
				GL_FLOAT,  // type
				GL_FALSE,  // normalised
				sizeof(stVertex),  // stride
				NULL  // pointer/offset
				);
			SDL_Platform::CheckGL();

			// color data
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,  // index
				4,  // size
				GL_UNSIGNED_BYTE,  // type
				GL_TRUE,  // normalised
				sizeof(stVertex),  // stride
				(void*)(sizeof(float)* 3)  // pointer/offset
				);
			SDL_Platform::CheckGL();

			// Unblind VAO
			glEnableVertexAttribArray(NULL);
			SDL_Platform::CheckGL();

			// Unblind VBO
			glBindVertexArray(NULL);
			SDL_Platform::CheckGL();

			msh->loadedLocal = true;
		}

		CSDL_Meshloader::~CSDL_Meshloader() {
			// Todo: unload all meshes.
		}
	}
}