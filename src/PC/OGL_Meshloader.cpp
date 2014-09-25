#include "OGL_Meshloader.h"
#include "OGL_Renderer.h"
#include "../FileIO.h"

//TODO: Move similar cross platrfomr code to a base class

namespace Engine{
	namespace OGL{
		stMesh* OGL_Meshloader::loadOBJFile(const std::string &filename)
		{

			const char * c = filename.c_str();

			//--
			printf("attempting to read file: %s\n", c);
			std::vector<stvec3> vertices;
			std::vector<stvec2> uvs;
			std::vector<stvec3> normals;
			bool res = loadOBJ(c, vertices, uvs, normals);
			printf("file read success, vertices:%i\n", vertices.size());
			//--turn the obj into our achacic mesh class

			stMesh* m = new stMesh();
			m->numVerts = vertices.size();

			for (int i = 0; i < (m->numVerts); ++i)
			{
				stVertex a;
				a.x = vertices[i].x;
				a.y = vertices[i].y;
				a.z = vertices[i].z;
				a.rgba = randomColor();
				m->vertexData.push_back(a);
			}
			m->loadedMain = true;
			m->strip = false;

			return m;
		}

		bool OGL_Meshloader::loadOBJ(const char * path, std::vector<stvec3> & out_vertices, std::vector<stvec2> & out_uvs, std::vector<stvec3> & out_normals)
		{
			printf("Loading OBJ file %s...\n", path);

			std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
			std::vector<stvec3> temp_vertices;
			std::vector<stvec2> temp_uvs;
			std::vector<stvec3> temp_normals;

			FILE * file = fopen(path, "r");

			if (file == NULL){
				printf("Impossible to open the file! Are you in the right path ?\n");
				getchar();
				DBG_HALT;
				return false;
			}

			while (1){

				char lineHeader[128];
				// read the first word of the line
				int res = fscanf(file, "%s", lineHeader);
				if (res == EOF)
					break; // EOF = End Of File. Quit the loop.

				// else : parse lineHeader

				if (strcmp(lineHeader, "v") == 0){
					stvec3 vertex;
					fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					temp_vertices.push_back(vertex);
				}
				else if (strcmp(lineHeader, "vt") == 0){
					stvec2 uv;
					fscanf(file, "%f %f\n", &uv.x, &uv.y);
					uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
					temp_uvs.push_back(uv);
				}
				else if (strcmp(lineHeader, "vn") == 0){
					stvec3 normal;
					fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					temp_normals.push_back(normal);
				}
				else if (strcmp(lineHeader, "f") == 0){
					std::string vertex1, vertex2, vertex3;
					unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
					int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					if (matches != 9){
						printf("File can't be read by our simple parser :-( Try exporting with other options\n");
						return false;
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
				else{
					// Probably a comment, eat up the rest of the line
					char stupidBuffer[1000];
					fgets(stupidBuffer, 1000, file);
				}

			}

			// For each vertex of each triangle
			for (unsigned int i = 0; i < vertexIndices.size(); i++){

				// Get the indices of its attributes
				unsigned int vertexIndex = vertexIndices[i];
				unsigned int uvIndex = uvIndices[i];
				unsigned int normalIndex = normalIndices[i];

				// Get the attributes thanks to the index
				stvec3 vertex = temp_vertices[vertexIndex - 1];
				stvec2 uv = temp_uvs[uvIndex - 1];
				stvec3 normal = temp_normals[normalIndex - 1];

				// Put the attributes in buffers
				out_vertices.push_back(vertex);
				out_uvs.push_back(uv);
				out_normals.push_back(normal);

			}

			return true;
		}


		void OGL_Meshloader::loadOnGPU(stMesh* msh)
		{
			//Generate VAO
			glGenVertexArrays(1, &msh->gVAO);
			OGL_Renderer::checkerr();

			//Bind VAO
			glBindVertexArray(msh->gVAO);
			OGL_Renderer::checkerr();

			//Generate VBO
			glGenBuffers(1, &(msh->gVBO));
			OGL_Renderer::checkerr();

			//Bind VBO
			glBindBuffer(GL_ARRAY_BUFFER, (msh->gVBO));
			OGL_Renderer::checkerr();

			//put the data in it
			glBufferData(GL_ARRAY_BUFFER, msh->vertexData.size() * sizeof(stVertex), &msh->vertexData[0], GL_STATIC_DRAW);
			OGL_Renderer::checkerr();


			/* stVertex layout:
					[x,y,z,color]
					[float,float,float,uint]
					Color is really 4 chars [rgba]
					So effectivly, this is what ogl will see:
					[float,float,float,char,char,char,char]
					*/
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,		//index
				3,		//size
				GL_FLOAT,	//type
				GL_FALSE,	//normalised
				sizeof(stVertex),	//stride
				NULL	//pointer/offset
				);
			OGL_Renderer::checkerr();

			//color data
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,	//index
				4,	//size
				GL_UNSIGNED_BYTE,	//type
				GL_TRUE,	//normalised
				sizeof(stVertex),	//stride
				(void*)(sizeof(float) * 3)	//pointer/offset
				);
			OGL_Renderer::checkerr();

			//Unblind VAO
			glEnableVertexAttribArray(NULL);
			OGL_Renderer::checkerr();

			//Unblind VBO
			glBindVertexArray(NULL);
			OGL_Renderer::checkerr();

			msh->loadedLocal = true;
		}
	}
}