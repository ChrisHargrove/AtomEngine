/*!
    * \class Camera "Camera.h" 
    * \brief A camera component.
    * 
    * This component handles all the view capabilities of the game engine.
*/
#pragma once

#ifdef BUILDING_DLL
#define ATOM_API __declspec(dllexport)
#else
#define ATOM_API __declspec(dllimport)
#endif

#include "Component.h"
#include <GLM/glm.hpp>

class ATOM_API Camera : public Component
{
public:
    Camera();
    virtual ~Camera();

    /*!
        * \brief Gets the zoom value of the camera.
        * \returns Returns a float showing the level of zoom.
    */
    float GetZoom() const;

    /*!
        * \brief Gets the View Matrix from the Camera.
        * \return Returns a 4x4 Matrix containing the view matrix.
        *
        * This function recalculates the current view matrix when it is called, stores it
        * and then returns it.
    */
    glm::mat4 GetViewMatrix();

    /*!
        * \brief Gets the Orthographic View Matrix from the Camera.
        * \return Returns a 4x4 Matrix containing the orthographic view matrix.
        *
        * This function recalculates the current orthographic view matrix when it is called.
    */
    glm::mat4 GetOrthoMatrix() const;

    /*!
        * \brief Sets whether or not the camera should have its pitch constrained.
        * \param enable Should Pitch constraint be enabled. Defaults to true.
        *
        * This is primarily to stop the camera from spinning up and over. More used in FPS 
        * style cameras.
    */
    void EnablePitchConstraint(bool enable = true);

    /*!
        * \brief Sets a target for the camera to follow.
        * \param target The target position for the camera to follow.
        *
        * If No target is set then the camera will just look at a defined direction.
    */
    void SetTarget(glm::vec3 target);

    /*!
        * \brief Gets if the camera pitch has been constrained.
        * \return Returns a boolean value as to whether the camera is constrained.
    */
    bool IsPitchConstrained() const;

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

    float m_zoom;               /*!< The zoom value of the camera. */
    bool m_isPitchConstrained;  /*!< Should the camera pitch be contrained. */

    glm::vec3 m_target;         /*!< The camera target. */
    glm::mat4 m_viewMatrix;     /*!< The current 4x4 view matrix. */

    template<class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("Zoom", m_zoom),
            cereal::make_nvp("PitchConstrained", m_isPitchConstrained),
            cereal::make_nvp("Target", m_target));
    }
    
};

CEREAL_REGISTER_TYPE_WITH_NAME(Camera, "CameraObject");
