#include "FrameworkPCH.h"

#include "TransformGizmo3D.h"

#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Core/FWCore.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "Scene/Scene.h"

#define SMALL_NUM 0.00000001
#define selection_cutoff 0.17
#define width_correction 0.055

namespace fw
{

void Gizmo3D::Draw(TransformMode mode, CameraComponent* pCamera, MyMatrix mat, ShaderProgram* pShader, Texture* pTexture, vec2 UVScale, vec2 UVOffset)
{
    if(mode == TransformMode::TRANSLATION ||
       mode == TransformMode::SCALE)
    {
        if (m_GizmoState == GizmoState::NONE)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Red(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Green(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Blue(), UVScale, UVOffset, nullptr);
            m_pOMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::White(), UVScale, UVOffset, nullptr);
        }
        
        if (m_GizmoState == GizmoState::X)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Green(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Blue(), UVScale, UVOffset, nullptr);
            m_pOMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::White(), UVScale, UVOffset, nullptr);
        }
    
        if (m_GizmoState == GizmoState::Y)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Red(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Blue(), UVScale, UVOffset, nullptr);
            m_pOMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::White(), UVScale, UVOffset, nullptr);
        }
    
        if (m_GizmoState == GizmoState::Z)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Red(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Green(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pOMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::White(), UVScale, UVOffset, nullptr);
        }
    
        if (m_GizmoState == GizmoState::XYZ)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pOMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
        }
    }
    
    if(mode == TransformMode::ROTATION)
    {
        if (m_GizmoState == GizmoState::NONE)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Red(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Green(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Blue(), UVScale, UVOffset, nullptr);
        }
        
        if (m_GizmoState == GizmoState::X)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Green(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Blue(), UVScale, UVOffset, nullptr);
        }
    
        if (m_GizmoState == GizmoState::Y)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Red(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Blue(), UVScale, UVOffset, nullptr);
        }
    
        if (m_GizmoState == GizmoState::Z)
        {
            m_pXMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Red(), UVScale, UVOffset, nullptr);
            m_pYMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Green(), UVScale, UVOffset, nullptr);
            m_pZMesh->Draw(pCamera, &mat, pShader, pTexture, nullptr, nullptr, Color::Yellow(), UVScale, UVOffset, nullptr);
        }
    }

}

Gizmo3D::~Gizmo3D()
{
}

TransformGizmo3D::TransformGizmo3D(GameCore* pGameCore)
{
    m_pShader = pGameCore->GetResourceManager()->GetShader("Gizmo");
    m_pGameCore = pGameCore;

    Init();
    Construct(m_TranslationGizmo3D, m_RotationGizmo3D, m_ScaleGizmo3D);
}

TransformGizmo3D::~TransformGizmo3D()
{
}
    
void TransformGizmo3D::Init()
{
    x_ray.origin = 0;
    x_ray.direction = vec3(1, 0 , 0);
    
    y_ray.origin = 0;
    y_ray.direction = vec3(0, 1 , 0);
    
    z_ray.origin = 0;
    z_ray.direction = vec3(0, 0 , 1);

    XY.normal = vec3(0, 0, 1);
    YZ.normal = vec3(1, 0, 0);
    XZ.normal = vec3(0, 1, 0);
}
    
