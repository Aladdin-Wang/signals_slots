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

#ifndef __SIGNALS_SLOTS_H_
#define __SIGNALS_SLOTS_H_
#include ".\app_cfg.h"
#if USE_SERVICE_SIGNALS_SLOTS == ENABLED
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* example:
  //连接信号与槽
  #define connect(__SIG_OBJ,__SIG_NAME,__SLOT_OBJ,__SLOT_FUN)    \
            direct_connect(__SIG_OBJ.tObject,__SIG_NAME,__SLOT_OBJ,__SLOT_FUN)
  example：
  connect(&tCanMsgObj,SIGNAL(send_sig),&s_tFIFOin,SLOT(enqueue_bytes));
  

  //定义信号
  signals(__NAME,__OBJ,...) 
  example：
  signals(send_sig,can_data_msg_t *ptThis,
      args(              
            uint8_t *pchByte,
            uint16_t hwLen
          ));
  //发送信号
   emit(__NAME,__OBJ,...) 
   example：  
   emit(send_sig,&tCanMsgObj,
      args(
            tCanMsgObj.CanDATA.B,
            tCanMsgObj.CanDLC
         ));

  //发送信号的对象需要继承信号与槽
  SIG_SLOT_OBJ

  //定义槽
   slots(__NAME,__OBJ,...) (非必须，普通函数即可)
   example：
   slots(enqueue_bytes,byte_queue_t *ptObj,
      args(
            void *pchByte,
            uint16_t hwLength
         ));
*/

#define SIG_NAME_MAX 20

#define SIGNAL(x) "sig_"#x

#define SLOT(x) x

#define SIG_SLOT_OBJ  sig_slot_t tObject;

#ifndef __PLOOC_VA_NUM_ARGS_IMPL
#   define __PLOOC_VA_NUM_ARGS_IMPL( _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,     \
                                    _12,_13,_14,_15,_16,__N,...)      __N
#endif

#ifndef __PLOOC_VA_NUM_ARGS
#define __PLOOC_VA_NUM_ARGS(...)                                                \
            __PLOOC_VA_NUM_ARGS_IMPL( 0,##__VA_ARGS__,16,15,14,13,12,11,10,9,   \
                                      8,7,6,5,4,3,2,1,0)
#endif
                                  
#undef __CONNECT2
#undef CONNECT2
#undef __CONNECT3
#undef CONNECT3

#define __CONNECT3(__A, __B, __C)         __A##__B##__C
#define __CONNECT2(__A, __B)              __A##__B

#define CONNECT3(__A, __B, __C)           __CONNECT3(__A, __B, __C)
#define CONNECT2(__A, __B)                __CONNECT2(__A, __B)

#ifndef SAFE_NAME
#define SAFE_NAME(__NAME)   CONNECT3(__,__NAME,__LINE__)
#endif


#define  args(...)            ,__VA_ARGS__

#define _args(...)            __VA_ARGS__

#define __RecFun_0(__OBJ)                         \
            __RecFun((__OBJ))

#define __RecFun_1(__OBJ, __ARG1)                     \
            __RecFun((__OBJ),(__ARG1))

#define __RecFun_2(__OBJ, __ARG1, __ARG2)                  \
            __RecFun((__OBJ),(__ARG1), (__ARG2))

#define __RecFun_3(__OBJ, __ARG1, __ARG2, __ARG3)                \
            __RecFun((__OBJ),(__ARG1), (__ARG2), (__ARG3))                         

#define __RecFun_4(__OBJ, __ARG1, __ARG2, __ARG3, __ARG4)               \
            __RecFun((__OBJ),(__ARG1), (__ARG2), (__ARG3), (__ARG4))    
            
#define __RecFun_5(__OBJ, __ARG1, __ARG2, __ARG3, __ARG4, __ARG5)                \
            __RecFun((__OBJ),(__ARG1), (__ARG2), (__ARG3), (__ARG4), (__ARG5))  

#define __RecFun_6(__OBJ, __ARG1, __ARG2, __ARG3, __ARG4, __ARG5, __ARG6)                \
            __RecFun((__OBJ),(__ARG1), (__ARG2), (__ARG3), (__ARG4), (__ARG5), (__ARG6)) 

#define __RecFun_7(__OBJ, __ARG1, __ARG2, __ARG3, __ARG4, __ARG5, __ARG6, __ARG7)                \
            __RecFun((__OBJ),(__ARG1), (__ARG2), (__ARG3), (__ARG4), (__ARG5), (__ARG6), (__ARG7)) 

#define __RecFun_8(__OBJ, __ARG1, __ARG2, __ARG3, __ARG4, __ARG5, __ARG6, __ARG7, __ARG8)                \
            __RecFun((__OBJ),(__ARG1), (__ARG2), (__ARG3), (__ARG4), (__ARG5), (__ARG6), (__ARG7), (__ARG8)) 
 
            
#define __signals(__NAME,...)                                    \
             typedef void CONNECT2(__NAME,_fun_t)( __VA_ARGS__);  

#define signals(__NAME,__OBJ,...)               \
          __signals(__NAME,_args(__OBJ __VA_ARGS__))

#define __emit(__OBJ,...) \
   		   CONNECT2(__RecFun_,__PLOOC_VA_NUM_ARGS(__VA_ARGS__))              \
                   ((__OBJ)->ptRecObj,##__VA_ARGS__); 

#define emit(__NAME,__OBJ,...)                     \
           do {sig_slot_t *ptObj = &((__OBJ)->tObject);                \
               do{if(__OBJ == NULL || ptObj == NULL ) break;              \
		           CONNECT2(__NAME,_fun_t) *__RecFun = ptObj->ptRecFun;   \
                   if(__RecFun != NULL && strcmp(ptObj->chSenderName,SIGNAL(__NAME)) == 0) \
					   __emit(ptObj __VA_ARGS__);         \
                   ptObj = ptObj->ptNext;             \
               }while(ptObj != NULL);              \
           }while(0)

#define __slots(__NAME,...)             \
            void __NAME(__VA_ARGS__);
            
#define slots(__NAME,__OBJ,...)  \
            __slots(__NAME,_args(__OBJ,##__VA_ARGS__))

#define connect(__SIG_OBJ,__SIG_NAME,__SLOT_OBJ,__SLOT_FUN)    \
            direct_connect(__SIG_OBJ.tObject,__SIG_NAME,__SLOT_OBJ,__SLOT_FUN)

#define disconnect(__SIG_OBJ,__SIG_NAME)    \
            auto_disconnect(__SIG_OBJ.tObject,__SIG_NAME)


typedef struct sig_slot_t sig_slot_t;
typedef struct sig_slot_t{
	char   chSenderName[SIG_NAME_MAX];
    void * ptSenderObj;  
	void * ptRecObj;
	void * ptRecFun;
    sig_slot_t *ptNext;
    sig_slot_t *ptPrev;
}sig_slot_t;

bool direct_connect(sig_slot_t *ptSenderObj, const char *ptSender,void *ptRecObj,void *ptRecFun);
void auto_disconnect(sig_slot_t *ptSenderObj, const char *ptSender);


#endif
#endif /* QUEUE_QUEUE_H_ */
