struct Payload {
    uint seed;

    bool doesScatter;
    vec3 attenuation;
    vec3 scatterDirection;
    vec3 pointOnSphere;
};

struct Ray {
    vec3 origin;
    vec3 direction;
};

struct Camera {
    float fov;
    float aperture;
    float focusDistance;
    vec3 lookFrom;
    vec3 lookAt;
    vec3 up;
};

struct Viewport {
    vec3 horizontal;
    vec3 vertical;
    vec3 upperLeftCorner;
    vec3 cameraUp;
    vec3 cameraRight;
};

struct Sphere {
    vec4 geometry;
    uint materialType;
    uint textureType;
    vec4 colors[2];
    float materialSpecificAttribute;
};