void TransformGizmo3D::Construct(Gizmo3D& Translation, Gizmo3D& Rotation, Gizmo3D& Scale)
{
    {
        m_TranslationGizmo3D.m_pXMesh = m_pGameCore->GetResourceManager()->GetMesh("X-Arrow");
        m_TranslationGizmo3D.m_pYMesh =  m_pGameCore->GetResourceManager()->GetMesh("Y-Arrow");
        m_TranslationGizmo3D.m_pZMesh =  m_pGameCore->GetResourceManager()->GetMesh("Z-Arrow");
        m_TranslationGizmo3D.m_pOMesh =  m_pGameCore->GetResourceManager()->GetMesh("Sphere");
    }

    {
        m_RotationGizmo3D.m_pXMesh = m_pGameCore->GetResourceManager()->GetMesh("YZ-Ring");
        m_RotationGizmo3D.m_pYMesh = m_pGameCore->GetResourceManager()->GetMesh("XZ-Ring");
        m_RotationGizmo3D.m_pZMesh = m_pGameCore->GetResourceManager()->GetMesh("XY-Ring");
    }

    {
        m_ScaleGizmo3D.m_pXMesh = m_pGameCore->GetResourceManager()->GetMesh("X-Barrel");
        m_ScaleGizmo3D.m_pYMesh = m_pGameCore->GetResourceManager()->GetMesh("Y-Barrel");
        m_ScaleGizmo3D.m_pZMesh = m_pGameCore->GetResourceManager()->GetMesh("Z-Barrel");
        m_ScaleGizmo3D.m_pOMesh = m_pGameCore->GetResourceManager()->GetMesh("Box");
    }

}
    
void TransformGizmo3D::Draw()
{
    if( m_pActiveGameObject )
    {
        if(m_pActiveGameObject->GetScene())
        {
            CameraComponent* pSceneCamera = m_pActiveGameObject->GetScene()->GetActiveCameraComponent();
            
            if (pSceneCamera)
            {
                if (m_pActiveGameObject)
                {
                    m_ActiveGizmo->Draw(m_TransformMode, pSceneCamera, m_GizmoTransform, m_pShader, nullptr, 1, 0);
                }
            }
        }
    }
}

void TransformGizmo3D::Update(Scene* pActiveScene)
{
    UpdateTransformGizmo(pActiveScene);
    if(m_pActiveGameObject)
    {
        bool IsMouseDown = m_pGameCore->GetFramework()->IsMouseButtonDown(0);
        vec2 MousePos;
        int a, b, w;
        m_pGameCore->GetFramework()->GetMouseCoordinates(&a, &b);
        w = m_pGameCore->GetFramework()->GetWindowHeight();
        MousePos = vec2((float)a, (float)(w - b));
        static vec2 PrevMousePos = vec2((float)a, (float)b);

        ray camera_ray = GetCameraRay();

        vec3 POI_cam_with_x;
        vec3 POI_cam_with_y;
        vec3 POI_cam_with_z;
        float distance_from_x = ShortestDistanceBetweenTwoRays(&x_ray, &camera_ray, nullptr, &POI_cam_with_x);
        float distance_from_y = ShortestDistanceBetweenTwoRays(&y_ray, &camera_ray, nullptr, &POI_cam_with_y);
        float distance_from_z = ShortestDistanceBetweenTwoRays(&z_ray, &camera_ray, nullptr, &POI_cam_with_z);
        
        static vec3 prev_POI_cam_with_x = POI_cam_with_x;
        static vec3 prev_POI_cam_with_y = POI_cam_with_y;
        static vec3 prev_POI_cam_with_z = POI_cam_with_z;
        
        static vec3 point_of_intersection;
        
        if(!IsMouseDown)
        {
             if(m_TransformMode == TransformMode::TRANSLATION ||
                m_TransformMode == TransformMode::SCALE)
             {
                 UpdateTranslationAndScale(distance_from_x, distance_from_y, distance_from_z);
             }
             else
             {
                 point_of_intersection = UpdateRotation(&camera_ray, XY, YZ, XZ);
                 ImGui::Text("POI %f, %f, %f", point_of_intersection.x, point_of_intersection.y, point_of_intersection.z);
             } 
        }

        else
        {
            if(PrevMousePos != MousePos)
            {
                vec3 deltaX = POI_cam_with_x - prev_POI_cam_with_x;
                vec3 deltaY = POI_cam_with_y - prev_POI_cam_with_y;
                vec3 deltaZ = POI_cam_with_z - prev_POI_cam_with_z;
                
                if(m_TransformMode == TransformMode::TRANSLATION)
                {
                    ManipulateTranslation(camera_ray, deltaX, deltaY, deltaZ);
                }
                
                else if (m_TransformMode == TransformMode::ROTATION)
                {
                    ManipulateRotation(&camera_ray, point_of_intersection);
                }
                    
                else
                {
                    ManipulateScale(&camera_ray, deltaX, deltaY, deltaZ);
                }
            }
            prev_POI_cam_with_x = POI_cam_with_x;
            prev_POI_cam_with_y = POI_cam_with_y;
            prev_POI_cam_with_z = POI_cam_with_z;
        }
        PrevMousePos = vec2((float)a,(float)(w - b));
   }
}

