//
// Copyright 2020 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#ifndef Hd_USTC_CG_MESH_H
#define Hd_USTC_CG_MESH_H

#include "../DescriptorTableManager.h"
#include "../api.h"
#include "internal/memory/DeviceMemoryPool.hpp"
#include "nvrhi/nvrhi.h"
#include "pxr/base/gf/matrix4f.h"
#include "pxr/imaging/garch/glApi.h"
#include "pxr/imaging/hd/mesh.h"
#include "pxr/imaging/hd/vertexAdjacency.h"
#include "pxr/pxr.h"
// SceneTypes
#include "../nodes/shaders/shaders/Scene/SceneTypes.slang"
#include "internal/memory/DeviceMemoryPool.hpp"
USTC_CG_NAMESPACE_OPEN_SCOPE
class Hd_USTC_CG_RenderParam;
using namespace pxr;

class HD_USTC_CG_API Hd_USTC_CG_Mesh final : public HdMesh {
   public:
    HF_MALLOC_TAG_NEW("new Hd_USTC_CG_Mesh");

    Hd_USTC_CG_Mesh(const SdfPath& id);

    ~Hd_USTC_CG_Mesh() override;

    HdDirtyBits GetInitialDirtyBitsMask() const override;
    void Sync(
        HdSceneDelegate* sceneDelegate,
        HdRenderParam* renderParam,
        HdDirtyBits* dirtyBits,
        const TfToken& reprToken) override;

    void Finalize(HdRenderParam* renderParam) override;

    nvrhi::rt::AccelStructHandle BLAS;

   protected:
    DeviceMemoryPool<unsigned>::MemoryHandle indexBuffer;

    DeviceMemoryPool<float>::MemoryHandle vertexBuffer;
    DeviceMemoryPool<float>::MemoryHandle texcoordBuffer;
    DeviceMemoryPool<float>::MemoryHandle normalBuffer;

    DeviceMemoryPool<GeometryInstanceData>::MemoryHandle instanceBuffer;
    DeviceMemoryPool<nvrhi::rt::InstanceDesc>::MemoryHandle rt_instanceBuffer;
    DeviceMemoryPool<MeshDesc>::MemoryHandle mesh_desc_buffer;
    DeviceMemoryPool<nvrhi::DrawIndexedIndirectArguments>::MemoryHandle
        draw_indirect;

    GfMatrix4f transform;
    VtArray<GfVec3i> triangulatedIndices;
    VtIntArray trianglePrimitiveParams;
    VtArray<GfVec3f> points;
    VtVec3fArray computedNormals;
    static constexpr GLuint normalLocation = 1;
    static constexpr GLuint texcoordLocation = 2;

    void create_gpu_resources(Hd_USTC_CG_RenderParam* render_param);
    void updateTLAS(
        Hd_USTC_CG_RenderParam* render_param,
        HdSceneDelegate* sceneDelegate,
        HdDirtyBits* dirtyBits);

    uint32_t _dirtyBits;
    bool _texcoordsClean;

    void _InitRepr(const TfToken& reprToken, HdDirtyBits* dirtyBits) override;
    void _SetMaterialId(
        HdSceneDelegate* scene_delegate,
        Hd_USTC_CG_Mesh* hd_ustc_cg_mesh);

    HdDirtyBits _PropagateDirtyBits(HdDirtyBits bits) const override;
    TfTokenVector _UpdateComputedPrimvarSources(
        HdSceneDelegate* sceneDelegate,
        HdDirtyBits dirtyBits);
    void _UpdatePrimvarSources(
        HdSceneDelegate* sceneDelegate,
        HdDirtyBits dirtyBits,
        HdRenderParam* param);

    // This class does not support copying.
    Hd_USTC_CG_Mesh(const Hd_USTC_CG_Mesh&) = delete;
    Hd_USTC_CG_Mesh& operator=(const Hd_USTC_CG_Mesh&) = delete;

   private:
    HdCullStyle _cullStyle;
    bool _doubleSided;

    bool _normalsValid;
    bool _adjacencyValid;

    HdMeshTopology topology;
    Hd_VertexAdjacency _adjacency;

    bool _refined;

    // A local cache of primvar scene data. "data" is a copy-on-write handle to
    // the actual primvar buffer, and "interpolation" is the interpolation mode
    // to be used. This cache is used in _PopulateRtMesh to populate the
    // primvar sampler map in the prototype context, which is used for shading.
    struct PrimvarSource {
        VtValue data;
        HdInterpolation interpolation;
    };

    TfHashMap<TfToken, PrimvarSource, TfToken::HashFunctor> _primvarSourceMap;
};

USTC_CG_NAMESPACE_CLOSE_SCOPE

#endif  // Hd_USTC_CG_MESH_H
