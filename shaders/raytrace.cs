#version 430

layout (local_size_x = 1, local_size_y = 1) in;

layout (rgba32f, binding = 0) uniform image2D destTex;

struct      Camera {
    vec3    pos;
    vec3    rot;
    float   fov;
    float   reflectDepth;
};

struct      Hit {
    float   dist;
    vec3    impact;
    int     idx;
};

struct      Object {
    float   type;
    vec3    pos;
    vec3    rot;
    vec4    color;
    float   radius;
    float   diffuse;
    float   specular;
    float   shininess;
    float   reflect;
};

struct      Light {
    vec3    pos;
    vec4    color;
};

struct Result {
    float   dist;
    vec3    impact;
    vec3    normal;
    vec3    reflect;
    vec4    color;
    int     idx;
};

uniform Camera  uCamera;

uniform float   uNbObjects;
uniform Object  uObjects[20];

uniform float   uNbLights;
uniform Light   uLights[20];

uniform vec2    uSize;


float linmap(float val, float A, float B, float a, float b) {
    return (val - A)*(b-a)/(B-A) + a;
}

vec3        rx(vec3 v, float angle) {
    vec3    ret;

    if (angle != 0) {
        angle = radians(angle);
        ret.x = v.x;
        ret.y = v.y * cos(angle) - v.z * sin(angle);
        ret.z = v.y * sin(angle) + v.z * cos(angle);
        return ret;
    }
    return v;
}

vec3        ry(vec3 v, float angle) {
    vec3    ret;

    if (angle != 0) {
        angle = radians(angle);
        ret.x = v.x * cos(angle) + v.z * sin(angle);
        ret.y = v.y;
        ret.z = v.z * cos(angle) - v.x * sin(angle);
        return ret;
    }
    return v;
}

vec3        rz(vec3 v, float angle) {
    vec3    ret;

    if (angle != 0) {
        angle = radians(angle);
        ret.x = v.x * cos(angle) - v.y * sin(angle);
        ret.y = v.x * sin(angle) + v.y * cos(angle);
        ret.z = v.z;
        return ret;
    }
    return v;
}

vec3        rotate(vec3 v, vec3 r) {
    v = rx(v, r.x);
    v = ry(v, r.y);
    v = rz(v, r.z);
    return v;
}

vec3    calcDirVector(vec2 fpos) {
    return vec3(
        (uSize.x / 2) - fpos.x,
        (uSize.y / 2) - fpos.y,
        (uSize.x / 2) / tan(radians(uCamera.fov / 2))
    );
}

float       intersectSphere(vec3 camera, vec3 dir, float radius) {
    float   a = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
    float   b = 2 * dir.x * camera.x + 2 * dir.y * camera.y + 2 * dir.z * camera.z;
    float   c = camera.x * camera.x + camera.y * camera.y + camera.z * camera.z - radius * radius;

    float   delta = b * b - 4 * a * c;

    if (delta < 0)
        return -1.0;
    else if (delta > 0) {
        delta = sqrt(delta);
        float root[2];
        root[0] = (-b - delta) / (2 * a);
        root[1] = (-b + delta) / (2 * a);
        if (root[0] > 0)
            if (root[1] > 0)
                return (root[0] > root[1]) ? root[1] : root[0];
            else
                return root[0];
        else
            return (root[1] > 0) ? root[1] : -1.0;
    } else
        return -b / (2 * a);
}

vec3       getNormalSphere(vec3 impact) {
    return impact;
}

float       intersectPlane(vec3 camera, vec3 dir) {
    vec3    normal = vec3(0, 1, 0);
    float   n1 = dot(normal, camera);
    float   n2 = dot(normal, dir);
    float   d;

    if (n2 == 0)
        return -1.0;
    d = -n1 / n2;
    return (d < 0) ? -1.0 : d;

    // if (dir.y == 0)
    //     if (camera.y != 0)
    //         return -1.0;
    //     else
    //         return 0.0;
    // else {
    //     k = -camera.y / dir.y;
    //     return (k < 0) ? -1.0 : k;
    // }
    // k = dot(dir, vec3(0, 1, 0));
    // return (k == 0 ? -1.0 : vec3())
}

vec3    getNormalPlane(vec3 impact) {
    return vec3(0, 1, 0);
}

