#include <stdbool.h>

#include "Python.h"

#include "pycore_ast.h"           // PyAST_mod2obj()
#include "pycore_ceval.h"         // _Py_EnterRecursiveCall()
#include "pycore_compile.h"       // _PyAST_Compile()
#include "pycore_interp.h"        // PyInterpreterState.importlib
#include "pycore_object.h"        // _PyDebug_PrintTotalRefs()
//#include "pycore_parser.h"        // _PyParser_ASTFromString()
#include "pycore_pyerrors.h"      // _PyErr_GetRaisedException()
#include "pycore_pylifecycle.h"   // _Py_FdIsInteractive()
#include "pycore_pystate.h"       // _PyInterpreterState_GET()
#include "pycore_pythonrun.h"     // export _PyRun_InteractiveLoopObject()
#include "pycore_sysmodule.h"     // _PySys_Audit()
#include "pycore_traceback.h"     // _PyTraceBack_Print_Indented()

#include "errcode.h"              // E_EOF
#include "marshal.h"              // PyMarshal_ReadLongFromFile()


// [antocuni] hack hack hack
char *
PyOS_Readline(FILE *sys_stdin, FILE *sys_stdout, const char *prompt)
{
    PyErr_SetString(PyExc_ValueError, "readline not available");
    return NULL;
}


mod_ty
_PyParser_ASTFromString(const char *str, PyObject* filename, int mode,
                        PyCompilerFlags *flags, PyArena *arena)

{
    PyErr_SetString(PyExc_ValueError, "_PyParser_ASTFromString not available");
    return NULL;
}


mod_ty
_PyParser_ASTFromFile(FILE *fp, PyObject *filename_ob, const char *enc,
                      int mode, const char *ps1, const char* ps2,
                      PyCompilerFlags *flags, int *errcode, PyArena *arena)
{
    PyErr_SetString(PyExc_ValueError, "_PyParser_ASTFromFile not available");
    return NULL;
}

char *
_PyTokenizer_FindEncodingFilename(int fd, PyObject *filename) {
    // this assumes it's UTF-8
    return NULL;
}


Py_ssize_t
_PyPegen_byte_offset_to_character_offset(PyObject *line, Py_ssize_t col_offset)
{
    const char *str = PyUnicode_AsUTF8(line);
    if (!str) {
        return -1;
    }
    Py_ssize_t len = strlen(str);
    if (col_offset > len + 1) {
        col_offset = len + 1;
    }
    assert(col_offset >= 0);
    PyObject *text = PyUnicode_DecodeUTF8(str, col_offset, "replace");
    if (!text) {
        return -1;
    }
    Py_ssize_t size = PyUnicode_GET_LENGTH(text);
    Py_DECREF(text);
    return size;
}

// this is a fake & empty module
static struct PyModuleDef _tokenizemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_tokenize",
};

PyMODINIT_FUNC
PyInit__tokenize(void)
{
    return PyModuleDef_Init(&_tokenizemodule);
}

// this is a fake & empty module
static struct PyModuleDef _astmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_ast",
};

PyMODINIT_FUNC
PyInit__ast(void)
{
    return PyModuleDef_Init(&_astmodule);
}



// this magic defines the _PyOpcode_Deopt array. It is normally done by
// compile.c, but since it's not included in nocomp build, we redo the magic
// here.
#include "opcode.h"

#define NEED_OPCODE_METADATA
#include "pycore_opcode_metadata.h" // _PyOpcode_opcode_metadata, _PyOpcode_num_popped/pushed
#undef NEED_OPCODE_METADATA

int PyAST_Check(PyObject* obj) {
    return 0;
}

mod_ty PyAST_obj2mod(PyObject* ast, PyArena* arena, int mode) {
    PyErr_SetString(PyExc_ValueError, "PyAST_obj2mod not available");
    return NULL;
}

PyObject* PyAST_mod2obj(mod_ty t)
{
    PyErr_SetString(PyExc_ValueError, "PyAST_mod2obj not available");
    return NULL;
}

int _PyAST_Validate(mod_ty) {
    return 0;
}

int
_PyCompile_AstOptimize(mod_ty mod, PyObject *filename, PyCompilerFlags *cf,
                       int optimize, PyArena *arena)
{
    return 0;
}

PyObject *
_PyCompile_CleanDoc(PyObject *doc)
{
    PyErr_SetString(PyExc_ValueError, "_PyCompile_CleanDoc not available");
    return NULL;
}

PyObject *
_PyCompile_OptimizeCfg(PyObject *instructions, PyObject *consts, int nlocals)
{
    PyErr_SetString(PyExc_ValueError, "_PyCompile_OptimizeCfg not available");
    return NULL;
}

PyObject *
_PyCompile_CodeGen(PyObject *ast, PyObject *filename, PyCompilerFlags *pflags,
                   int optimize, int compile_mode)
{
    PyErr_SetString(PyExc_ValueError, "_PyCompile_CodeGen not available");
    return NULL;
}

PyCodeObject *
_PyCompile_Assemble(_PyCompile_CodeUnitMetadata *umd, PyObject *filename,
                    PyObject *instructions)
{
    PyErr_SetString(PyExc_ValueError, "_PyCompile_Assemble not available");
    return NULL;
}