void TransformGizmo3D::SetTransformMode(TransformMode mode)
{
    m_TransformMode = mode;
}

bool TransformGizmo3D::IsHovered()
{
    return m_ActiveGizmo->m_GizmoState != GizmoState::NONE;
}

void TransformGizmo3D::UpdateTransformGizmo(Scene* pActiveScene)
{
    m_pActiveGameObject = pActiveScene->GetSelectedGameObject();
    m_pCamera = pActiveScene->GetActiveCameraComponent();
    if(m_pActiveGameObject && m_pCamera)
    {
        vec3 pos = m_pActiveGameObject->GetTransform()->GetPosition();
        vec3 rot = 0;
        
        float fixed_distance_from_camera = 5.0f;

        float distance_from_camera = (pActiveScene->GetActiveCameraComponent()->GetPosition() - pos).Length();
        gizmo_scale = base_scale * distance_from_camera / fixed_distance_from_camera;
        
        m_GizmoTransform.CreateSRT(gizmo_scale, rot, pos);

        x_ray.origin = m_pActiveGameObject->GetTransform()->GetPosition();
        x_ray.scale = gizmo_scale;
        x_ray.end = x_ray.origin + x_ray.scale * x_ray.direction;
        
        y_ray.origin = m_pActiveGameObject->GetTransform()->GetPosition();
        y_ray.scale = gizmo_scale;
        y_ray.end = y_ray.origin + y_ray.scale * y_ray.direction;
        
        z_ray.origin = m_pActiveGameObject->GetTransform()->GetPosition();
        z_ray.scale = gizmo_scale;
        z_ray.end = z_ray.origin + z_ray.scale * z_ray.direction;

        XY.origin = m_pActiveGameObject->GetTransform()->GetPosition();
        YZ.origin = m_pActiveGameObject->GetTransform()->GetPosition();
        XZ.origin = m_pActiveGameObject->GetTransform()->GetPosition();
    }

    if(m_TransformMode == TransformMode::TRANSLATION) { m_ActiveGizmo = &m_TranslationGizmo3D; }
    if(m_TransformMode == TransformMode::ROTATION) { m_ActiveGizmo = &m_RotationGizmo3D; }
    if(m_TransformMode == TransformMode::SCALE) { m_ActiveGizmo = &m_ScaleGizmo3D; }
        
}
 ray TransformGizmo3D::GetCameraRay()
{
    vec2 device_coordinates = m_pCamera->GetNormalizedDeviceCoordinates();
     
    MyMatrix* proj = m_pCamera->GetProjectionMatrix();
    MyMatrix* view = m_pCamera->GetViewMatrix();

    MyMatrix view_proj_inverse = (*proj * *view).GetInverse();
    
    vec4 ray_start = view_proj_inverse * vec4(device_coordinates.x, device_coordinates.y, 0.0f, 1.0f);
    ray_start = ray_start * (1.0f / ray_start.w);
    
    vec4 ray_end = view_proj_inverse * vec4(device_coordinates.x, device_coordinates.y, 1.0f, 1.0f);
    ray_end = ray_end * (1.0f / ray_end.w);

    ray camera_ray;
    camera_ray.origin = ray_start.XYZ();
    camera_ray.end = ray_end.XYZ();
    camera_ray.direction = (camera_ray.end - camera_ray.origin).GetNormalized();

    return camera_ray;
}
     
