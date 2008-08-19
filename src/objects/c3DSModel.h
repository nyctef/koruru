#ifndef C3DSMODEL_H_
#define C3DSMODEL_H_

// this file doesn't use includes.h since includes.h links to us.
//#include "../utils/includes.h"

#include "GL/glew.h"

#include <lib3ds/file.h>
#include <lib3ds/mesh.h>

#include <string>
#include <cstring>
using namespace std;

/** \brief A wrapper around lib3ds
 * 
 * Provides vetices and texture coordinates at the moment,
 * but textures may be broken and need checking out.
 */
class c3DSModel {
	public:
		c3DSModel(std::string filename);
		virtual void Draw() const;
		virtual void DrawBoundingBox() const;
		virtual void CreateVBO();
		virtual ~c3DSModel();
		GLfloat bbmin[3], bbmax[3];
	protected:
		void GetFaces();
		unsigned int m_TotalFaces;
		Lib3dsFile* m_model;
		GLuint m_VertexVBO, m_NormalVBO, m_TexCoordVBO;
};

#endif /*C3DSMODEL_H_*/
