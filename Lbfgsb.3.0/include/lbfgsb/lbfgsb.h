// Type definitions from f2c - must match local_f2c.h
#include <stdint.h>

typedef int32_t integer;
typedef double doublereal;
typedef int32_t logical;

#ifdef f2c_i2
typedef int16_t ftnlen;
#else
typedef int32_t ftnlen;
#endif

extern "C" {
int setulb_(integer* n,
            integer* m,
            doublereal* x,
            doublereal* l,
            doublereal* u,
            integer* nbd,
            doublereal* f,
            doublereal* g,
            doublereal* factr,
            doublereal* pgtol,
            doublereal* wa,
            integer* iwa,
            char* task,
            integer* iprint,
            char* csave,
            logical* lsave,
            integer* isave,
            doublereal* dsave,
            ftnlen task_len,
            ftnlen csave_len);
}
