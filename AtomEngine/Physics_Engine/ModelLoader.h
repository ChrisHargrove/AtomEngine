/*!
    * \class ModelLoader "ModelLoader.h"
    * \brief A helper class to assist with Loading Models.
    *
    * This class utilizes the Assimp Library to allow for the loading of many different file formats.
    * It will load the data and populate a Mesh object with the data.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <string>

class POD_Mesh;

class ATOM_API ModelLoader
{
public:
    /*!
        * \brief Static function to Load a model.
        * \param fileName The filename of the object you wish to load.
        * \param mesh A pointer to the mesh object you wish to populate with data.
        * \return Returns a boolean as to whether loading was successful or not. False = failure.
        *
        * This function with load the file from the specified path provided and place the data
        * inside the mesh provided.
    */
    static bool LoadModel(const std::string& fileName);

    static bool LoadModel(const std::string& fileName, POD_Mesh* mesh);

private:
    /*!
        * \brief Processes a node from the scene heirarchy inside the Assimp loader.
        * \param node The node to start at inside the Assimp scene.
        * \param scene The Assimp scene to be processing.
        * \param mesh A pointer to the mesh being populated.
        *
        * This function is used to recursivly get all data out of the scene heirarchy.
    */

    static void ProcessNode(aiNode* node, const aiScene* scene, POD_Mesh* mesh);

    /*!
        * \brief Processes a mesh from inside the node of an Assimp scene.
        * \param aimesh The mesh to be processed.
        * \param scene The scene that the mesh came from.
        * \param mesh The mesh that is to be populated with data.
        *
        * This function is where all data from the Assimp mesh is transferred into the mesh 
        * object.
    */

    static void ProcessMesh(aiMesh* aimesh, const aiScene* scene, POD_Mesh* mesh);

    static void CalculateMeshBounds(POD_Mesh* mesh);

};

