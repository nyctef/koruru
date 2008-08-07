#include "c3DSModel.h"


// Load 3DS model
c3DSModel::c3DSModel(std::string filename) {
	m_TotalFaces = 0;
	
	m_model = lib3ds_file_load(filename.c_str());
	// If loading the model failed, we throw an exception
	if(!m_model) throw strcat((char*)"Unable to load ", filename.c_str()); 
	
	// generate the bounding box and put opposite corners into bbmin and bbmax.
	lib3ds_file_bounding_box_of_objects(m_model, true, false, false, bbmin, bbmax);

	this->CreateVBO();
}

// Destructor
c3DSModel::~c3DSModel() {
	glDeleteBuffers(1, &m_VertexVBO);
	glDeleteBuffers(1, &m_NormalVBO);
	glDeleteBuffers(1, &m_TexCoordVBO);
	
	if(m_model != NULL) lib3ds_file_free(m_model);
}

// Copy vertices and normals to the memory of the GPU
void c3DSModel::CreateVBO() {
	if (m_model == NULL) throw "m_model is NULL, ouch!";
	
	// Calculate the number of faces we have in total
	GetFaces();
	
	// Allocate memory for our vertices and normals
	Lib3dsVector * vertices = new Lib3dsVector[m_TotalFaces * 3];
	Lib3dsVector * normals = new Lib3dsVector[m_TotalFaces * 3];
	Lib3dsTexel * texCoords = new Lib3dsTexel[m_TotalFaces * 3];
	   
	Lib3dsMesh * mesh;
	unsigned int FinishedFaces = 0;
	// Loop through all the meshes
	for(mesh = m_model->meshes;mesh != NULL;mesh = mesh->next) {
		lib3ds_mesh_calculate_normals(mesh, &normals[FinishedFaces*3]);
		// Loop through every face
		for(unsigned int cur_face = 0; cur_face < mesh->faces;cur_face++) {
			Lib3dsFace * face = &mesh->faceL[cur_face];
			for(unsigned int i = 0;i < 3;i++) {
				 if(mesh->texels) { // If there are texture-coordinates.
                         memcpy(&texCoords[FinishedFaces*3 + i], mesh->texelL[face->points[ i ]], sizeof(Lib3dsTexel)); 
                    }
				memcpy(&vertices[FinishedFaces*3 + i], mesh->pointL[face->points[i]].pos, sizeof(Lib3dsVector));
			}
			FinishedFaces++;
		}
	}
	
	// Generate a Vertex Buffer Object and store it with our vertices
	glGenBuffers(1, &m_VertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * m_TotalFaces, vertices, GL_STATIC_DRAW);
	
	// Generate another Vertex Buffer Object and store the normals in it
	glGenBuffers(1, &m_NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * m_TotalFaces, normals, GL_STATIC_DRAW);
	
	// Generate a third VBO and store the texture coordinates in it. 
    glGenBuffers(1, &m_TexCoordVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Lib3dsTexel) * 3 * m_TotalFaces, texCoords, GL_STATIC_DRAW);
	
	// Clean up our allocated memory (data is now in GPU)
	delete vertices;
	delete normals;
	delete texCoords;
	
	// We no longer need lib3ds
	lib3ds_file_free(m_model);
	m_model = NULL;
}

// Count the total number of faces this model has
void c3DSModel::GetFaces()
{
	if(m_model == NULL) throw "m_model is NULL. Ouch!";
	
	m_TotalFaces = 0;
	Lib3dsMesh * mesh;
	// Loop through every mesh
	for(mesh = m_model->meshes;mesh != NULL;mesh = mesh->next) {
		// Add the number of faces this mesh has to the total faces
		m_TotalFaces += mesh->faces;
	}
}

// Render the model using Vertex Buffer Objects
void c3DSModel::Draw() const
{
	if(m_TotalFaces == 0) throw "m_TotalFaces is 0. Ouch!";
	
	// Enable vertex and normal arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	
	// Bind the vbo with the normals
	glBindBuffer(GL_ARRAY_BUFFER, m_NormalVBO);
	
	// The pointer for the normals is NULL which means that OpenGL will use the currently bound vbo
	glNormalPointer(GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordVBO);     
    glTexCoordPointer(2, GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	// Render the triangles
	glDrawArrays(GL_TRIANGLES, 0, m_TotalFaces * 3);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
}

void c3DSModel::DrawBoundingBox() const {
	glBegin(GL_LINES);
		glVertex3f(bbmin[0], bbmin[1], bbmin[2]);
		glVertex3f(bbmax[0], bbmax[1], bbmax[2]);
		
		glVertex3f(bbmin[0], bbmin[1], bbmin[2]);
		glVertex3f(bbmax[0], bbmin[1], bbmin[2]);
		
		glVertex3f(bbmax[0], bbmin[1], bbmin[2]);
		glVertex3f(bbmax[0], bbmax[1], bbmin[2]);
		
		glVertex3f(bbmax[0], bbmax[1], bbmin[2]);
		glVertex3f(bbmin[0], bbmax[1], bbmin[2]);
		
		glVertex3f(bbmin[0], bbmax[1], bbmin[2]);
		glVertex3f(bbmin[0], bbmin[1], bbmin[2]);
		
		glVertex3f(bbmin[0], bbmin[1], bbmax[2]);
		glVertex3f(bbmax[0], bbmin[1], bbmax[2]);
		
		glVertex3f(bbmax[0], bbmin[1], bbmax[2]);
		glVertex3f(bbmax[0], bbmax[1], bbmax[2]);
		
		glVertex3f(bbmax[0], bbmax[1], bbmax[2]);
		glVertex3f(bbmin[0], bbmax[1], bbmax[2]);
		
		glVertex3f(bbmin[0], bbmax[1], bbmax[2]);
		glVertex3f(bbmin[0], bbmin[1], bbmax[2]);
	glEnd();
}
