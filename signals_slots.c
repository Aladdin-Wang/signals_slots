/****************************************************************************
*  Copyright 2022 KK (https://github.com/Aladdin-Wang)                                    *
*                                                                           *
*  Licensed under the Apache License, Version 2.0 (the "License");          *
*  you may not use this file except in compliance with the License.         *
*  You may obtain a copy of the License at                                  *
*                                                                           *
*     http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                           *
*  Unless required by applicable law or agreed to in writing, software      *
*  distributed under the License is distributed on an "AS IS" BASIS,        *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
*  See the License for the specific language governing permissions and      *
*  limitations under the License.                                           *
*                                                                           *
****************************************************************************/

#include "signals_slots.h"
/* 
 * Function: direct_connect
 * Description: Establishes a direct connection between a signal and slot.
 * Parameters:
 *   - SenderObj: Pointer to the signal-slot object.
 *   - ptSender: Name of the sender.
 *   - RecObj: Pointer to the receiver object.
 *   - RecFun: Pointer to the receiver function.
 * Returns: Returns true if the connection is successful, otherwise returns false.
 */
bool direct_connect(sig_slot_t *SenderObj, const char *ptSender,void *RecObj,void *RecFun)
{
    if(SenderObj == NULL || ptSender == NULL || RecObj == NULL || RecFun == NULL){
        return false;
    }
	/* Create a pointer to the metadata of the signal-slot object */
    sig_slot_t * ptMetaObj = SenderObj;
    do{
		/*Allow signals to be connected to other signals.*/
        if(strstr(RecFun,"sig_")){
            memcpy(ptMetaObj->chSenderName,ptSender,strlen(ptSender)); 
            while(ptMetaObj->ptNext != NULL){
                ptMetaObj = ptMetaObj->ptNext;
            }
            ptMetaObj->ptNext = RecObj; 
            ptMetaObj->ptNext->ptPrev = ptMetaObj;          
            ptMetaObj = ptMetaObj->ptNext;
           
            memcpy(ptMetaObj->chSenderName,RecFun,strlen(RecFun));  
            break;            
        }
        /*Support connecting one signal to multiple slot functions or multiple signals to one slot function.*/
        if(strcmp(ptMetaObj->chSenderName,ptSender) == 0 || strlen(ptMetaObj->chSenderName) > 0){
           sig_slot_t * ptSenderObj = malloc(sizeof(struct sig_slot_t));
           if(ptSenderObj == NULL){
               return false;
           }
           while(ptMetaObj->ptNext != NULL){
               ptMetaObj = ptMetaObj->ptNext;
           }
           ptMetaObj->ptNext = ptSenderObj;
           ptMetaObj->ptNext->ptPrev = ptMetaObj;  
           ptMetaObj = ptMetaObj->ptNext; 	   
        }
		ptMetaObj->ptRecFun = RecFun;
		ptMetaObj->ptRecObj = RecObj;
		memcpy(ptMetaObj->chSenderName,ptSender,strlen(ptSender));         
    }while(0);
    return true;
}

/*
 * Function: auto_disconnect
 * Description: Disconnects a signal and frees associated memory.
 * Parameters:
 *   - ptSenderObj: Pointer to the signal-slot object.
 *   - ptSender: Name of the sender.
 */
void auto_disconnect(sig_slot_t *ptSenderObj, const char *ptSender)
{
    if(ptSenderObj == NULL || ptSender == NULL){
        return;
    }
    sig_slot_t * ptMetaObj = ptSenderObj;
	if(strcmp(ptMetaObj->chSenderName,ptSender) == 0){
	   while(ptMetaObj->ptNext != NULL){
		   ptMetaObj = ptMetaObj->ptNext;
	   }
 
       while(ptMetaObj != NULL){
           ptMetaObj->ptNext = NULL;
           memset(ptMetaObj->chSenderName,0,sizeof(ptMetaObj->chSenderName));
           if(ptMetaObj->ptRecFun != NULL){
               ptMetaObj->ptRecObj = NULL;
               ptMetaObj->ptRecFun = NULL;
               sig_slot_t * ptObj = ptMetaObj;
               free(ptObj);
           }
           ptMetaObj = ptMetaObj->ptPrev;
       }
	}
}
