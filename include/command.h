#ifndef _COMMAND_H_
#define _COMMAND_H_

typedef enum {
    ARGUMENT_STRING = 0,
    ARGUMENT_INTEGER
} ARGUMENT_TYPE;

typedef struct _ARGUMENT {
    ULONG Unknown0;
    ULONG Unknown1;
    LONG Unknown2;
    ARGUMENT_TYPE Type;
    PUCHAR String;
} ARGUMENT, *PARGUMENT;

typedef
ULONG
(*COMMAND_FUNCTION)(
    int argc,
    PARGUMENT argv
    );
    
typedef struct _COMMAND_INFO {
    PCHAR Name;
    COMMAND_FUNCTION Handler;
    PCHAR Description;
} COMMAND_INFO, *PCOMMAND_INFO;



extern PVOID CommandListBegin, CommandListEnd;
extern PCOMMAND_INFO* CommandList;

VOID
FwCmdAdd(
    PCHAR Name,
    COMMAND_FUNCTION Handler,
    PCHAR Description
    );
    
ULONG
FwCmdlineInitialize(
    VOID
    );



#endif