float TransformGizmo3D::ShortestDistanceBetweenTwoRays(ray* R1, ray* R2, vec3 *P1, vec3 *P2)
{
     vec3   u = R1->end - R1->origin;
     vec3   v = R2->end - R2->origin;
     vec3   w = R1->origin - R2->origin;
     float    a = u.Dot(u);         // always >= 0
     float    b = u.Dot(v);
     float    c = v.Dot(v);         // always >= 0
     float    d = u.Dot(w);
     float    e = v.Dot(w);
     float    D = a*c - b*b;        // always >= 0
     float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
     float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0
     
     // compute the line parameters of the two closest points
     if (D < SMALL_NUM) { // the lines are almost parallel
         sN = 0.0;         // force using point origin on ray R1
         sD = 1.0;         // to prevent possible division by 0.0 later
         tN = e;
         tD = c;
     }
     else {                 // get the closest points on the infinite lines
         sN = (b*e - c*d);
         tN = (a*e - b*d);
         if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
             sN = 0.0;
             tN = e;
             tD = c;
         }
         else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
             sN = sD;
             tN = e + b;
             tD = c;
         }
     }
     
     if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
         tN = 0.0;
         // recompute sc for this edge
         if (-d < 0.0)
             sN = 0.0;
         else if (-d > a)
             sN = sD;
         else {
             sN = -d;
             sD = a;
         }
     }
     else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
         tN = tD;
         // recompute sc for this edge
         if ((-d + b) < 0.0)
             sN = 0;
         else if ((-d + b) > a)
             sN = sD;
         else {
             sN = (-d +  b);
             sD = a;
         }
     }
     // finally do the division to get sc and tc
     sc = (float)((abs(sN) < SMALL_NUM ? 0.0 : sN / sD));
     tc = (float)(abs(tN) < SMALL_NUM ? 0.0 : tN / tD);
     
     if(P1)
      {
          *P1 = w + (sc * u);
         int bp =1;
      }
     if(P2) { *P2 = tc * v; }   
    
     // get the difference of the two closest points
     vec3 dP = w + (sc * u) - tc * v;
     return dP.Length();   // return the closest distance
}

   
bool TransformGizmo3D::IntersectRayWithPlane(ray *R, plane *P)
{
     float denom = R->direction.Dot(P->normal);
     if (abs(denom) > 1e-6)
     { 
         vec3 p0r0 = P->origin - R->origin; 
         R->scale = P->normal.Dot(p0r0) / denom; 
         return (R->scale >= 0); 
     }
     
    return false; 
}

void TransformGizmo3D::UpdateTranslationAndScale(float distance_from_x, float distance_from_y, float distance_from_z)
{
    if(distance_from_x < width_correction * gizmo_scale &&
            distance_from_y > width_correction * gizmo_scale &&
            distance_from_z > width_correction * gizmo_scale)
    {
        m_ActiveGizmo->m_GizmoState = GizmoState::X;
    }
    
    else if(distance_from_y < width_correction * gizmo_scale &&
            distance_from_x > width_correction * gizmo_scale &&
            distance_from_z > width_correction * gizmo_scale)
    {
        m_ActiveGizmo->m_GizmoState = GizmoState::Y;
    }

    else if(distance_from_z < width_correction * gizmo_scale &&
            distance_from_x > width_correction * gizmo_scale &&
            distance_from_y > width_correction * gizmo_scale)
    {
        m_ActiveGizmo->m_GizmoState = GizmoState::Z;
    }
    
    else if(distance_from_z < width_correction * gizmo_scale &&
            distance_from_x < width_correction * gizmo_scale &&
            distance_from_z < width_correction * gizmo_scale)
    {
        m_ActiveGizmo->m_GizmoState = GizmoState::XYZ;
    }
        
    else
    {
        m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
    }
}

