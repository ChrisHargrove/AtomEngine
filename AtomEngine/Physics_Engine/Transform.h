/*!
    * \class Transform "Transform.h"
    * \brief A transform component
    *
    * This component handles all translation, rotation and scaling of a parent game object.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Component.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>

class ATOM_API Transform : public Component
{
public:
    Transform();
    virtual ~Transform();

    /*!
        * \brief Sets the translation for this component.
        * \param position A 3 component vector to set translation.
        *
        * Will set the translation for the component which the parent GameObject can retrieve
        * its positional data from
    */
    void SetPosition(glm::vec3 position);

    /*!
        * \brief Sets the rotation for this component.
        * \param rotation A 3 component vector to set rotation.
        *
        * Will set the rotation for the component using Euler Angles. The parent GameObject
        * can retrieve this rotational data.
        * Each component pertains to its axis, so X value is on the x-axis etc.
    */
    void SetRotation(glm::vec3 rotation);

    /*!
        * \brief Sets the rotation for this component.
        * \param rotation A 4 component quaternion to set rotation.
        *
        * Will set the rotation for the component using a quaternion. The parent GameObject
        * can retrieve this rotational data.
    */
    void SetRotation(glm::quat rotation);

    /*!
        * \brief Sets the scale for this component.
        * \param scale A 3 component vector to set the scale.
        *
        * Will set the scale for the component using a 3 component vector which denotes scale
        * in each axis respectively, x, y, z. The parent GameObject can then retrieve this data.
    */
    void SetScale(glm::vec3 scale);

    /*!
        * \brief Gets the current translation of the component.
        * \return A 3 component vector containing translation data.
    */
    glm::vec3& GetPosition();

    /*!
        * \brief Gets the current rotation of the component.
        * \return A 4 component quaternion containing rotation data.
    */
    glm::quat& GetRotation();

    glm::mat3 GetRotationMatrix();

    /*!
        * \brief Gets the current rotation angles as Euler Angles.
        * \return A 3 component vector containing rotation data as Euler Angles
    */
    glm::vec3& GetEulerAngles();

    /*!
        * \brief Gets the current scale of the component.
        * \return A 3 component vector containing scale data, where each part of the
        * vector represents scale on x,y,z axis
    */
    glm::vec3& GetScale();

    /*!
        * \brief Get the transform matrix for the component.
        * \return Returns a 4x4 matrix containing all transformation data.
    */
    glm::mat4& GetTransform();

    glm::mat4* GetTransformPtr();

    /*!
        * \brief Gets the Forward Vector of the transform.
        * \return Returns a 3 component vector representing the forward direction.
        * 
        * Calculates the Forward vector by disassembling the transformation matrix.
    */
    glm::vec3 GetForward();

    /*!
        * \brief Gets the Right Vector of the transform.
        * \return Returns a 3 component vector representing the Right direction.
        *
        * Calculates the Right vector by disassembling the transformation matrix.
    */
    glm::vec3 GetRight();

    /*!
        * \brief Gets the Up Vector of the transform.
        * \return Returns a 3 component vector representing the Up direction.
        *
        * Calculates the Up vector by disassembling the transformation matrix.
    */
    glm::vec3 GetUp();

    /*!
        * \brief Rotates the Transform
        * \param rotation The amount to rotate, represented as Yaw, Pitch, Roll respectively.
    */
    void Rotate(glm::vec3 rotation);

    /*!
        * \brief Translates the Transform
        * \param translation The amount to translate by on X,Y,Z axis' respectively.
    */
    void Translate(glm::vec3 translation);

    /*!
        * \brief Scales the Transform
        * \param scale The amount to scale by on X,Y,Z axis' respectively.
    */
    void Scale(glm::vec3 scale);

    /*!
        * \brief Virtual Update function that is called each frame.
        * \param deltaTime The time passed since last frame.
        *
        * This function is where the component behaviour is applied.
    */
    void Update(float deltaTime) override;

    /*!
        * \brief Virtual Initialize function that is only called once.
        *
        * This function is where the initial setup for this component is set.
    */
    bool Initialize() override;

    

private:

    friend class cereal::access;

    /*!
        * \brief Calculates the transformation matrix.
    */
    void CalculateTransform();

    void CalculateRotation();

    glm::vec3 m_position;    /*!< A 3 component vector containing translation data for the Transform. */
    glm::quat m_orientation;       /*!< A 3 component vector containing rotation data for the transform. */
    glm::vec3 m_eulerAngles;
    glm::vec3 m_scale;          /*!< A 3 component vector containing scale data for the transform. */
    glm::mat4 m_transformMatrix; /*!< A 4x4 Matrix containing all transform data. */

    Transform* m_parentTransform; /*!< A parent Transform Component. */

    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("Position", m_position), cereal::make_nvp("Orientation", m_orientation), cereal::make_nvp("Scale", m_scale));
    }

};

CEREAL_REGISTER_TYPE_WITH_NAME(Transform, "TransformObject");
