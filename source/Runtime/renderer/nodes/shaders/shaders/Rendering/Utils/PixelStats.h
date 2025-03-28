/***************************************************************************
 # Copyright (c) 2015-23, NVIDIA CORPORATION. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the distribution.
 #  * Neither the name of NVIDIA CORPORATION nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************/
#pragma once
#include "PixelStatsShared.slang"
#include "Core/Macros.h"



#include "Core/Pass/ComputePass.h"

#include "utils/Algorithm/ParallelReduction.h"
#include <memory>

namespace USTC_CG
{
    /** Helper class for collecting runtime stats in the path tracer.

        Per-pixel stats are logged in buffers on the GPU, which are immediately ready for consumption
        after end() is called. These stats are summarized in a reduction pass, which are
        available in getStats() or printStats() after async readback to the CPU.
    */
    class HD_USTC_CG_API PixelStats
    {
    public:
        struct Stats
        {
            uint32_t visibilityRays = 0;
            uint32_t closestHitRays = 0;
            uint32_t totalRays = 0;
            uint32_t pathVertices = 0;
            uint32_t volumeLookups = 0;
            float    avgVisibilityRays = 0.f;
            float    avgClosestHitRays = 0.f;
            float    avgTotalRays = 0.f;
            float    avgPathLength = 0.f;
            float    avgPathVertices = 0.f;
            float    avgVolumeLookups = 0.f;
        };

        PixelStats(ref<Device> pDevice);

        void setEnabled(bool enabled) { mEnabled = enabled; }
        bool isEnabled() const { return mEnabled; }

        void beginFrame(RenderContext* pRenderContext, const uint2& frameDim);
        void endFrame(RenderContext* pRenderContext);

        /** Perform program specialization and bind resources.
            This call doesn't change any resource declarations in the program.
        */
        void prepareProgram(const ref<Program>& pProgram, const ShaderVar& var);

        void renderUI(Gui::Widgets& widget);

        /** Fetches the latest stats generated by begin()/end().
            \param[out] stats The stats are copied here.
            \return True if stats are available, false otherwise.
        */
        bool getStats(PixelStats::Stats& stats);

        /** Returns the per-pixel ray count texture or nullptr if not available.
            \param[in] pRenderContext The render context.
            \return Texture in R32Uint format containing per-pixel ray counts, or nullptr if not available.
        */
        const nvrhi::TextureHandle getRayCountTexture(RenderContext* pRenderContext);

        /** Returns the per-pixel path length texture or nullptr if not available.
            \return Texture in R32Uint format containing per-pixel path length, or nullptr if not available.
        */
        const nvrhi::TextureHandle getPathLengthTexture() const;

        /** Returns the per-pixel path vertex count texture or nullptr if not available.
            \return Texture in R32Uint format containing per-pixel path vertex counts, or nullptr if not available.
        */
        const nvrhi::TextureHandle getPathVertexCountTexture() const;

        /** Returns the per-pixel volume lookup count texture or nullptr if not available.
            \return Texture in R32Uint format containing per-pixel volume lookup counts, or nullptr if not available.
        */
        const nvrhi::TextureHandle getVolumeLookupCountTexture() const;

    protected:
        void copyStatsToCPU();
        void computeRayCountTexture(RenderContext* pRenderContext);

        static const uint32_t kRayTypeCount = (uint32_t)PixelStatsRayType::Count;

        ref<Device>                         mpDevice;

        // Internal state
        std::unique_ptr<ParallelReduction>  mpParallelReduction;            ///< Helper for parallel reduction on the GPU.
        nvrhi::BufferHandle                         mpReductionResult;              ///< Results buffer for stats readback (CPU mappable).
        nvrhi::EventQueryHandle                          mpFence;                        ///< GPU fence for sychronizing readback.

        // Configuration
        bool                                mEnabled = false;               ///< Enable pixel statistics.
        bool                                mEnableLogging = false;         ///< Enable printing to logfile.

        // Runtime data
        bool                                mRunning = false;               ///< True inbetween begin() / end() calls.
        bool                                mWaitingForData = false;        ///< True if we are waiting for data to become available on the GPU.
        uint2                               mFrameDim = { 0, 0 };           ///< Frame dimensions at last call to begin().

        bool                                mStatsValid = false;            ///< True if stats have been read back and are valid.
        bool                                mRayCountTextureValid = false;  ///< True if total ray count texture is valid.
        Stats                               mStats;                         ///< Traversal stats.

        nvrhi::TextureHandle                        mpStatsRayCount[kRayTypeCount]; ///< Buffers for per-pixel ray count stats.
        nvrhi::TextureHandle                        mpStatsRayCountTotal;           ///< Buffer for per-pixel total ray count. Only generated if getRayCountTexture() is called.
        nvrhi::TextureHandle                        mpStatsPathLength;              ///< Buffer for per-pixel path length stats.
        nvrhi::TextureHandle                        mpStatsPathVertexCount;         ///< Buffer for per-pixel path vertex count.
        nvrhi::TextureHandle                        mpStatsVolumeLookupCount;       ///< Buffer for per-pixel volume lookup count.
        bool                                mStatsBuffersValid = false;     ///< True if per-pixel stats buffers contain valid data.

        ref<ComputePass>                    mpComputeRayCount;              ///< Pass for computing per-pixel total ray count.
    };
}
