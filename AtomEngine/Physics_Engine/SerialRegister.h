#pragma once

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "BallCollider.h"

#include <CEREAL/cereal.hpp>
#include <CEREAL/archives/xml.hpp>
#include <CEREAL/types/memory.hpp>
#include <CEREAL/types/polymorphic.hpp>


CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Transform);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Mesh);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Camera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, RigidBody);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BoxCollider);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, BallCollider);
