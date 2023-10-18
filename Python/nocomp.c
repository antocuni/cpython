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