vec3 TransformGizmo3D::UpdateRotation(ray *camera_ray, plane XY, plane YZ, plane XZ)
{
    float distance_from_xy = FLT_MAX;
    float distance_from_xz = FLT_MAX;
    float distance_from_yz = FLT_MAX;

    vec3 POI_xy;
    vec3 POI_yz;
    vec3 POI_xz;
    
    if(IntersectRayWithPlane(camera_ray, &XY))
    {
        vec3 point_on_plane = camera_ray->origin + camera_ray->scale * camera_ray->direction;
        POI_xy = XY.origin + gizmo_scale * (point_on_plane - XY.origin).GetNormalized();
        distance_from_xy = (point_on_plane - POI_xy).Length();
    }
    
    if(IntersectRayWithPlane(camera_ray, &XZ))
    {
        vec3 point_on_plane = camera_ray->origin + camera_ray->scale * camera_ray->direction;
        POI_xz = XZ.origin + gizmo_scale * (point_on_plane - XZ.origin).GetNormalized();
        distance_from_xz = (point_on_plane - POI_xz).Length();
    }
    
    if(IntersectRayWithPlane(camera_ray, &YZ))
    {
        vec3 point_on_plane = camera_ray->origin + camera_ray->scale * camera_ray->direction;
        POI_yz = YZ.origin + gizmo_scale * (point_on_plane - YZ.origin).GetNormalized();
        distance_from_yz = (point_on_plane - POI_yz).Length();
    }
    
    
    if(distance_from_xy < distance_from_yz &&
       distance_from_xy < distance_from_xz)
    {
        if(distance_from_xy < width_correction * gizmo_scale)
        {
            m_ActiveGizmo->m_GizmoState = GizmoState::Z;
            return POI_xz;
        }
        else
        {
            m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
            return NULL;
        }
    }
    
    else if(distance_from_xz < distance_from_yz)
    {
        if(distance_from_xz < width_correction * gizmo_scale)
        {
            m_ActiveGizmo->m_GizmoState = GizmoState::Y;
            return POI_xz;
        }
        else
        {
            m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
            return NULL;
        }
    }
    
    else if(distance_from_yz < width_correction * gizmo_scale)
    {
       m_ActiveGizmo->m_GizmoState = GizmoState::X;
        return POI_yz;
    }
    else
    {
       m_ActiveGizmo->m_GizmoState = GizmoState::NONE;
        return NULL;
    }
}

void TransformGizmo3D::ManipulateTranslation(ray camera_ray, vec3 deltaX, vec3 deltaY, vec3 deltaZ)
{
    vec3 pos = m_pActiveGameObject->GetTransform()->GetPosition();
                     
    if(m_ActiveGizmo->m_GizmoState == GizmoState::X)
    {
        pos.x += deltaX.x;
        m_pActiveGameObject->GetTransform()->SetPosition(pos);
    }
    
    if(m_ActiveGizmo->m_GizmoState == GizmoState::Y)
    {
        pos.y += deltaY.y;
        m_pActiveGameObject->GetTransform()->SetPosition(pos);
    }
    
    if(m_ActiveGizmo->m_GizmoState == GizmoState::Z)
    {
        pos.z += deltaZ.z;
        m_pActiveGameObject->GetTransform()->SetPosition(pos);
    }
    
    if(m_ActiveGizmo->m_GizmoState == GizmoState::XYZ)
    {
        plane new_plane;
        new_plane.origin = m_pActiveGameObject->GetTransform() ->GetPosition();
        new_plane.normal = (m_pCamera->GetPosition() - m_pActiveGameObject->GetTransform()->GetPosition()).GetNormalized();

        IntersectRayWithPlane(&camera_ray, &new_plane);
        pos = camera_ray.origin + camera_ray.scale * camera_ray.direction; 
        m_pActiveGameObject->GetTransform()->SetPosition(pos);
    }
     
}

