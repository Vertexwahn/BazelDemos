#include "embree3/rtcore.h"

#include <pmmintrin.h>
#include <xmmintrin.h> // SIMD Extensions intrinsics

#include "gmock/gmock.h"

TEST(Embree, Embree_initialization) {
    // Setup SIMD Extensions intrinsics for EmbreeIntersector
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);

    // create EmbreeIntersector device at application startup
    RTCDevice device = rtcNewDevice(nullptr);
    EXPECT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);

    // create scene
    RTCScene scene = rtcNewScene(device);
    ASSERT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);
    ASSERT_TRUE(scene);

    // application vertex and index layout
    struct Vertex   { float x, y, z, a; };
    struct Triangle { int v0, v1, v2, materialID; };

    Vertex v[3];
    v[0].x = -10; v[0].y = -10; v[0].z = 10;
    v[1].x =  20; v[1].y = -10; v[1].z = 10;
    v[2].x =  20; v[2].y =  20; v[2].z = 10;

    Triangle t[1];
    t[0].v0 = 0;
    t[0].v1 = 1;
    t[0].v2 = 2;

    Vertex *vertexPtr = v;
    Triangle *indexPtr = t;

    // create triangle mesh
    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
    ASSERT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);
    ASSERT_TRUE(geom);

    // share data buffers
    size_t vertexCount = 3;
    size_t indexCount = 3;
    rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX, 0,
        RTC_FORMAT_FLOAT3, vertexPtr, 0, sizeof(Vertex), vertexCount);
    ASSERT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);
    rtcSetSharedGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX, 0,
        RTC_FORMAT_UINT3, indexPtr, sizeof(Vertex) * vertexCount, sizeof(Triangle), indexCount);
     ASSERT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);

    // commit geometry
    rtcCommitGeometry(geom);
    // attach geometry to scene
    unsigned int geomID = 1;
    rtcAttachGeometryByID(scene, geom, geomID);
    ASSERT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);

    // commit changes
    rtcCommitScene(scene);
    ASSERT_THAT(rtcGetDeviceError(device), RTC_ERROR_NONE);
/*
    RTCRay ray;
    ray.org[0] = 0.0;
    ray.org[1] = 0.0;
    ray.org[2] = 0.0;

    ray.dir[0] = 0.0;
    ray.dir[1] = 0.0;
    ray.dir[2] = 20.0;

    ray.tnear = 0.0;
    ray.tfar = 1.0;
    ray.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect(scene, ray);
    EXPECT_THAT(rtcDeviceGetError(device), RTC_NO_ERROR);
*/
    // release objects
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}