PyCodeObject *
_PyAST_Compile(mod_ty mod, PyObject *filename, PyCompilerFlags *pflags,
               int optimize, PyArena *arena)
{
    PyErr_SetString(PyExc_ValueError, "_PyAST_Compile not available");
    return NULL;
}

PyObject *
_PyAST_GetDocString(asdl_stmt_seq *body)
{
    PyErr_SetString(PyExc_ValueError, "_PyAST_GetDocString not available");
    return NULL;
}

void _PyAST_Fini(PyInterpreterState *interp)
{
}

int
_PyAST_Optimize(mod_ty mod, PyArena *arena, int optimize, int ff_features)
{
    return 0;
}

/******************** extracts from compile.c ******************/

//#define NEED_OPCODE_TABLES
#include "pycore_opcode_utils.h"
//#undef NEED_OPCODE_TABLES




/* Return the stack effect of opcode with argument oparg.

   Some opcodes have different stack effect when jump to the target and
   when not jump. The 'jump' parameter specifies the case:

   * 0 -- when not jump
   * 1 -- when jump
   * -1 -- maximal
 */
static int
stack_effect(int opcode, int oparg, int jump)
{
    if (0 <= opcode && opcode <= MAX_REAL_OPCODE) {
        if (_PyOpcode_Deopt[opcode] != opcode) {
            // Specialized instructions are not supported.
            return PY_INVALID_STACK_EFFECT;
        }
        int popped, pushed;
        if (jump > 0) {
            popped = _PyOpcode_num_popped(opcode, oparg, true);
            pushed = _PyOpcode_num_pushed(opcode, oparg, true);
        }
        else {
            popped = _PyOpcode_num_popped(opcode, oparg, false);
            pushed = _PyOpcode_num_pushed(opcode, oparg, false);
        }
        if (popped < 0 || pushed < 0) {
            return PY_INVALID_STACK_EFFECT;
        }
        if (jump >= 0) {
            return pushed - popped;
        }
        if (jump < 0) {
            // Compute max(pushed - popped, alt_pushed - alt_popped)
            int alt_popped = _PyOpcode_num_popped(opcode, oparg, true);
            int alt_pushed = _PyOpcode_num_pushed(opcode, oparg, true);
            if (alt_popped < 0 || alt_pushed < 0) {
                return PY_INVALID_STACK_EFFECT;
            }
            int diff = pushed - popped;
            int alt_diff = alt_pushed - alt_popped;
            if (alt_diff > diff) {
                return alt_diff;
            }
            return diff;
        }
    }

    // Pseudo ops
    switch (opcode) {
        case POP_BLOCK:
        case JUMP:
        case JUMP_NO_INTERRUPT:
            return 0;

        case EXIT_INIT_CHECK:
            return -1;

        /* Exception handling pseudo-instructions */
        case SETUP_FINALLY:
            /* 0 in the normal flow.
             * Restore the stack position and push 1 value before jumping to
             * the handler if an exception be raised. */
            return jump ? 1 : 0;
        case SETUP_CLEANUP:
            /* As SETUP_FINALLY, but pushes lasti as well */
            return jump ? 2 : 0;
        case SETUP_WITH:
            /* 0 in the normal flow.
             * Restore the stack position to the position before the result
             * of __(a)enter__ and push 2 values before jumping to the handler
             * if an exception be raised. */
            return jump ? 1 : 0;

        case STORE_FAST_MAYBE_NULL:
            return -1;
        case LOAD_CLOSURE:
            return 1;
        case LOAD_METHOD:
            return 1;
        case LOAD_SUPER_METHOD:
        case LOAD_ZERO_SUPER_METHOD:
        case LOAD_ZERO_SUPER_ATTR:
            return -1;
        default:
            return PY_INVALID_STACK_EFFECT;
    }

    return PY_INVALID_STACK_EFFECT; /* not reachable */
}

int
PyCompile_OpcodeStackEffectWithJump(int opcode, int oparg, int jump)
{
    return stack_effect(opcode, oparg, jump);
}

int
PyCompile_OpcodeStackEffect(int opcode, int oparg)
{
    return stack_effect(opcode, oparg, -1);
}

int
PyUnstable_OpcodeIsValid(int opcode)
{
    return IS_VALID_OPCODE(opcode);
}

int
PyUnstable_OpcodeHasArg(int opcode)
{
    return OPCODE_HAS_ARG(opcode);
}

int
PyUnstable_OpcodeHasConst(int opcode)
{
    return OPCODE_HAS_CONST(opcode);
}

int
PyUnstable_OpcodeHasName(int opcode)
{
    return OPCODE_HAS_NAME(opcode);
}

int
PyUnstable_OpcodeHasJump(int opcode)
{
    return OPCODE_HAS_JUMP(opcode);
}

int
PyUnstable_OpcodeHasFree(int opcode)
{
    return OPCODE_HAS_FREE(opcode);
}

int
PyUnstable_OpcodeHasLocal(int opcode)
{
    return OPCODE_HAS_LOCAL(opcode);
}

int
PyUnstable_OpcodeHasExc(int opcode)
{
    return IS_BLOCK_PUSH_OPCODE(opcode);
}
