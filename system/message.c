/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include "..\compiler.h"
/******************************************************************************/
int errorCnt, warnCnt, todoCnt;
BOOL COMPILE_SUCCESS;
/******************************************************************************/
char* szErrors[] = {
    "#: %s",
    "Invalid expression: " QSTR "",
    "Misplaced expression: " QSTR "",
    "Opening brace (\"{\") not allowed here",
    "Closing brace (\"}\") not allowed here",
    "Closing brackets (\")\") must have equal opening brackets (\"(\")",
    "Closing bracket (\")\") expected",
    "Closing brace (\"}\") expected",
    "Invalid char code \" \\%c \"",
    "Char end quote (\" ' \") expected, got " QCHAR "",
    "Script number not specified",
    "Script number already defined as: %d",
    "Resource number %d out of range! Must be between 0 and %d",
    "Integer expected",
    "Invalid preprocessor directive: \"#%s\"",
    "Illegal integer! " QSTR " can not be trailed with " QCHAR " or any alpha char",
    "Illegal character 0x%02X (" QCHAR ")",
    "Unterminated string " QSTR ". End quote (\'\"\') expected",
    "Expression \"%s...\" too long!",
    "String \"%s...\" too large!",
    "String enclosed in quotes ('\"') expected",
    "" QSTR ", already undefined or not previously defined",
    "Unterminated #ifdef or #ifndef expression",
    "Semicolon (\";\") expected. Wanted \";\", got " QSTR "",
    "Elements of enumeration must be contained in braces (\"{\", \"}\")",
    "Label name " QSTR " already defined as a variable",
    "Label name " QSTR " already defined as a function",
    "Label name " QSTR " already defined",
    "" QSTR " Is not a valid label name. Labels must contain only alphanumeric characters and/or \"_\" underscores",
    "Kernel function prototype declaration expected. Wanted typecast, got " QSTR "",
    "Kernel function prototype declaration expected. Wanted \"(\", got " QSTR "",
    "Pointer declaration too deep",
    "Unterminated opening brackets \'(\'!",
    "Integer " QSTR " too large!",
    "Array size: %d bytes, too small!",
    "Array size: %d bytes, too large!",
    "Array end character expected. Wanted (\"]\"), got " QSTR "",
    "Pointer variable declarations outside of code blocks can not have preassigned values",
    "Elements of array " QSTR ", must be separated by commas (\",\")",
    "Elements of array " QSTR ", must be contained in braces (\"{\", \"}\")",
    "Variable " QSTR ", is not an array and can not store multiple elements",
    "Elements of array " QSTR ", must be terminated with a closing brace. Wanted \"}\", got " QSTR "",
    "Integer " QSTR " out of range. It must be between -32768 and +32767",
    "Variables of typecast " QSTR " must be pointers",
    "Elements of array " QSTR ", exceed the initiated size by %d bytes",
    "Array elements of string " QSTR ", must be strings, chars, or integers (0-255), not " QSTR "",
    "Illegal string character 0x%02X, in string \"%s...\"",
    "Variable typecast expected. Wanted typecast, got " QSTR ".",
    "Function parameter declaration of: %s, not terminated",
    "Functions can not be declared within other functions",
    "Superclass name expected! " QSTR " is not a class!",
    "Objects can not be declared within other objects!",
    "Objects can not be declared within functions!",
    "Temp variables of function " QSTR ", too large in size! Trying to kill the stack? Not on my watch!",
    "Invalid label! " QSTR " is a reserved word",
    "Assignments can only be made on variables",
    "If statements require a condition enclosed in brackets. Wanted \"(\", got " QSTR "",
    "Invalid conditional expression. Wanted condition, got " QSTR "",
    "If statements must be enclosed in \"{\" and \"}\"",
    "Do statements must have a while. Wanted \"while\", got " QSTR "",
    "Void functions can not return a value",
    "Case declarations must end with a colon. Wanted \":\", got " QSTR "",
    "Property or method name expected. " QSTR " is not a member of " QSTR "",
    "Send to object " QSTR ", too large",
    "Switch statements must be initiated with a '{' brace",
    "Switch statements must be terminated with a '}' brace",
    "Function calls must have a parameter list enclosed in brackets. Wanted \"(\", got " QSTR "",
    "Function %s terminated with too few parameters. Expected parameter " QSTR "",
    "Comma to separate parameters of function " QSTR " required. Wanted \",\", got " QSTR "",
    "Function " QSTR ", parameter declaration must be terminated with a closing bracket. Wanted \")\", got " QSTR "",
    "Too many parameters sent to the function " QSTR "",
    "Array element index " QSTR " out of range",
    "Variable " QSTR ", is not a pointer and can not be indexed",
    "Sends can only be made on objects",
    "Send to object " QSTR ", not terminated!",
    "Object header property " QSTR " is read only!",
    "Invalid send! " QSTR " is not a member of object " QSTR "",
    "No send object accessable within the current function. Function has no class linked to it",
    "\"public\" keyword can not be used on kernel function prototypes",
    "\"public\" keyword can not be used on variable declarations! Public variables are automatically those of script.65535",
    "Total public objects/functions exceed the maximum",
    "Object file too large!",
    "Unable to save unit object file for " QSTR "",
    "Unable to load unit object file: " QSTR "",
    "Invalid object file: " QSTR "",
    "Object file " QSTR " is not a compatible version",
    "Too many parameters in call to " QSTR "! Wanted \")\", got " QSTR "",
    "Illegal binary integer declaration " QSTR "! Only '1' and '0' may be used",
    "Unimplemented keyword",
    "Invalid alignment value: %d",
    "Variable declarations must be made within banks! No bank active",
    "Label declarations must be made within banks! No bank active",
    "Preprocessor keyword " QSTR ", requires a '.' followed by a member name",
    "Unkown mapper board type name: " QSTR "",
    "Invalid mirroring type name: " QSTR ". Expected \"vertical\" or \"horizontal\"",
    "Function " QSTR " is being declared in a RAM bank. RAM data can only be declared at runtime",

    "Function declarations must have parameter block enclosed in \"(\" and \")\". Wanted \"(\", got " QSTR "",
    "Function parameter block not terminated. Wanted \")\", got " QSTR "",
    "Function " QSTR " not inline! Only inline functions can have parameters",

    "Invalid operand(s) passed to opcode " QSTR "",
    "Unterminated integer expression",

    "New line expected after opcode",

    "Opcode " QSTR " operands can not be immediate",
    "Opcode " QSTR " operands can not be indirect",
    "Opcode " QSTR " operands can not be indexed",
    "Invalid index register on opcode " QSTR ". Wanted X or Y, got " QSTR "",
    "Closing brace (\"]\") on opcode " QSTR ", misplaced",
    "Operand of opcode " QSTR " must have a closing brace! Wanted \"]\", got " QSTR "",
    "Operand of opcode " QSTR " must be a zero page address",
    "Opcode " QSTR " needs an operand",

    "Label or function name expected! " QSTR " is not a valid pointer",
    "Branch out of range",

    "" QSTR " is not yet defined, it's value is unknown",
    "" QSTR " is not an integer",

    "" QSTR " is not a member of struct " QSTR "",
    "Declaration not allowed within struct",

    "Variable " QSTR " is not a BYTE or CHAR type. Only byte sized variables can be assigned strings",

    "Variables in RAM banks can not be initialized with predefined values",
    "Only ROM banks are allowed to be initialized with predefined values",

    "Unable to open file " QSTR " for reading",
    "Unable to open file " QSTR " for writing",
    "Incbin file " QSTR " is %d bytes in length, exceeding the maximum defined size of %d bytes",
    "Incbin file " QSTR " is %d bytes in length, too large for current bank, which only has %d bytes free",

    "Unknown request " QSTR "",

    "Interrupt name expected! Wanted interrupt, got " QSTR "",
    "Interrupt vectors not within range of current bank " QSTR "",

    "Unexpected integer " QSTR "! \"+\",\"-\" or other combine opertator expected",

    "Misplaced use of function " QSTR ". Inline functions can not be accessed by address or jumped to",

    "Inline function " QSTR "'s definition is too large! The maximum macro definition size is %d bytes",

    "\"IF\" statement branches outside of bank! It begins in bank " QSTR " and ends in bank " QSTR "",
    "\"WHILE\" statement branches outside of bank! It begins in bank " QSTR " and ends in bank " QSTR "",
    "\"FOREVER\" statement branches outside of bank! It begins in bank " QSTR " and ends in bank " QSTR "",

    "Invalid conditional expression: " QSTR "",
    "If statements must have a condition enclosed in brackets. Wanted \"(\", got " QSTR "",

    "Typedefs can only be made on variable types! " QSTR " is not a variable type",
    "sizeof can only be used on variables or variable types! " QSTR " is not a variable or variable type",
    "Variable " QSTR " is a typedef, and can not be assigned values",

    "Invalid array index value for variable " QSTR ", must be an immediate integer value",

    "Only IF statements can have ELSE blocks",

    "Declaration of function " QSTR " not terminated! Wanted '}'",
    "Declaration of struct " QSTR " not terminated! Wanted '}'",

    "Enum class member name expected for enum class " QSTR "! Need \".\", followed by a member name, got " QCHAR "",
    "Enum class member name expected! " QSTR " is not a member of enum class " QSTR "",

    "\"#else\" preprocessor directive unexpected",

    "Self nested define " QSTR ", not allowed",
    "Self nested source include " QSTR ", not allowed",

    "Error adding path " QSTR " to list",

    "Variable " QSTR ", is not an array!",

    "Function " QSTR " requires parameters",
    "Function " QSTR " requires parameter " QSTR "",

    "Bank size for " QSTR " can not be zero",

    "Inline functions can not use the \"noreturn\" keyword",

    "Switch statements require register to be evaluated enclosed in brackets. Wanted \"(\", got " QSTR "",
    "Switch \"REG.*\" evaluator expected! Got " QSTR "",
    "Switch \"REG\" evaluator requires a '.' followed by the register to be evaluated! Got " QSTR "",
    "Switch \"REG.\" evaluator must be followed by the register to be evaluated! Wanted \"A\", \"X\", or \"Y\", Got " QSTR "",
    "Case expression must be followed by code",
    "Expected \"case\", \"default\" or \"}\" in switch block. Got " QSTR "",
    "Unterminated switch block",

    "8 bit fix up address " QSTR ", out of range",

    "Only interrupt functions can have a subtype",
    "Invalid subfunction type: " QSTR "",

    "Bank size: %d bytes exceeds the maximum bank size which is %d bytes",

    "Attempted to assign " QSTR " as an incorrect interrupt vector! Wanted " QSTR ", got " QSTR "",

    "Label: " QSTR ", not previously declared. Labels used with arithmatic operations must be declared prior to use",

    "Struct elements must be contained in braces (\'{\',\'}\') for declaration. Expecting in brace (\"{\"), got " QSTR "",
    "Elements of struct " QSTR ", must be separated by commas (\",\")",

    "sizeof keyword can not be used on inline functions (" QSTR ")",
    "bankof keyword can not be used on inline functions (" QSTR ")",
    "bankof keyword can not be used on variables (todo) (" QSTR ")",
    "bankof can not be used on " QSTR "",

    "Opcode " QSTR " can only be indexed for zero page addresses",
};