vec3    prepare(vec3 v, int idx, bool translate) {
    if (translate)
        v -= uObjects[idx].pos;
    v = rz(v, -uObjects[idx].rot.z);
    v = ry(v, -uObjects[idx].rot.y);
    v = rx(v, -uObjects[idx].rot.x);
    return v;
}

vec3    antiPrepare(vec3 v, int idx, bool translate) {
    v = rx(v, uObjects[idx].rot.x);
    v = ry(v, uObjects[idx].rot.y);
    v = rz(v, uObjects[idx].rot.z);
    if (translate)
        v += uObjects[idx].pos;
    return v;
}

float   calcDiffuseComponent(vec3 normal, vec3 surfaceToLight) {
    return max(dot(normal, surfaceToLight), 0.0);
}

float   calcSpecularComponent(vec3 normal, vec3 surfaceToLight, vec3 surfaceToCamera, float shininess, float specular) {
    if (dot(normal, surfaceToLight) < 0.0)
        return 0.0;
    vec3 lightReflect = reflect(-surfaceToLight, normal);
    return pow(max(dot(lightReflect, surfaceToCamera), 0.0), shininess) * specular;
}

Result      raytrace(vec3 camera, vec3 dirVec, int exclude) {
    Result  result;
    Hit     hit;

    hit.dist = -1;
    hit.idx = -1;
    for (int i = 0; i < uNbObjects; i++) {
        if (i == exclude) continue;
        float potentialHit;
        vec3 eye = prepare(camera, i, true);
        vec3 dir = prepare(dirVec, i, false);
        if (uObjects[i].type == 0)
            potentialHit = intersectSphere(eye, dir, uObjects[i].radius);
        else if (uObjects[i].type == 1)
            potentialHit = intersectPlane(eye, dir);
        if (potentialHit != -1.0 && (hit.dist == -1 || potentialHit < hit.dist)) {
            hit.dist = potentialHit;
            hit.impact = eye + dir * hit.dist;
            hit.idx = i;
        }
    }

    if (hit.dist == -1.0) {
        result.dist = -1;
        result.impact = vec3(-1, -1, -1);
        result.color = vec4(0, 0, 0, 1);
        return result;
    }

    result.dist = hit.dist;

    result.color = uObjects[hit.idx].color;

    if (uObjects[hit.idx].type == 0)
        result.normal = getNormalSphere(hit.impact);
    else if (uObjects[hit.idx].type == 1)
        result.normal = getNormalPlane(hit.impact);

    result.normal = normalize(antiPrepare(result.normal, hit.idx, false));
    result.reflect = reflect(dirVec, result.normal);
    result.impact = antiPrepare(hit.impact, hit.idx, true);
    
    vec4 colors[20];

    for (int i = 0; i < uNbLights; i++) {
        vec3 surfaceToLight = normalize(uLights[i].pos - result.impact);
        vec3 surfaceToCamera = normalize(uCamera.pos - result.impact);
        colors[i] = result.color * uLights[i].color * calcDiffuseComponent(result.normal, surfaceToLight)
                          + result.color * uLights[i].color * calcSpecularComponent(result.normal, surfaceToLight, surfaceToCamera, uObjects[hit.idx].shininess, uObjects[hit.idx].specular);
    }

    result.color = vec4(0, 0, 0, 1);
    for (int i = 0; i < uNbLights; i++)
        result.color += colors[i];
    result.color /= uNbLights;

    result.idx = hit.idx;

    return result;
}

void        main() {
    ivec2   pos = ivec2(gl_GlobalInvocationID.xy);
    vec2    fpos = vec2(pos.xy);
    vec3    dirVec = normalize(rotate(normalize(calcDirVector(fpos)), uCamera.rot));
    vec4    color;
    Result  result;
    float   reflectDepth;

    result = raytrace(uCamera.pos, dirVec, -1);
    color = result.color;
    reflectDepth = uCamera.reflectDepth;
    while (result.dist != -1.0 && uObjects[result.idx].reflect != 0 && reflectDepth > 0) {
        reflectDepth -= 1;
        Result tmp = raytrace(result.impact, result.reflect, result.idx);
        color = (color * (1.0 - uObjects[result.idx].reflect)) + (tmp.color * uObjects[result.idx].reflect);
        result = tmp;
    }

    imageStore(destTex, pos, color);
}
