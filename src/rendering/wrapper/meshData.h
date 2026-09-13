//
// Created by beni on 2026. 09. 04..
//
#pragma once

#include <vector>
#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "glad/glad.h"
#include "model/voxels/ChunkData.h"


struct VertexPos3 {
	glm::vec3 position;
};

struct VertexPos3Color3
{
    glm::vec3 position;
    glm::vec3 color;
};

struct VertexPos3Tex2
{
    glm::vec3 position;
    glm::vec2 texCoord;
};

struct VertexPos2Tex2Light1
{
    glm::vec2 position;
    glm::vec2 texCoord;
    float light;
};

struct VertexPos2Tex2
{
    glm::vec2 position;
    glm::vec2 texCoord;
};


struct VertexPos3Norm3tTex2
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};



template<typename VertexT>
struct MeshObject
{
    std::vector<VertexT> vertexArray;
    std::vector<GLuint>  indexArray;
};

struct OGLObject
{
    GLuint  vaoID = 0; // Vertex Array Object erőforrás azonosító
    GLuint  vboID = 0; // Vertex Buffer Object erőforrás azonosító
    GLuint  iboID = 0; // Index Buffer Object erőforrás azonosító
    GLsizei count = 0; // mennyi indexet/vertexet kell rajzolnunk
};


struct VertexAttributeDescriptor
{
    GLuint index = -1;
    GLuint strideInBytes = 0;
    GLint  numberOfComponents = 0;
    GLenum glType = GL_NONE;
};



class DrawableMesh
{
public:

	DrawableMesh();
	virtual ~DrawableMesh() = default;

	virtual void Init() = 0;
	void Clean();

	void SetWorldTransform(const glm::mat4& trf);
	[[nodiscard]] const glm::mat4& GetWorldTransform() const;

	virtual void DrawObject() const;



protected:
	OGLObject gpuObject;
	bool isGenerated = false;


	glm::mat4 world;
	glm::mat4 invWorld;

	virtual void CleanOther() {}
	static void CleanOGLObject( OGLObject& ObjectGPU );

	template <typename VertexT>
	void GenerateFromMesh(const MeshObject<VertexT>& mesh, std::initializer_list<VertexAttributeDescriptor> vertexAttrDescList)
	{
		if (isGenerated)
			CleanOGLObject(gpuObject);

		gpuObject = CreateGLObjectFromMesh(mesh, vertexAttrDescList);

		isGenerated = true;
	}

	static const std::initializer_list<VertexAttributeDescriptor>& GetVertexAttribListPos3Tex2();
	static const std::initializer_list<VertexAttributeDescriptor>& GetVertexAttribListPos3Norm3Tex2();
	static const std::initializer_list<VertexAttributeDescriptor>& GetVertexAttribListPos2Tex2Light1();
	static const std::initializer_list<VertexAttributeDescriptor>& GetVertexAttribListPos2Tex2();

	static const std::initializer_list<VertexAttributeDescriptor> &GetVertexAttribListPos3();

private:


