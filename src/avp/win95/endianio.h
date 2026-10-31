#ifndef _included_endianio_h_
#define _included_endianio_h_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include "fixer.h"
#include "datatype.h"

MY_BYTE GetByte(FILE *fp);
MY_WORD GetLittleWord(FILE *fp);
DWORD GetLittleDword(FILE *fp);

VOID PutByte(MY_BYTE v, FILE *fp);
VOID PutLittleWord(MY_WORD v, FILE *fp);
VOID PutLittleDword(DWORD v, FILE *fp);

#ifdef __cplusplus
};
#endif /* __cplusplus */

#endif /* _included_endianio_h_ */