char* szFatal[] = {
    "#: %s",
    "Unable to allocate %d bytes of memory",
    "Compile: FAILED",
    "Output script resource too large",
    "Unable to save resource file: " QSTR "",
    "Unable to open file " QSTR " for reading",
    "Unable to open file " QSTR " for writing",
    "Too many unterminated brackets",
    "Label buffer out of memory. Failed to add label: %s",
    "Code not terminated",
    "No bank currently active!",
    "Total banks exceed maximum allowed!",
    "Data in bank " QSTR ", is %d byte in size, which exceeds the maximum size of %d bytes!",
    "Data in bank " QSTR ", is %d byte in size, which exceeds the maximum size of %d bytes, overlapping with the interrupt vectors",
    "Active bank " QSTR ", is not a ROM bank!",
    "Active bank " QSTR ", is not a CHR bank!",
    "Variable declarations can not be made outside of RAM or ROM",
    "Too many unterminated braces",
    "Unable to open file: " QSTR ", for writing",
    "Function error",
};

char* szWarnings[] = {
    "#: %s",
    "Integer larger than 8bits, conversion will lose significant digits", //"Uncasted integer larger than 16bits, converting to long",
    "Integer larger than 16bits, conversion will lose significant digits", //"Uncasted integer larger than 16bits, converting to long",
    "Function " QSTR " is being declared in a CHR bank",
    "Integer division by zero",
    "Array index of variable " QSTR " out of bounds! Wanted 0..%d, got %d",

    "" QSTR " has already been defined as: \"%s\"",
    "Redefinition of " QSTR " not identical!",

    "Variable " QSTR ", is accessed from within both a function and an interrupt",

    "Opcode " QSTR ", placed in RAM. Code can not be placed in RAM during compile time",
};
char* szNotice[] = {
    "%s",
    "DEBUG: %s",
    "Compile: Successful",
};
/******************************************************************************/
FILE* logFile;
int curline;
S32 lpos;
U8* cpos;
/******************************************************************************/
void InitMessages()
{
    logFile = fopen("e:\\lsi\\scripts\\log.txt", "w");
    curline = -1;
}
/******************************************************************************/
void ShutDownMessages()
{
    if (logFile) {
        fclose(logFile);
        logFile = NULL;
    }
}
/******************************************************************************/

