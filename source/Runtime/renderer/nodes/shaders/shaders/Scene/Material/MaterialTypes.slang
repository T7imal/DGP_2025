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
#include "utils/HostDeviceShared.slangh"

USTC_CG_NAMESPACE_OPEN_SCOPE

/** Material type.
*/
// TODO: Specify the backing type when Slang issue has been resolved
enum class MaterialType // : uint32_t
{
    Unknown,    ///< Unknown material type, e.g. type is not yet registered.

    Standard,
    Cloth,
    Hair,
    MERL,
    MERLMix,
    PBRTDiffuse,
    PBRTDiffuseTransmission,
    PBRTConductor,
    PBRTDielectric,
    PBRTCoatedConductor,
    PBRTCoatedDiffuse,
    RGL,
    BuiltinCount // Must be last
};

/** Alpha mode. This specifies how alpha testing should be done.
*/
enum class AlphaMode
{
    Opaque,     ///< No alpha test.
    Mask,       ///< Alpha mask.

    Count // Must be last
};

/** Shading models supported by the standard material.
*/
enum class ShadingModel
{
    MetalRough,
    SpecGloss,

    Count // Must be last
};

/** Normal map type. This specifies the encoding of a normal map.
*/
enum class NormalMapType
{
    None,       ///< Normal map is not used.
    RGB,        ///< Normal encoded in RGB channels in [0,1].
    RG,         ///< Tangent space encoding in RG channels in [0,1].

    Count // Must be last
};

USTC_CG_NAMESPACE_CLOSE_SCOPE
