/*!
* \class SubMesh "Mesh.h"
* \brief A SubMesh object that is only used for Mesh Components.
*
* This class deals with the storage of Vertex data and holds the Buffer information for rendering purposes.
*/

/*!
* \class Mesh "Mesh.h"
* \brief A mesh component
*
* This component is ued to handle everything to do with model loading and rendering. It is a list of SubMesh objects, that 
* actually hold all the vertex data.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Buffer.h"
#include <GLM/glm.hpp>
#include <vector>
#include <string>

#include "Types.h"

class ATOM_API SubMesh
{
public:
    SubMesh();
    virtual ~SubMesh();

    friend class ModelLoader;
    friend class Mesh;

    /*!
        * \brief Will Render the submesh.
    */
    void Render();

    void Render(int instanceCount);

    Buffer* GetVAO();

private:
    unsigned int m_drawCount;               /*!< The number of vertices that need to be drawn. */
    std::vector<ComplexVertex> m_vertices;  /*!< A vector of vertex data. */
    std::vector<unsigned int> m_indices;    /*!< A vector of indice data. */

    Buffer m_vertexArray;       /*!< The Vertex Array Object. */
    Buffer m_vertexBuffer;      /*!< The Vertex Buffer Object. */
    Buffer m_elementBuffer;     /*!< The Element Array Buffer. */
};

class ATOM_API Mesh : public Component
{
public:
    Mesh();
    virtual ~Mesh();

    friend class ModelLoader;

    /*!
        * \brief Sets the name of the mesh that needs to be loaded.
        * \param meshName The name of the mesh you wish to load.
        *
        * This function only sets the name string of the mesh. This is primarily to be used
        * when reading in serialized data..
    */
    void SetMesh(const std::string& meshName);

    /*!
        * \brief Loads a mesh from its already stored mesh name.
        * \return Returns a boolean as to whether it was successful or not. False = failure.
        *
        * This function will check the name string of the mesh, if it is empty it will read the serialized
        * data for this mesh to retrieve its mesh name. If the name is already set then it will just call the 
        * other load mesh function.
    */
    bool LoadMesh();

    /*!
        * \brief Loads a mesh from the mesh name provided
        * \param meshName The name of the mesh you wish to load.
        * \return Returns a boolean as to whether it was successful or not. False = failure.
        *
        * Loads the file from the mesh name and then processes all data from the file and populates
        * the SubMesh list of the Mesh.
    */
    bool LoadMesh(const std::string& meshName);

    /*!
        * \brief Renders the entire mesh.
        *
        * Loops through all SubMesh's and renders them all.
    */
    void Render();

    void Render(int instanceCount);

    std::vector<SubMesh*> GetSubMeshList();

    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("MeshName", m_meshName));
    }

    /*!
        * \brief A virtual Update function that is called every frame.
        * \param deltaTime The time passed since the last frame.
        *
        * This function is where the component behaviour is applied.
    */
    void Update(float deltaTime) override;

    /*!
        * \brief A virtual Initialize function that is only called once.
        *
        * This function is where the initial setup for this component is set.
    */
    void Initialize() override;

private:
    

    std::string m_meshName;                 /*!< The name of the overall Mesh. */
    std::vector<SubMesh*> m_subMeshList;    /*!< The list of sub-mesh's. */
};

CEREAL_REGISTER_TYPE_WITH_NAME(Mesh, "MeshObject");
