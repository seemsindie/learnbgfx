#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

// Define camera movement options
typedef enum {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} Camera_Movement;

// Define camera struct
typedef struct {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
} Camera;

// Default camera values
#define YAW         -90.0f
#define PITCH       0.0f
#define SPEED       2.5f
#define SENSITIVITY 0.1f
#define ZOOM        45.0f

// Function prototypes
void init_camera(Camera *camera, vec3 position, vec3 up, float yaw, float pitch);
void camera_get_view_matrix(Camera *camera, mat4 view);
void camera_process_keyboard(Camera *camera, Camera_Movement direction, float deltaTime);
void camera_process_mouse_movement(Camera *camera, float xoffset, float yoffset, int constrainPitch);
void camera_process_mouse_scroll(Camera *camera, float yoffset);
void update_camera_vectors(Camera *camera);  // Declaration of update_camera_vectors
void camera_dolly_zoom(Camera* camera, float zoomAmount);
void rotate_camera(Camera* camera, float angle, int vertical);

#endif
