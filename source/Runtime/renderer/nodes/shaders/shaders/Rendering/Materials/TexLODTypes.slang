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

/** Enums for texture level-of-detail -- see TexLODHelpers.slang
*/

#pragma once
#include "utils/HostDeviceShared.slangh"

USTC_CG_NAMESPACE_OPEN_SCOPE

/** This enum is shared between CPU/GPU.
    It enumerates the different the different texture LOD modes.
*/
enum class TexLODMode : uint32_t
{
    Mip0 = 0,
    RayCones = 1,
    RayDiffs = 2,
    Stochastic = 3,
};

FALCOR_ENUM_INFO(TexLODMode, {
    { TexLODMode::Mip0, "Mip0" },
    { TexLODMode::RayCones, "RayCones" },
    { TexLODMode::RayDiffs, "RayDiffs" },
    { TexLODMode::Stochastic, "Stochastic" },
});
FALCOR_ENUM_REGISTER(TexLODMode);

/** This enum is shared between CPU/GPU.
    Both Combo and Unified avoid computing surface spread from the G-buffer.
    Combo uses ray differentials to compute the spread at the first hit and then average edge curvature for all hits after that.
    Unified uses interpolated edge curvatures for the first hit and then average edge curvatures for all hits after that.
*/
enum class RayConeMode : uint32_t
{
    Combo = 0,
    Unified = 1,
};

FALCOR_ENUM_INFO(RayConeMode, {
    { RayConeMode::Combo, "Combo" },
    { RayConeMode::Unified, "Unified" },
});
FALCOR_ENUM_REGISTER(RayConeMode);

USTC_CG_NAMESPACE_CLOSE_SCOPE
