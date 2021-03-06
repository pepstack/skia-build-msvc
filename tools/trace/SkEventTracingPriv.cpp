/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkEventTracingPriv.h"

#include "SkATrace.h"
#include "SkChromeTracingTracer.h"
#ifndef SKIA_DLL
#include "SkCommandLineFlags.h"
#else
#include "../../include/trace/SkEventTracing.h"
#endif
#include "SkDebugfTracer.h"
#include "SkEventTracer.h"
#include "SkTraceEvent.h"

#ifndef SKIA_DLL
DEFINE_string(trace, "",
              "Log trace events in one of several modes:\n"
              "  debugf     : Show events using SkDebugf\n"
              "  atrace     : Send events to Android ATrace\n"
              "  <filename> : Any other string is interpreted as a filename. Writes\n"
              "               trace events to specified file as JSON, for viewing\n"
              "               with chrome://tracing");

DEFINE_string(traceMatch, "",
              "Filter which categories are traced.\n"
              "Uses same format as --match\n");
#endif

void initializeEventTracingForTools(const char* traceFlag) {
#ifndef SKIA_DLL
    if (!traceFlag) {
        if (FLAGS_trace.isEmpty()) {
            return;
        }
        traceFlag = FLAGS_trace[0];
    }

    SkEventTracer* eventTracer = nullptr;
    if (0 == strcmp(traceFlag, "atrace")) {
        eventTracer = new SkATrace();
    } else if (0 == strcmp(traceFlag, "debugf")) {
        eventTracer = new SkDebugfTracer();
    } else {
        eventTracer = new SkChromeTracingTracer(traceFlag);
    }
#else
    SkEventTracer* eventTracer = nullptr;
    eventTracer = new SkChromeTracingTracer(traceFlag);
#endif
    SkAssertResult(SkEventTracer::SetInstance(eventTracer));
}

#ifdef SKIA_DLL
void SkEventTracing::initializeEventTracingForTools(const char* traceFileName)
{
    ::initializeEventTracingForTools(traceFileName);
}
#endif

uint8_t* SkEventTracingCategories::getCategoryGroupEnabled(const char* name) {
    static_assert(0 == offsetof(CategoryState, fEnabled), "CategoryState");

    // We ignore the "disabled-by-default-" prefix in our internal tools
    if (SkStrStartsWith(name, TRACE_CATEGORY_PREFIX)) {
        name += strlen(TRACE_CATEGORY_PREFIX);
    }

    // Chrome's implementation of this API does a two-phase lookup (once without a lock, then again
    // with a lock. But the tracing macros avoid calling these functions more than once per site,
    // so just do something simple (and easier to reason about):
    SkAutoMutexAcquire lock(&fMutex);
    for (int i = 0; i < fNumCategories; ++i) {
        if (0 == strcmp(name, fCategories[i].fName)) {
            return reinterpret_cast<uint8_t*>(&fCategories[i]);
        }
    }

    if (fNumCategories >= kMaxCategories) {
        SkDEBUGFAIL("Exhausted event tracing categories. Increase kMaxCategories.");
        return reinterpret_cast<uint8_t*>(&fCategories[0]);
    }
#ifndef SKIA_DLL
    fCategories[fNumCategories].fEnabled = SkCommandLineFlags::ShouldSkip(FLAGS_traceMatch, name)
            ? 0 : SkEventTracer::kEnabledForRecording_CategoryGroupEnabledFlags;
#else
    fCategories[fNumCategories].fEnabled = true;
#endif

    fCategories[fNumCategories].fName = name;
    return reinterpret_cast<uint8_t*>(&fCategories[fNumCategories++]);
}

const char* SkEventTracingCategories::getCategoryGroupName(const uint8_t* categoryEnabledFlag) {
    if (categoryEnabledFlag) {
        return reinterpret_cast<const CategoryState*>(categoryEnabledFlag)->fName;
    }
    return nullptr;
}
