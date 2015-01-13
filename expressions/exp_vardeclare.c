/***************************************************************************
 *  NESHLA: The Nintendo Entertainment System High Level Assembler
 *  Copyright (C) 2003,2004,2005 Brian Provinciano, http://www.bripro.com
 *
 *  This program is free software. 
 *	You may use this code for anything you wish.
 *	It comes with no warranty.
 ***************************************************************************/

/******************************************************************************/
#pragma hdrstop
#include "..\compiler.h"
/******************************************************************************
 * Handles variable declarations
 ******************************************************************************/
#pragma package(smart_init)
/******************************************************************************/

/******************************************************************************/
int FASTCALL CompileVarBody(U16 flags, S16 *brackCnt, VAR *var, int *_elementsSize, int braceCnt, int cast);
/******************************************************************************/
BOOL FASTCALL comProc_VarDeclare(U16 flags, S16 *brackCnt)
{
	int cast,arraySize,elementsSize,braceCnt;
    S32 offset=-1,elementsEnd;
    S32 num;
    U16 varflags = 0;
    char *label=NULL;
    VAR *castvar=NULL;
    VAR *var=NULL,*pvar;
    BOOL VARSET, SEEKED = FALSE;

    if(!STRCMP(szTemp,"typedef")) {
     	varflags |= VARFLAG_TYPECAST; 
        GetNextWord();
    }

    if(!STRCMP(szTemp,"shared")) {
     	varflags |= VARFLAG_SHARED;
        GetNextWord();
    }

	if((cast=IsVarCast(szTemp,&castvar))==VARCAST_NULL) {
    	if(varflags&VARFLAG_TYPECAST) {
        	error(ERR_NOTTYPECAST,szTemp);
         	return TRUE;
        }
    	return FALSE;
    }

    if(curVar&&(curVar->flags&VARFLAG_TYPECAST))
    	varflags |= VARFLAG_TYPECAST;

    for(;;) {
		if(!IsStringLabel(GetNextWord())) {
			error(ERR_BADLABEL,szTemp);
        	sprintf(szTemp, "invlabel_%04X", invlabel++);
		} else {
         	if(PRECOMPILING && IsLabelUsed(szTemp,curVar?curVar->childVars:vars))
        		sprintf(szTemp, "invlabel_%04X", invlabel++);
        }
    	label = strdup(szTemp);

    	if(PeekNextWord()[0]==':') {
    		GetNextWord();
			if(IsStrNum(GetNextWord())) {
				offset = ConfirmWord(StrToInt(szTemp));  
     			varflags |= VARFLAG_FIXED;
			} else
				error(ERR_INTEXP);
            VARSET = TRUE;
    	} else {
			if(varflags&VARFLAG_TYPECAST)
				VARSET = TRUE;
			else if(!curBank)
    			fatal(FTL_VARNOTROMBANK);
		
    		if(cast == VARCAST_STRUCT && !(varflags & VARFLAG_TYPECAST) && !PRECOMPILING) {
    			offset = GetBankOffset();
				VARSET = TRUE;
            } else
            	VARSET = FALSE;
        }

    	if(PRECOMPILING) {
    		var = AddVariable(label, cast, castvar, varflags, offset);
            if(varflags&VARFLAG_TYPECAST) {
            	if(curVar) {
        			var->offset		= VarSize(curVar);
                }
        	}
		} else {
    		var = FindVariable(curVar?curVar->childVars:(varflags&VARFLAG_TYPECAST)?typedefs:vars, label);
            if(!(var->flags&VARFLAG_TYPECAST) && (!(var->flags&VARFLAG_DEFCASTED) || cast!=VARCAST_STRUCT) && (var->offset==-1))
            	SetVarOffsets(var,GetBankOffset(),FALSE);
    	}
		if(castvar) {
            var->size = castvar->size;
        }

        arraySize = 0;
    	if(PeekNextWord()[0]=='[') {
    		GetNextWord();
            if(PeekNextWord()[0]==']') {
            	arraySize=-1;//autosize
    			GetNextWord();
            } else {          /*
				if(IsStrNum(szTemp)) {
            	    if(((var->flags&VARFLAG_TYPECAST) && PRECOMPILING) || (!(var->flags&VARFLAG_TYPECAST))) {
                		arraySize = ConfirmWord(StrToInt(szTemp));
						var->arraySize = arraySize;
                	}
				} else
					error(ERR_INTEXP);
                    */
				CompileImmediateInteger(0, &num, -1, 0);
                if(((var->flags&VARFLAG_TYPECAST) && PRECOMPILING) || (!(var->flags&VARFLAG_TYPECAST))) {
            		arraySize = ConfirmWord(num);
            		var->arraySize = arraySize;
                }
    			if(GetNextWord()[0]!=']')
					error(ERR_ARRAYENDEXP,szTemp);
            }
    	}

        if(!(var->flags&VARFLAG_DEFCASTED) && (cast==VARCAST_STRUCT)) {
    		SetCurVar(var);
			GetCode(flags|CF_BRACEOK|CF_GETNEXTWORD, brackCnt);
			ReleaseCurVar();
        } else {
        	if(PeekNextWord()[0]=='=') {
    			GetNextWord();
				if(var->flags&VARFLAG_TYPECAST) {
            		error(ERR_VARCASTASSIGN,label);
                	SeekPastBraceBlock();
            	} else if(curBank->type==BANKTYPE_RAM) {
                	error(ERR_VARDECLAREINRAM);
                	SeekPastBraceBlock();
                } else {
                          
					elementsSize	= 0;
	                elementsEnd		=
	                	(GetBankOffset()-curBank->org) +
	                    ((arraySize==-1)?
	                    	var->size:var->size*arraySize);

     				if(GetNextWord()[0]=='{') {
                    	if(!arraySize) {
                         	error(ERR_NOTARRAY,var->label);
                        }
                        braceCnt = 1;
    					GetNextWord();
                    } else braceCnt = 0;
                    do {
                    	braceCnt =
                        	CompileVarBody(flags,brackCnt, var, &elementsSize, braceCnt, cast);
                    } while(braceCnt);

                    if(!PRECOMPILING) {
                    	if(arraySize==-1) {
                    		arraySize=elementsSize/VarSize(var);
                        	SEEKED = TRUE;
                    	} else {
                    		if(elementsSize>VarSize(var))
                    			error(ERR_ELEMENTSTOOLARGE,var->label,elementsSize-VarSize(var));
                			BankSeek(elementsEnd);
                        	SEEKED = TRUE;
                   		}
              			var->arraySize = arraySize;
                    }

                }
        	} else {
             	if(((var->flags&VARFLAG_TYPECAST) && PRECOMPILING) || (!(var->flags&VARFLAG_TYPECAST) && !PRECOMPILING && !VARSET)) {
              		if(arraySize==-1)
                    	arraySize = 1;
                    if(arraySize!=0)
                    	var->arraySize = arraySize;
                	if(!PRECOMPILING) {
						BankSeekFwd(VarSize(var));
                        SEEKED = TRUE;
                    }
                }
            }
            if(((var->flags&VARFLAG_TYPECAST) && PRECOMPILING) || (!(var->flags&VARFLAG_TYPECAST) && !PRECOMPILING)) {
            	// adjust all the var sizes for structs
            	arraySize = VarSize(var);
                pvar = var->parent;
                while(pvar) {
                 	pvar->size += arraySize;
                    pvar = pvar->parent;
                }
            }
        }
		if(!(var->flags&VARFLAG_TYPECAST) && (cast==VARCAST_STRUCT))
			SetVarOffsets(var,offset,FALSE);
        // removed 2004-05-22, if problems occurs, check
        if(!SEEKED && (var->flags&(VARFLAG_TYPECAST|VARFLAG_DEFCASTED))==VARFLAG_DEFCASTED && (cast==VARCAST_STRUCT)) {
        	CheckCurBank();
            if((var->flags&VARFLAG_TYPECAST) || !PRECOMPILING)
            	BankSeekFwd(VarSize(var));
        }           
    	ssFree(label);
        
        if(PeekNextWord()[0]!=',')
        	break;
        GetNextWord();
    }

    return TRUE;
}
/******************************************************************************/
int FASTCALL CompileVarBody(U16 flags, S16 *brackCnt, VAR *var, int *_elementsSize, int braceCnt, int cast)
{
	int elementsSize, len;
    S32 num;
    VAR *childvar;
    S16 braceCntStart;
    int elementsStart;

    elementsSize = *_elementsSize;
                 
	if(!PRECOMPILING) {
    	elementsStart = elementsSize;
    }

	if(comProc_LabelDeclaration(flags, brackCnt)) {
	 	// catch the labels       
	    if(braceCnt) {
	     	if(GetNextWord()[0]=='}')
	        	braceCnt--;
	    }
	} else {
		if(var->cast == VARCAST_STRUCT) {
	        braceCntStart = braceCnt;
			if(szTemp[0]!='{') {
	        	error(ERR_STRUCTELEMENTBRACE,szTemp);
			} else {    
				GetNextWord();
	    		braceCnt++;
	        }
	        childvar = FindFirstVariable(var->childVars);
	        while(childvar) {
	        	braceCnt =
					CompileVarBody(flags|CF_VARCHILD,brackCnt, childvar, &elementsSize, braceCnt, childvar->cast);

	         	childvar = childvar->next;
	            if(childvar && !braceCnt) return 0;
	        }
			if(braceCntStart != braceCnt) {
	        	error(ERR_CLOSEBRACEEXP,var->label);
			}
            if(!PRECOMPILING)
            	var=var;
		} else if(szTemp[0]=='{' && var->arraySize) {
        	int arsize = var->arraySize;

	        braceCntStart = braceCnt;
            GetNextWord();
            braceCnt++;

	        childvar = CloneVar(var, curVar, 0);
            childvar->arraySize = 0;
	        do {
	        	braceCnt =
					CompileVarBody(flags|CF_VARCHILD,brackCnt, childvar, &elementsSize, braceCnt, childvar->cast);
            	arsize--;
	        } while(braceCnt && braceCnt > braceCntStart);

            FreeVars(&childvar);

			if(braceCntStart != braceCnt) {
	        	error(ERR_CLOSEBRACEEXP,var->label);
			}
			if(!PRECOMPILING && arsize < 0) {
             	error(ERR_ARRAYTOOLARGE,var->label);
            }
		} else if(szTemp[0]=='"') {
	    	if(varcasts[cast].size!=VARSIZE_BYTE)
	        	error(ERR_STRINGNOTBYTE,var->label);
	 		if(!DoString())
	    		return 0;
			if(!PRECOMPILING) {
            	len = lenSzStr;//(U16)strlen(szString);
                if(var->arraySize && len > var->arraySize) {
                 	len = var->arraySize;
                    szString[len?len-1:0] = '\0';
                    error(ERR_STRINGTOOLONG,szString);
                }
	        	BankWrite(szString,len);
                if(var->arraySize) {
                	BankFill(0,var->arraySize-len);
	        		elementsSize += var->arraySize;
                } else {
	        		elementsSize += len;
                }
	        }
		} else {
			CompileImmediateInteger(0, &num, -4, 0);
			if(!PRECOMPILING) {
            	if(strToInt_LabelObject) {
                 	// do the fixup
                    AddFixOffs(
                    	strToInt_LabelType,
                        (var->flags&VARFLAG_16BIT)?FIXOFFS_WORD:FIXOFFS_BYTE,
                        curBank->ptr,
                        0,
                        strToInt_LabelObject
                    );
                }
				if(var->flags&VARFLAG_16BIT) {
	    			WriteCodeW(num);
	        	    elementsSize += 2;
	    		} else {
	    			WriteCodeB(num);
	        	    elementsSize ++;
	        	}
	        }
		}
	    if(braceCnt) {
	     	if(GetNextWord()[0]=='}') {
	        	braceCnt--;
	        } else if(szTemp[0]!=',') {
	        	error(ERR_ARRAYCOMMAEXP,var->label);
	        } else {
	     		if(GetNextWord()[0]=='}') {
	        		braceCnt--;
	            }
	        }
	    }
	}

    if(!PRECOMPILING && (flags&CF_VARCHILD)) {
    	int amount = (elementsSize-elementsStart);
    	if(var->arraySize && amount < var->arraySize) {
    		BankFill(0,var->arraySize-amount);
    		elementsSize += var->arraySize-amount;
    	}
    }

	*_elementsSize = elementsSize;
	
	return braceCnt;
}
/******************************************************************************/

