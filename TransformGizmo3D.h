#pragma once
#include "Math/MyMatrix.h"
#include "Math/Vector.h"

namespace fw
{

class Mesh;
class CameraComponent;
class vec3;
class ShaderProgram;
class Texture;
class GameCore;
class Scene;
class GameObject;

enum class GizmoState
{
    NONE,
    X,
    Y,
    Z,
    XY,
    YZ,
    XZ,
    XYZ
};

enum class TransformMode
{
    TRANSLATION,
    ROTATION,
    SCALE
};

struct Gizmo3D
{
    Mesh* m_pXMesh;
    Mesh* m_pYMesh;
    Mesh* m_pZMesh;
    Mesh* m_pOMesh;

    GizmoState m_GizmoState = GizmoState::NONE;
    void Draw(TransformMode mode, CameraComponent* pCamera, MyMatrix mat, ShaderProgram* pShader, class Texture* pTexture, vec2 UVScale, vec2 UVOffset);

    ~Gizmo3D();
};

struct ray
{
    vec3 origin;
    vec3 direction;
    vec3 end;
    float scale;
};

struct plane
{
    vec3 origin;
    vec3 normal;
};

struct circle
{
    vec3 origin;
    float radius;

    circle(vec3 o, float r) { origin = o; radius = r; }
};
    
class TransformGizmo3D
{
public:
    TransformGizmo3D(GameCore* pGameCore);
    ~TransformGizmo3D();

    void Draw();
    void Update(Scene* pActiveScene);
    void SetTransformMode(TransformMode mode);
    bool IsHovered();

    float base_scale = 0.8f;

private:
    void Init();
    void Construct(Gizmo3D& Translation, Gizmo3D& Rotation, Gizmo3D& Scale);
    void UpdateTransformGizmo(Scene* pActiveScene);
    float ShortestDistanceBetweenTwoRays(ray* R1, ray* R2, vec3 *P1 = nullptr, vec3 *P2 = nullptr);
    bool IntersectRayWithPlane(ray *R, plane *P);
    void UpdateTranslationAndScale(float distance_from_x, float distance_from_y, float distance_from_z);
    vec3 UpdateRotation(ray* camera_ray, plane xy, plane yz, plane xz);
    void ManipulateTranslation(ray camera_ray, vec3 deltaX, vec3 deltaY, vec3 deltaZ);
    void ManipulateScale(ray* camera_ray, vec3 deltaX, vec3 deltaY, vec3 deltaZ);
    void ManipulateRotation(ray* camera_ray, vec3 point_of_intersection);
    
    ray GetCameraRay();
    Gizmo3D m_TranslationGizmo3D;
    Gizmo3D m_RotationGizmo3D;
    Gizmo3D m_ScaleGizmo3D;

    ray x_ray;
    ray y_ray;
    ray z_ray;

    plane XY;
    plane YZ;
    plane XZ;
    
    Gizmo3D* m_ActiveGizmo = &m_TranslationGizmo3D;
    
    MyMatrix m_GizmoTransform;
    float gizmo_scale;

    TransformMode m_TransformMode = TransformMode::TRANSLATION;

    GameCore* m_pGameCore = nullptr;
    ShaderProgram* m_pShader = nullptr;
    GameObject* m_pActiveGameObject = nullptr;
    CameraComponent* m_pCamera = nullptr;
};

}


