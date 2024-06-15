#include <camera.h>

// Initialize the camera with given parameters
void init_camera(Camera *camera, vec3 position, vec3 up, float yaw, float pitch) {
    glm_vec3_copy(position, camera->Position);
    glm_vec3_copy(up, camera->WorldUp);
    camera->Yaw = yaw;
    camera->Pitch = pitch;
    camera->MovementSpeed = SPEED;
    camera->MouseSensitivity = SENSITIVITY;
    camera->Zoom = ZOOM;
    glm_vec3_zero(camera->Front);
    glm_vec3_zero(camera->Right);
    glm_vec3_zero(camera->Up);
    update_camera_vectors(camera);
}

// Get the view matrix using camera parameters
void camera_get_view_matrix(Camera *camera, mat4 view) {
    vec3 target;
    glm_vec3_add(camera->Position, camera->Front, target);
    glm_lookat(camera->Position, target, camera->Up, view);
}

// Process keyboard input to move the camera
void camera_process_keyboard(Camera *camera, Camera_Movement direction, float deltaTime) {
    float velocity = camera->MovementSpeed * deltaTime;
    vec3 temp;
    if (direction == FORWARD) {
        glm_vec3_scale(camera->Front, velocity, temp);
        glm_vec3_add(camera->Position, temp, camera->Position);
    }
    if (direction == BACKWARD) {
        glm_vec3_scale(camera->Front, velocity, temp);
        glm_vec3_sub(camera->Position, temp, camera->Position);
    }
    if (direction == LEFT) {
        glm_vec3_cross(camera->Front, camera->WorldUp, temp);
        glm_vec3_normalize(temp);
        glm_vec3_scale(temp, velocity, temp);
        glm_vec3_sub(camera->Position, temp, camera->Position);
    }
    if (direction == RIGHT) {
        glm_vec3_cross(camera->Front, camera->WorldUp, temp);
        glm_vec3_normalize(temp);
        glm_vec3_scale(temp, velocity, temp);
        glm_vec3_add(camera->Position, temp, camera->Position);
    }
}

// Process mouse movement to update camera direction
void camera_process_mouse_movement(Camera *camera, float xoffset, float yoffset, int constrainPitch) {
    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (constrainPitch) {
        if (camera->Pitch > 89.0f)
            camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f)
            camera->Pitch = -89.0f;
    }

    update_camera_vectors(camera);
}

// Process mouse scroll to adjust zoom level
void camera_process_mouse_scroll(Camera *camera, float yoffset) {
    camera->Zoom -= yoffset;
    if (camera->Zoom < 1.0f)
        camera->Zoom = 1.0f;
    if (camera->Zoom > 45.0f)
        camera->Zoom = 45.0f;
    printf("Zoom: %f\n", camera->Zoom);
    printf("Y-offset: %f\n", yoffset);

}

void camera_dolly_zoom(Camera* camera, float zoomAmount) {
    vec3 direction;
    glm_vec3_scale(camera->Front, zoomAmount, direction);
    glm_vec3_add(camera->Position, direction, camera->Position);
}

// Update camera vectors based on updated Euler angles
void update_camera_vectors(Camera *camera) {
    vec3 front;
    front[0] = cos(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch));
    front[1] = sin(glm_rad(camera->Pitch));
    front[2] = sin(glm_rad(camera->Yaw)) * cos(glm_rad(camera->Pitch));
    glm_normalize_to(front, camera->Front);

    glm_cross(camera->Front, camera->WorldUp, camera->Right);
    glm_normalize(camera->Right);

    glm_cross(camera->Right, camera->Front, camera->Up);
    glm_normalize(camera->Up);
}

void rotate_camera(Camera* camera, float angle, int vertical) {
    vec3 target;
    glm_vec3_add(camera->Position, camera->Front, target); // Target point to look at

    vec3 temp;
    glm_vec3_sub(camera->Position, target, temp);

    mat4 rotation;
    vec3 axis;
    if (vertical) {
        glm_vec3_copy(camera->Right, axis);
    } else {
        glm_vec3_copy(camera->WorldUp, axis);
    }
    
    glm_rotate_make(rotation, glm_rad(angle), axis);

    glm_mat4_mulv3(rotation, temp, 1.0f, temp);
    glm_vec3_add(temp, target, camera->Position);

    glm_vec3_sub(target, camera->Position, camera->Front);
    glm_vec3_normalize(camera->Front);

    glm_cross(camera->Front, camera->WorldUp, camera->Right);
    glm_normalize(camera->Right);

    glm_cross(camera->Right, camera->Front, camera->Up);
    glm_normalize(camera->Up);
}