void TransformGizmo3D::ManipulateRotation(ray* camera_ray, vec3 point_of_intersection)
{
    vec3 rot = m_pActiveGameObject->GetTransform()->GetRotation();
 
    if(m_ActiveGizmo->m_GizmoState == GizmoState::X)
    {
        vec3 OP = (YZ.origin - point_of_intersection).GetNormalized();
        ray tangent;
        
        tangent.origin = point_of_intersection;
        tangent.direction = OP.Cross(YZ.normal);
        tangent.scale = FLT_MAX;
        
        vec3 poi_tangent;
        ShortestDistanceBetweenTwoRays(&tangent, camera_ray, nullptr, &poi_tangent);
        static vec3 prev_poi_tangent = poi_tangent;
        float deltaX = (poi_tangent - prev_poi_tangent).Dot(tangent.direction);
        rot.x+=  -deltaX * 100 / gizmo_scale;
        
        m_pActiveGameObject->GetTransform()->SetRotation(rot);
        prev_poi_tangent = poi_tangent;
    }
    
    else if(m_ActiveGizmo->m_GizmoState == GizmoState::Y)
    {
        vec3 OP = (XZ.origin - point_of_intersection).GetNormalized();
        ray tangent;
        
        tangent.origin = point_of_intersection;
        tangent.direction = OP.Cross(XZ.normal);
        tangent.scale = FLT_MAX;
        
        vec3 poi_tangent;
        ShortestDistanceBetweenTwoRays(&tangent, camera_ray, nullptr, &poi_tangent);
        static vec3 prev_poi_tangent = poi_tangent;
        float delta = (poi_tangent - prev_poi_tangent).Dot(tangent.direction);
        rot.y+=  -delta * 100 / gizmo_scale;
        
        m_pActiveGameObject->GetTransform()->SetRotation(rot);
        prev_poi_tangent = poi_tangent;
    }
                    
    else if(m_ActiveGizmo->m_GizmoState == GizmoState::Z)
    {
        vec3 OP = (XY.origin - point_of_intersection).GetNormalized();
        ray tangent;
        
        tangent.origin = point_of_intersection;
        tangent.direction = OP.Cross(XY.normal);
        tangent.scale = FLT_MAX;
        
        vec3 poi_tangent;
        ShortestDistanceBetweenTwoRays(&tangent, camera_ray, nullptr, &poi_tangent);
        static vec3 prev_poi_tangent = poi_tangent;
        float delta = (poi_tangent - prev_poi_tangent).Dot(tangent.direction);
        rot.z+=  -delta * 100 / gizmo_scale;
        
        m_pActiveGameObject->GetTransform()->SetRotation(rot);
        prev_poi_tangent = poi_tangent;
    }
}

void TransformGizmo3D::ManipulateScale(ray* camera_ray, vec3 deltaX, vec3 deltaY, vec3 deltaZ)
{
    if(m_ActiveGizmo->m_GizmoState == GizmoState::X)
    {
        MyMatrix object_transform = *m_pActiveGameObject->GetTransform()->GetWorldTransform();
        object_transform.Scale(1 + deltaX.x, 1, 1);
        vec3 new_scale = object_transform.GetScale();
        m_pActiveGameObject->GetTransform()->SetScale(new_scale);
    }
    
    if(m_ActiveGizmo->m_GizmoState == GizmoState::Y)
    {
        MyMatrix object_transform = *m_pActiveGameObject->GetTransform()->GetWorldTransform();
        object_transform.Scale(1, 1 + deltaY.y, 1);
        vec3 new_scale = object_transform.GetScale();
        m_pActiveGameObject->GetTransform()->SetScale(new_scale);
   }
    
    if(m_ActiveGizmo->m_GizmoState == GizmoState::Z)
    {
        MyMatrix object_transform = *m_pActiveGameObject->GetTransform()->GetWorldTransform();
        object_transform.Scale(1, 1, 1 + deltaZ.z);
        vec3 new_scale = object_transform.GetScale();
        m_pActiveGameObject->GetTransform()->SetScale(new_scale);
    }
    
    if(m_ActiveGizmo->m_GizmoState == GizmoState::XYZ)
    {
        vec3 scale = m_pActiveGameObject->GetTransform()->GetScale();
        scale +=  gizmo_scale * vec3(deltaX.x + deltaY.y + deltaZ.z);
        if(scale.x > 0 &&
           scale.y > 0 &&
           scale.z > 0)
        {
            m_pActiveGameObject->GetTransform()->SetScale(scale);
        }
    }                   
     
}
} // namespace fw