#define PRINT_EMSG(STRINGS)                                                                         \
    if (scr) {                                                                                      \
        while (scr->parent && !scr->path) {                                                         \
            printf("Within \"%s\": (%d),\n\t", scr->filename, scr->line);                           \
            scr = scr->parent;                                                                      \
        }                                                                                           \
        printf("%s (%d: %d): ", scr->filename, scr->line, GetLineCharsEx(scr->buffer, scr->inPtr)); \
    }                                                                                               \
    va_start(argptr, errnum);                                                                       \
    vprintf(STRINGS[errnum], argptr);                                                               \
    va_end(argptr);

void msgQuit(void)
{
    ShutDownCompiler();
    //getch();
    bexit(1);
}
/*----------------------------------------------------------------------------*/
void error(int errnum, ...)
{
    va_list argptr;
    INSCRIPT* scr = curScript;

    printf("\n[Error]: ");
    COMPILE_SUCCESS = FALSE;

    PRINT_EMSG(szErrors);
    printf("!");

    if (errorCnt++ >= cfg.msg.error.max)
        msgQuit();
    ;
}
/*----------------------------------------------------------------------------*/
void errorf(char* filename, int line, int errnum, ...)
{
    va_list argptr;

    printf("\n[Error]: ");
    COMPILE_SUCCESS = FALSE;

    if (filename) {
        printf("%s (%d): ", filename, line);
    }
    va_start(argptr, errnum);
    vprintf(szErrors[errnum], argptr);
    va_end(argptr);

    printf("!");

    if (errorCnt++ >= cfg.msg.error.max)
        msgQuit();
}
/*----------------------------------------------------------------------------*/
void fatal(int errnum, ...)
{
    va_list argptr;
    INSCRIPT* scr = curScript;

    printf("\n\n[Fatal Error]: ");
    COMPILE_SUCCESS = FALSE;

    PRINT_EMSG(szFatal);
    printf("!");

    msgQuit();
}
/*----------------------------------------------------------------------------*/
void warning(int errnum, ...)
{
    va_list argptr;
    INSCRIPT* scr = curScript;

    if (!PRECOMPILING) {
        printf("\n[Warning]: ");

        PRINT_EMSG(szWarnings);
        printf("!");

        if (warnCnt++ >= cfg.msg.warning.max)
            msgQuit();
    }
}
/*----------------------------------------------------------------------------*/
void notice(int msg, char* str, ...)
{
    va_list argptr;
    INSCRIPT* scr = curScript;

    if (!PRECOMPILING && cfg.msg.tell.enabled) {
        if (msg != -1)
            printf("\n[Tell: %s]: ", szPreprocess[PREPROCESS_TELL].members[msg]);
        if (scr) {
            while (scr->parent && !scr->path) {
                printf("Within \"%s\": (%d),\n\t", scr->filename, scr->line);
                scr = scr->parent;
            }
            printf("%s (%d: %d): ", scr->filename, scr->line, GetLineCharsEx(scr->buffer, scr->inPtr));
        }

        printf("\n\t");
        va_start(argptr, str);
        vprintf(str, argptr);
        va_end(argptr);

        //getch();

        if (todoCnt++ >= cfg.msg.tell.max)
            msgQuit();
    }
}
/*----------------------------------------------------------------------------*/
void todo(char* str)
{
    if (!PRECOMPILING && cfg.msg.todo.enabled) {
        printf("\n[Todo]: %s", str);
        if (todoCnt++ >= cfg.msg.todo.max)
            msgQuit();
    }
}
/*----------------------------------------------------------------------------*/
void message(int errnum, ...)
{
    va_list argptr;
    INSCRIPT* scr = curScript;
    char* s;

    printf("\n");
    if (errnum) {
        if (errnum == 1)
            printf("[DEBUG]: ");
        else
            printf("\n[Compile]: ");
    }

    if (errnum == 1) {
        if (scr) {
            while (scr->parent && !scr->path)
                scr = scr->parent;
            printf("%s (%d: %d): ", scr->filename, scr->line, GetLineCharsEx(scr->buffer, scr->inPtr));
        }
        va_start(argptr, errnum);
        s = va_arg(argptr, char*);
        vprintf(s, argptr);
        va_end(argptr);

        //getch();
    } else if (errnum == 0) {
        va_start(argptr, errnum);
        s = va_arg(argptr, char*);
        vprintf(s, argptr);
        va_end(argptr);
    } else {
        PRINT_EMSG(szNotice);
    }
}
/******************************************************************************/
void logprint(char* s, ...)
{ /*
	va_list argptr;

    va_start(argptr, s);
    vfprintf(logFile,s, argptr);
    va_end(argptr);   */
}
/******************************************************************************/
void logenter()
{ /*
    INSCRIPT *scr = curScript;
	int l;

    if(!PRECOMPILING && curline==-1) {
		while(scr->parent) scr = scr->parent;
    	curline = scr->line;
		lpos = GET_BUF_OFFSET(scr->buffer,scr->inPtr);
        cpos = rCode.ptr;
	}      */
}
/******************************************************************************/
void logexit()
{ /*
    INSCRIPT *scr = curScript;
	char *logStr, *end;
	if(curline==-1)
    	return;
                    
	if(cpos != rCode.ptr) {

    	while(scr->parent) scr = scr->parent;

    	logStr = scr->buffer+lpos;
    	end = scr->inPtr;

    	fputc('\n',logFile);
		while(*logStr && logStr < end)
    		fputc(*logStr++,logFile);  
    	curline = -1;
	}

    logenter();  */
}
/******************************************************************************/
void bexit(int code)
{
    message(0, "");
    exit(code);
}
/******************************************************************************/
