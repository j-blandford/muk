#include <libcxxrt/unwind.h>
#include <kernel/cpu.hpp>

struct ProcessorContext {
   unsigned long genericRegister[16];
   unsigned long instructionPointer;
};

struct _Unwind_Context {
   ProcessorContext processorContext;
};

void _Unwind_Resume(struct _Unwind_Exception*) {
   assert(false);
}

_Unwind_Reason_Code _Unwind_RaiseException(struct _Unwind_Exception*) {
   assert(false);
   return _URC_FATAL_PHASE1_ERROR;
}

unsigned long _Unwind_GetTextRelBase(struct _Unwind_Context*) {
   return 0;
}

unsigned long _Unwind_GetDataRelBase(struct _Unwind_Context *) {
   return 0;
}

unsigned long _Unwind_GetLanguageSpecificData(struct _Unwind_Context*) {
   return 0;
}

unsigned long _Unwind_GetRegionStart(struct _Unwind_Context*) {
   return 0;
}

_Unwind_Reason_Code _Unwind_Resume_or_Rethrow(struct _Unwind_Exception* e) {
   /* XXX Currently only C++ exception unwinding is supported */
   return _Unwind_RaiseException(e);
}

void _Unwind_SetGR(struct _Unwind_Context* context, int index,
      unsigned long value) {
   context->processorContext.genericRegister[index] = value;
}

void _Unwind_SetIP(struct _Unwind_Context* context, unsigned long value) {
   context->processorContext.instructionPointer = value;
}

// _Unwind_Reason_Code _Unwind_Backtrace(_Unwind_Trace_Fn, void*) {
//    /* XXX Implement this function.
//     *
//     * This function is mainly used for debugging, not a big problem to
//     * simply return an error.
//     */
//    return _URC_FATAL_PHASE1_ERROR;
// }

unsigned long _Unwind_GetIP(struct _Unwind_Context* context) {
   return context->processorContext.instructionPointer;
}