	template <typename VertexT>
	[[nodiscard]] static OGLObject CreateGLObjectFromMesh( const MeshObject<VertexT>& mesh, const std::initializer_list<VertexAttributeDescriptor> vertexAttrDescList )
	{
		OGLObject meshGPU = { 0 };


		// hozzunk létre egy új VBO erőforrás nevet
		glCreateBuffers(1, &meshGPU.vboID);

		// töltsük fel adatokkal a VBO-t
		glNamedBufferData(meshGPU.vboID,	// a VBO-ba töltsünk adatokat
						   mesh.vertexArray.size() * sizeof(VertexT),		// ennyi bájt nagyságban
						   mesh.vertexArray.data(),	// erről a rendszermemóriabeli címről olvasva
						   GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévő adatokat

		// index puffer létrehozása
		glCreateBuffers(1, &meshGPU.iboID);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGPU.iboID);
		glNamedBufferData(meshGPU.iboID, mesh.indexArray.size() * sizeof(GLuint), mesh.indexArray.data(), GL_STATIC_DRAW);

		meshGPU.count = static_cast<GLsizei>(mesh.indexArray.size());

		// 1 db VAO foglalása
		glCreateVertexArrays(1, &meshGPU.vaoID);
		// a frissen generált VAO beallítasa aktívnak

		glVertexArrayVertexBuffer( meshGPU.vaoID, 0, meshGPU.vboID, 0, sizeof( VertexT ) );

		// attribútumok beállítása
		for ( const auto& vertexAttrDesc: vertexAttrDescList )
		{
			glEnableVertexArrayAttrib( meshGPU.vaoID, vertexAttrDesc.index ); // engedélyezzük az attribútumot
			glVertexArrayAttribBinding( meshGPU.vaoID, vertexAttrDesc.index, 0 ); // melyik VBO-ból olvassa az adatokat

	        switch ( vertexAttrDesc.glType )
	        {

	        case GL_FLOAT: // Az attribútum float32-kat tartalmaz
			    glVertexArrayAttribFormat(
		    		meshGPU.vaoID,						  // a VAO-hoz tartozó attribútumokat állítjuk be
		    		vertexAttrDesc.index,				  // a VB-ben található adatok közül a soron következő "indexű" attribútumait állítjuk be
		    		vertexAttrDesc.numberOfComponents,	  // komponens szám
		    		vertexAttrDesc.glType,				  // adatok típusa
		    		GL_FALSE,							  // normalizalt legyen-e
		    		vertexAttrDesc.strideInBytes       // az attribútum hol kezdődik a sizeof(VertexT)-nyi területen belül
	            );
	            break;
	        case GL_UNSIGNED_INT: // Az attribútum uint-eket tartalmaz
	            glVertexArrayAttribIFormat(
	                meshGPU.vaoID,						  // a VAO-hoz tartozó attribútumokat állítjuk be
	                vertexAttrDesc.index,				  // a VB-ben található adatok közül a soron következő "indexű" attribútumait állítjuk be
	                vertexAttrDesc.numberOfComponents,	  // komponens szám
	                vertexAttrDesc.glType,				  // adatok típusa
	                vertexAttrDesc.strideInBytes       // az attribútum hol kezdődik a sizeof(VertexT)-nyi területen belül
	            );
	            break;
	        case GL_DOUBLE: // Az attribútum double-öket tartalmaz
	            glVertexArrayAttribLFormat(
	                meshGPU.vaoID,						  // a VAO-hoz tartozó attribútumokat állítjuk be
	                vertexAttrDesc.index,				  // a VB-ben található adatok közül a soron következő "indexű" attribútumait állítjuk be
	                vertexAttrDesc.numberOfComponents,	  // komponens szám
	                vertexAttrDesc.glType,				  // adatok típusa
	                vertexAttrDesc.strideInBytes       // az attribútum hol kezdődik a sizeof(VertexT)-nyi területen belül
	            );
	            break;
	        default: // Minden egyébnél feltételezzük, hogy az attribútum a [0,1] vagy [-1,1] intervallum egészekkel való tömörítése
	                 // Ezért itt bekapcsoljuk a normalizálást.
	            glVertexArrayAttribFormat(
	                meshGPU.vaoID,						  // a VAO-hoz tartozó attribútumokat állítjuk be
	                vertexAttrDesc.index,				  // a VB-ben található adatok közül a soron következő "indexű" attribútumait állítjuk be
	                vertexAttrDesc.numberOfComponents,	  // komponens szám
	                vertexAttrDesc.glType,				  // adatok típusa
	                GL_TRUE,							  // normalizalt legyen-e
	                vertexAttrDesc.strideInBytes       // az attribútum hol kezdődik a sizeof(VertexT)-nyi területen belül
	            );
	            break;
	        }

		}
		glVertexArrayElementBuffer( meshGPU.vaoID, meshGPU.iboID );

		return meshGPU;
	}

};
