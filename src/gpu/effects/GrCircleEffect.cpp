/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/*
 * This file was autogenerated from GrCircleEffect.fp; do not modify.
 */
#include "GrCircleEffect.h"
#if SK_SUPPORT_GPU
#include "glsl/GrGLSLColorSpaceXformHelper.h"
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLProgramBuilder.h"
#include "SkSLCPP.h"
#include "SkSLUtil.h"
class GrGLSLCircleEffect : public GrGLSLFragmentProcessor {
public:
    GrGLSLCircleEffect() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrCircleEffect& _outer = args.fFp.cast<GrCircleEffect>();
        (void)_outer;
        prevRadius = -1.0;
        fCircleVar = args.fUniformHandler->addUniform(kFragment_GrShaderFlag, kVec4f_GrSLType,
                                                      kDefault_GrSLPrecision, "circle");
        fragBuilder->codeAppendf(
                "float2 prevCenter;\nfloat prevRadius = %f;\nfloat d;\n@if (%d == 2 || %d == 3) "
                "{\n    d = (length((%s.xy - sk_FragCoord.xy) * %s.w) - 1.0) * %s.z;\n} else {\n   "
                " d = (1.0 - length((%s.xy - sk_FragCoord.xy) * %s.w)) * %s.z;\n}\n@if ((%d == 1 "
                "|| %d == 3) || %d == 4) {\n    d = clamp(d, 0.0, 1.0);\n} else {\n    d = d > 0.5 "
                "? 1.0 : 0.0;\n}\n%s = %s * d;\n",
                prevRadius, _outer.edgeType(), _outer.edgeType(),
                args.fUniformHandler->getUniformCStr(fCircleVar),
                args.fUniformHandler->getUniformCStr(fCircleVar),
                args.fUniformHandler->getUniformCStr(fCircleVar),
                args.fUniformHandler->getUniformCStr(fCircleVar),
                args.fUniformHandler->getUniformCStr(fCircleVar),
                args.fUniformHandler->getUniformCStr(fCircleVar), _outer.edgeType(),
                _outer.edgeType(), _outer.edgeType(), args.fOutputColor,
                args.fInputColor ? args.fInputColor : "float4(1)");
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrCircleEffect& _outer = _proc.cast<GrCircleEffect>();
        auto edgeType = _outer.edgeType();
        (void)edgeType;
        auto center = _outer.center();
        (void)center;
        auto radius = _outer.radius();
        (void)radius;
        UniformHandle& circle = fCircleVar;
        (void)circle;

        if (radius != prevRadius || center != prevCenter) {
            SkScalar effectiveRadius = radius;
            if (GrProcessorEdgeTypeIsInverseFill((GrPrimitiveEdgeType)edgeType)) {
                effectiveRadius -= 0.5f;
            } else {
                effectiveRadius += 0.5f;
            }
            pdman.set4f(circle, center.fX, center.fY, effectiveRadius,
                        SkScalarInvert(effectiveRadius));
            prevCenter = center;
            prevRadius = radius;
        }
    }
    SkPoint prevCenter;
    float prevRadius;
    UniformHandle fCircleVar;
};
GrGLSLFragmentProcessor* GrCircleEffect::onCreateGLSLInstance() const {
    return new GrGLSLCircleEffect();
}
void GrCircleEffect::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                           GrProcessorKeyBuilder* b) const {
    b->add32(fEdgeType);
}
bool GrCircleEffect::onIsEqual(const GrFragmentProcessor& other) const {
    const GrCircleEffect& that = other.cast<GrCircleEffect>();
    (void)that;
    if (fEdgeType != that.fEdgeType) return false;
    if (fCenter != that.fCenter) return false;
    if (fRadius != that.fRadius) return false;
    return true;
}
GrCircleEffect::GrCircleEffect(const GrCircleEffect& src)
        : INHERITED(src.optimizationFlags())
        , fEdgeType(src.fEdgeType)
        , fCenter(src.fCenter)
        , fRadius(src.fRadius) {
    this->initClassID<GrCircleEffect>();
}
std::unique_ptr<GrFragmentProcessor> GrCircleEffect::clone() const {
    return std::unique_ptr<GrFragmentProcessor>(new GrCircleEffect(*this));
}
GR_DEFINE_FRAGMENT_PROCESSOR_TEST(GrCircleEffect);
#if GR_TEST_UTILS
std::unique_ptr<GrFragmentProcessor> GrCircleEffect::TestCreate(GrProcessorTestData* testData) {
    SkPoint center;
    center.fX = testData->fRandom->nextRangeScalar(0.f, 1000.f);
    center.fY = testData->fRandom->nextRangeScalar(0.f, 1000.f);
    SkScalar radius = testData->fRandom->nextRangeF(0.f, 1000.f);
    GrPrimitiveEdgeType et;
    do {
        et = (GrPrimitiveEdgeType)testData->fRandom->nextULessThan(kGrProcessorEdgeTypeCnt);
    } while (kHairlineAA_GrProcessorEdgeType == et);
    return GrCircleEffect::Make(et, center, radius);
}
#endif
#endif
