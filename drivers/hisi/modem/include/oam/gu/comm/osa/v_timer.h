/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */


#ifndef _VOS_TIMER_H
#define _VOS_TIMER_H

#include "v_typdef.h"
#include "vos_config.h"
#include "v_msg.h"
#include "v_int.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define VOS_32K_TIMER_ENABLE  VOS_YES

/* errno definiens */
#define VOS_ERRNO_RELTM_CTRLBLK_INITFAIL                    0x20060000
#define VOS_ERRNO_RELTM_TASK_INITFAIL                       0x20060001
#define VOS_ERRNO_RELTM_START_MSGNOTINSTALL                 0x20060004
#define VOS_ERRNO_RELTM_START_INPUTMODEINVALID              0x20060005
#define VOS_ERRNO_SYSTIME_VALIDTIME_INPUTISNULL             0x20060006

#define VOS_ERRNO_RELTM_STOP_INPUTISNULL                    0x2006000A
#define VOS_ERRNO_RELTM_STOP_TIMERINVALID                   0x2006000B
#define VOS_ERRNO_RELTM_STOP_TIMERNOTSTART                  0x2006000C
#define VOS_ERRNO_RELTM_STOP_STATUSERROR                    0x2006000D
#define VOS_ERRNO_RELTM_FREE_RECEPTION                      0x2006000E


#define VOS_ERRNO_RELTM_RESTART_INPUTISNULL                 0x20060010
#define VOS_ERRNO_RELTM_RESTART_TIMERINVALID                0x20060011

#define VOS_ERRNO_RELTM_QUERY_PTICKNULL                     0x20060082
#define VOS_ERRNO_RELTM_QUERY_TIMERNULL                     0x20060083
#define VOS_ERRNO_RELTM_QUERY_TIMERSTOP                     0x20060084
#define VOS_ERRNO_RELTM_QUERY_TIMERDRVNULL                  0x20060085
#define VOS_ERRNO_RELTM_QUERY_TIMERNOTEXIST_INZERO          0x20060086
#define VOS_ERRNO_RELTM_QUERY_TIMERNOTEXIST_INSORT          0x20060087

#define VOS_ERRNO_SYSTIME_CALSEC_INPUTPARA1ISNULL           0x20600056
#define VOS_ERRNO_SYSTIME_CALSEC_INPUTPARA2ISNULL           0x20600057
#define VOS_ERRNO_SYSTIME_CMPDATE_INPUTDATE1INVALID         0x20600058
#define VOS_ERRNO_SYSTIME_CMPDATE_INPUTDATE2INVALID         0x20600060
#define VOS_ERRNO_SYSTIME_CALSEC_INPUTPARA3ISNULL           0x20600061
#define VOS_ERRNO_SYSTIME_CALSEC_DATAOVERFLOW               0x20600062
#define VOS_ERRNO_SYSTIME_CALSEC_CALDAYERROR                0x20600063
#define VOS_ERRNO_SYSTIME_CALSEC_INPUT1LESSTHAN2            0x20600064

#define VOS_ERRNO_SYSTIME_GETSYST_INPUTPARAISNULL           0x20160085

#define VOS_ERRNO_SYSTIME_TK2MS_MSECPERTICKISZERO           0x20600035
#define VOS_ERRNO_SYSTIME_TK2MS_INPUTTICKISTOOBIG           0x20600036

#define VOS_ERRNO_SYSTIME_S2TIME_INPUTPARA2ISNULL           0x20600093
#define VOS_ERRNO_SYSTIME_S2TIME_INPUTPARA3ISNULL           0x20600094

#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA1ISNULL          0x20600070
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA2ISNULL          0x20600071
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA4ISNULL          0x20600072
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTPARA5ISNULL          0x20600073
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTDATEINVALID          0x20600074
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTTIMEINVALID          0x20600075
#define VOS_ERRNO_SYSTIME_SEC1970_INPUTMILLSECSINVALID      0x20600076

#define VOS_ERRNO_DRXTIME_START_INPUTISNULL                 0x2060f000
#define VOS_ERRNO_DRXTIME_STOP_INPUTISNULL                  0x2060f001
#define VOS_ERRNO_DRXTIME_START_STOP_FAIL                   0x2060f002
#define VOS_ERRNO_DRXTIME_START_MSGNOTINSTALL               0x2060f003
#define VOS_ERRNO_DRXTIME_ERROR_TIMERHANDLE                 0x2060f004
#define VOS_ERRNO_DRXTIME_RESOURCE_INITFAIL                 0x2060f005
#define VOS_ERRNO_DRXTIME_TASK_INITFAIL                     0x2060f006

#define VOS_ERRNO_BIT64TIME_START_STOP_FAIL                 0x2060f011
#define VOS_ERRNO_BIT64TIME_START_MSGNOTINSTALL             0x2060f012
#define VOS_ERRNO_BIT64TIME_ERROR_TIMERNOUSED               0x2060f013
#define VOS_ERRNO_BIT64TIME_RESOURCE_INITFAIL               0x2060f014
#define VOS_ERRNO_BIT64TIME_TASK_INITFAIL                   0x2060f015

#define VOS_ERRNO_SYSTIMER_FULL                             0x2060FFFF

#define ELAPESD_TIME_INVAILD                                0xFFFFFFFF

typedef struct
{
    VOS_UINT32      ulStartCount;                   /* 调底软接口起定时器次数 */
    VOS_UINT32      ulStartSlice;                   /* 调底软接口起定时器时间 */
    VOS_UINT32      ulStopCount;                    /* 调底软接口停定时器次数 */
    VOS_UINT32      ulExpireCount;                  /* 收到定时器中断次数 */
    VOS_UINT32      ulStartErrCount;                /* 调底软起定时器接口返回错误次数 */
    VOS_UINT32      ulStopErrCount;                 /* 调底软停定时器接口返回错误次数 */
    VOS_UINT32      ulElapsedErrCount;              /* 调底软获取剩余时间接口次数 */
    VOS_UINT32      ulElapsedContentErrCount;       /* 调底软获取剩余时间接口返回错误次数 */
    VOS_UINT32      ulElapsedContentErrSlice;       /* 调底软获取剩余时间接口返回错误时间 */
    VOS_UINT32      ulExpiredShortErrCount;         /* SOC Timer超时时间小于配置时间次数 */
    VOS_UINT32      ulExpiredShortErrSlice;         /* SOC Timer超时时间小于配置时间时间 */
    VOS_UINT32      ulExpiredSendErrCount;          /* SOC Timer发送消息失败次数 */
    VOS_UINT32      ulExpiredSendErrSlice;          /* SOC Timer发送消息失败时间 */
    VOS_UINT32      ulExpiredLongErrCount;          /* SOC Timer超时时间大于配置时间次数 */
    VOS_UINT32      ulExpiredLongErrSlice;          /* SOC Timer超时时间大于配置时间时间 */
    VOS_UINT32      ulBit64TimerStartCount;
    VOS_UINT32      ulBit64TimerStopCount;
    VOS_UINT32      ulBit64TimerStartErrCount;
    VOS_UINT32      ulBit64TimerStopErrCount;
}VOS_TIMER_SOC_TIMER_INFO_STRU;

typedef struct
{
    VOS_UINT32      ulAction;/* the value of start;exp;0XFFFFFFFF->stop 0xFFFFFFFE->expire */
    VOS_UINT32      ulSlice;
    VOS_UINT32      ulCoreId;
}RTC_SOC_TIMER_DEBUG_INFO_STRU;

/* the Max length of timer unit is ms -> 18 hours*/
#define VOS_TIMER_MAX_LENGTH                                64800000

/*当前版本单时钟方案暂不需要考虑32.000K时钟状态，默认使用原有32.768K时钟流程*/
/*#define V8R1_SINGLECLOCK*/

/* 32.768K */
#define RTC_CYCLE_LENGTH                                    32.768

#ifdef V8R1_SINGLECLOCK
/* 32.000K */
#define RTC_SINGLECLOCK_CYCLE_LENGTH                        32.000
extern VOS_UINT16                                           g_usSingleXO;
#endif

struct TimerDrvStruct
{
    struct TimerDrvStruct *pNext;   /* double link                    */
    struct TimerDrvStruct *pPrev;   /* double link                    */
    VOS_UINT32    ulTimeLen;        /* expire time in ms              */
    VOS_UINT32    ulTicks;          /* time passed,used in hash link  */
    VOS_UINT8     ucMode;           /* timer mode                     */
    VOS_UINT8     ucStatus;         /* status of timer                */
    VOS_UINT8     ucType;           /* indicate DOPRA or vrpS timer   */
    VOS_UINT8     ucLink;           /* which link the timer belong    */
    VOS_UINT8     aucRsv[4];
};

#define VOS_RELTIMER_NOLOOP            0x0
#define VOS_RELTIMER_LOOP              1
#define VOS_RELTIMER_CALLBACK          2
#define VOS_DELTM_ON_TIMEOUTMSG        4

typedef VOS_VOID (*REL_TIMER_FUNC)( VOS_UINT32 ulParam, VOS_UINT32 ulTimerName);

typedef int * HTIMER;

typedef struct RelTimerGrpCBType * HTIMERGRP;

struct RelTimerType
{
    struct TimerDrvStruct drv;
    VOS_PID        Pid;               /* which module group belongs to */
    VOS_UINT32     ulTmName;          /* timer name */
    VOS_UINT32     ulParam;           /* parameter for timeout handling */
    VOS_UINT8      aucRsv[4];
    HTIMER         *phTimer;
    union
    {
        HTIMERGRP      hTimerGrp;     /* which timer group it belongs to */
        REL_TIMER_FUNC TimeOutFunc;   /* timeout callback function */
    }grp_func;
    struct REL_TIMER_MSG_STRU *pSend; /* msgs sent by this timer which in the msg queue */
};

typedef struct RelTimerType TTIMER;


/* timer group */
typedef struct RelTimerGrpCBType
{
    VOS_UINT32      MaxTimers;   /* timer maximum number in group  */
    VOS_UINT32      InitFlag;    /* Initialize flag */
    VOS_PID         Pid;         /* which module group belongs to */
    VOS_UINT8       aucRsv[4];
    REL_TIMER_FUNC  TimeOutFunc; /* timeout callback function */
    HTIMER          pTimer[1];   /* timer queue, it must be last field of this structure  */
} ReltimerGrpCB;

/******************************************************************************/

typedef struct REL_TIMER_MSG_STRU
{
    VOS_MSG_HEADER            /* Structure of message packet header, where ulSenderPid equals to VOS_PID_TIMER. */
    VOS_UINT8  ucType;        /* Type of timing message packet, being VOS_RELATIVE_TIMER. */
    VOS_UINT8  ucValid;       /* Whether the msg is valid, if invalid,*/
                              /* The value is VOS_TM_INVALID_MSG */
    VOS_UINT8  Reserved[2];   /* Reserved for align */
    VOS_UINT32 ulName;        /* Timer name, input when the timer is started. */
    VOS_UINT32 ulPara;        /* Timer parameter, input when the timer is started. */
                              /* When a timer belongs to a timer group, this parameter indicates its serial number in this timer group.  */
    HTIMER     hTm;           /* Which timer sends this message */

    /* Double link, linking messages sent by the same timer */
    struct REL_TIMER_MSG_STRU *pNext;
    struct REL_TIMER_MSG_STRU *pPrev;

}REL_TIMER_MSG;

enum
{
    VOS_TIMER_OM_START,
    VOS_TIMER_OM_RESTART,
    VOS_TIMER_OM_STOP,
    VOS_TIMER_OM_BUTT
};

/******************************************************************************/

#define VOS_MSG_TM_SECOND             0xFF
#define VOS_MSG_TM_MINUTE             0xFE
#define VOS_MSG_TM_CHECK_START        0xFD
#define VOS_MSG_TM_CHECK_STOP         0xFC
#define VOS_TM_INVALID_MSG            0xBD
#define VOS_TM_CHECK_START            2          /* Start checking system at 2:00... */
#define VOS_TM_CHECK_STOP             3          /* ...and stop it at 3:00           */

#define VOS_TM_TYPE_DOPRA             0
#define VOS_TM_TYPE_VRPS              1
#define VOS_TM_TYPE_MAX               2

#define VOS_TM_LINK_ZERO              0
#define VOS_TM_LINK_SORTED            1

#define APP_PID                       0xffffffff
extern CPU_TICK_EX                    vos_CPU_TICK;
extern VOS_UINT32                     vos_TimerTaskId;
extern VOS_UINT32                     RTC_TimerTaskId;
extern VOS_SPINLOCK                   g_stVosTimerSpinLock;

enum VOS_TIMER_PRECISION_ENUM
{
    VOS_TIMER_PRECISION_0   = 0,
    VOS_TIMER_PRECISION_5   = 5,
    VOS_TIMER_PRECISION_10  = 10,
    VOS_TIMER_PRECISION_20  = 20,
    VOS_TIMER_PRECISION_30  = 30,
    VOS_TIMER_PRECISION_40  = 40,
    VOS_TIMER_PRECISION_50  = 50,
    VOS_TIMER_PRECISION_60  = 60,
    VOS_TIMER_PRECISION_70  = 70,
    VOS_TIMER_PRECISION_80  = 80,
    VOS_TIMER_PRECISION_90  = 90,
    VOS_TIMER_PRECISION_100 = 100,
    VOS_TIMER_NO_PRECISION,
    VOS_TIMER_PRECISION_BUTT
};

typedef VOS_UINT32 VOS_TIMER_PRECISION_ENUM_UINT32;

typedef struct
{
    VOS_UINT8                       ucAction;
    VOS_UINT8                       ucMode;
    VOS_UINT8                       ucResult;
    VOS_UINT8                       ucType;
    VOS_PID                         Pid;
    VOS_UINT32                      ulLength;
    VOS_UINT32                      ulName;
    VOS_UINT32                      ulParam;
    VOS_TIMER_PRECISION_ENUM_UINT32 enPrecision;
    HTIMER                          ulHTimerAddress;
    VOS_UINT32                      ulRTCAction;    /* 记录RTC时钟的Action */
    VOS_UINT32                      ulRTCSlice;     /* 记录RTC时钟的Slice */
    VOS_UINT32                      ulCoreId;       /* 记录RTC时钟运行的CoreId */
}VOS_TIMER_OM_EVENT_STRU;

/* the Max timer number supported by VOS */
/* VOS_TIMER_MESSAGE_NUM must be equal VOS_MAX_TIMER_NUMBER */
#define VOS_MAX_TIMER_NUMBER                          250

#define VOS_UPPER_TIMER_NUMBER                        (VOS_MAX_TIMER_NUMBER/10)

/* the Max RTC timer number supported by VOS */
#define RTC_MAX_TIMER_NUMBER                          200

#define RTC_UPPER_TIMER_NUMBER                        (RTC_MAX_TIMER_NUMBER/10)

#define DRX_TIMER_NOT_USED_FLAG                       (1)
#define DRX_TIMER_USED_FLAG                           (2)

#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
#define DRX_TIMER_MAX_NUMBER                          (13)
#else
#define DRX_TIMER_MAX_NUMBER                          (7)
#endif
#define DRX_TIMER_TIMEOUT_INTERVAL                    (10*32768)

#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
#define BIT64_TIMER_MAX_NUMBER                        (64)
#else
#define BIT64_TIMER_MAX_NUMBER                        (32)
#endif

#define BIT64_TIMER_NOT_USED_FLAG                     (1)
#define BIT64_TIMER_USED_FLAG                         (2)

VOS_UINT32 VOS_TimerCtrlBlkInit(VOS_VOID);

VOS_UINT32 VOS_TimerTaskCreat(VOS_VOID);

VOS_UINT32 VOS_CheckTimer( HTIMER  *phTm, VOS_UINT32 *ulTimerID,
                           VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

VOS_UINT32 V_Start26MRelTimer( HTIMER *phTm, VOS_PID Pid, VOS_UINT32 ulLength,
    VOS_UINT32 ulName, VOS_UINT32 ulParam, VOS_UINT8 ucMode,
    VOS_UINT32 ulFileID, VOS_INT32 usLineNo);

VOS_UINT32 V_Start26MCallBackRelTimer( HTIMER *phTm, VOS_PID Pid,
    VOS_UINT32 ulLength, VOS_UINT32 ulName, VOS_UINT32 ulParam,
    VOS_UINT8 ucMode, REL_TIMER_FUNC TimeOutRoutine,
    VOS_UINT32 ulFileID, VOS_INT32 usLineNo);


VOS_UINT32 V_StartRelTimer( HTIMER *phTm, VOS_PID Pid, VOS_UINT32 ulLength,
    VOS_UINT32 ulName, VOS_UINT32 ulParam, VOS_UINT8 ucMode, VOS_TIMER_PRECISION_ENUM_UINT32 enPrecision,
    VOS_UINT32 ulFileID, VOS_INT32 usLineNo);

VOS_UINT32 V_Stop26MRelTimer( HTIMER *phTm, VOS_UINT32 ulFileID, VOS_INT32 usLineNo, VOS_TIMER_OM_EVENT_STRU *pstEvent );

#if ((!defined DMT) || (defined DMT_TIMER_ON))
#define VOS_StartRelTimer( phTm, Pid, ulLength, ulName, ulParam, ucMode, enPrecision )\
    V_StartRelTimer( (phTm), (Pid), (ulLength), (ulName), (ulParam), (ucMode), (enPrecision), VOS_FILE_ID, __LINE__ )
#endif

REL_TIMER_MSG *VOS_TimerPreAllocMsg( VOS_PID Pid );

VOS_UINT32 V_StopRelTimer( HTIMER *phTm, VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

#if ((!defined DMT) || (defined DMT_TIMER_ON))
#define VOS_StopRelTimer( phTm )\
    V_StopRelTimer( (phTm), VOS_FILE_ID, __LINE__ )
#endif


#if (VOS_WIN32 == VOS_OS_VER)

#define VOS_StartDrxTimer( phTm, Pid, ulLength, ulName, ulParam)\
    V_StartRelTimer( (phTm), (Pid), (ulLength), (ulName), (ulParam), VOS_RELTIMER_NOLOOP, VOS_TIMER_NO_PRECISION, VOS_FILE_ID, __LINE__ )

#define VOS_StopDrxTimer( phTm )\
    V_StopRelTimer( (phTm), VOS_FILE_ID, __LINE__ )

#else

/* Attention: DRX Timer is available just in CCPU. */
VOS_UINT32 V_StartDrxTimer( HTIMER *phTm, VOS_PID Pid, VOS_UINT32 ulLength,
    VOS_UINT32 ulName, VOS_UINT32 ulParam, VOS_UINT32 ulFileID, VOS_INT32 usLineNo);

#define VOS_StartDrxTimer( phTm, Pid, ulLength, ulName, ulParam)\
    V_StartDrxTimer( (phTm), (Pid), (ulLength), (ulName), (ulParam), VOS_FILE_ID, __LINE__ )

VOS_UINT32 V_StopDrxTimer( HTIMER *phTm, VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

#define VOS_StopDrxTimer( phTm )\
    V_StopDrxTimer( (phTm), VOS_FILE_ID, __LINE__ )

#endif

VOS_UINT32 V_RestartRelTimer( HTIMER *phTm, VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

#define VOS_RestartRelTimer( phTm )\
    V_RestartRelTimer( (phTm), VOS_FILE_ID, __LINE__ )

VOS_UINT32 V_GetRelTmRemainTime( HTIMER * phTm, VOS_UINT32 * pulTick,
                                 VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

#define VOS_GetRelTmRemainTime( phTm, pulTick )\
    V_GetRelTmRemainTime( (phTm), (pulTick), VOS_FILE_ID, __LINE__ )

VOS_UINT32 VOS_GetTick( VOS_VOID );

VOS_UINT32 VOS_CalRelativeSec( SYS_T  *pFirstTm, SYS_T  *pSecondTm,
                               VOS_UINT32 *RelativeSec );

/*****************************************************************************
 函 数 名  : V_StartCallBackRelTimer
 功能描述  : 申请一个带有回调的RTC定时器
 输入参数  : VOS_PID Pid    申请组件PID
             VOS_UINT32 ulLength    定时器时长,最大VOS_TIMER_MAX_LENGTH(18小时)，超过最大长度OSA发起主动复位
             VOS_UINT32 ulName      定时器名称
             VOS_UINT32 ulParam     定时器参数
             VOS_UINT8 ucMode       定时器循环模式
                       VOS_RELTIMER_LOOP  -- start periodically
                       VOS_RELTIMER_NOLOO -- start once time
             REL_TIMER_FUNC TimeOutRoutine 回调接口
             VOS_TIMER_PRECISION_ENUM_UINT32 enPrecision    定时器精度要求，单位0 - 100->0%- 100%
             VOS_UINT32 ulFileID    调用文件号
             VOS_INT32 lLineNo      调用行号
 输出参数  : HTIMER *phTm timer句柄
 返 回 值  : VOS_OK 定时器创建成功或者ERRNO表示创建失败
 调用函数  :
 被调函数  :

*****************************************************************************/
VOS_UINT32 V_StartCallBackRelTimer( HTIMER *phTm, VOS_PID Pid,
    VOS_UINT32 ulLength, VOS_UINT32 ulName, VOS_UINT32 ulParam,
    VOS_UINT8 ucMode, REL_TIMER_FUNC TimeOutRoutine, VOS_TIMER_PRECISION_ENUM_UINT32 enPrecision,
    VOS_UINT32 ulFileID, VOS_INT32 usLineNo);


#define VOS_StartCallBackRelTimer(phTm, Pid, ulLength, ulName, ulParam, ucMode, TimeOutRoutine, enPrecision)\
    V_StartCallBackRelTimer((phTm), (Pid), (ulLength), (ulName), (ulParam), (ucMode), (TimeOutRoutine), (enPrecision), VOS_FILE_ID, __LINE__ )

VOS_UINT32 VOS_TmTickToMillSec( VOS_UINT32 ulTicks );

VOS_VOID VOS_Tm_Sec2DateTime( VOS_UINT32 ulSec,
                              VOS_UINT32 *pulRetDate,
                              VOS_UINT32 *pulRetTime );

VOS_BOOL VOS_IsTimeValid( TIME_T *pTime );

VOS_UINT32 VOS_SysSecsSince1970( DATE_T *pDate,
                                 TIME_T *pTime,
                                 VOS_UINT32 ulMillSecs,
                                 VOS_UINT32 *pulRetSecHigh,
                                 VOS_UINT32 *pulRetSecLow );

VOS_BOOL VOS_Is26MTimer( HTIMER *phTm );

VOS_UINT32 VOS_Is26MTimerRunning( VOS_VOID );

/* RTC timer begin */

/* errno definiens */
#define VOS_RTC_ERRNO_RELTM_START_MSGNOTINSTALL                 0x2fff0004
#define VOS_RTC_ERRNO_RELTM_START_INPUTMODEINVALID              0x2fff0005
#define VOS_RTC_ERRNO_SYSTIME_VALIDTIME_INPUTISNULL             0x2fff0006

#define VOS_RTC_ERRNO_RELTM_STOP_INPUTISNULL                    0x2fff000A
#define VOS_RTC_ERRNO_RELTM_STOP_TIMERINVALID                   0x2fff000B
#define VOS_RTC_ERRNO_RELTM_STOP_TIMERNOTSTART                  0x2fff000C
#define VOS_RTC_ERRNO_RELTM_STOP_STATUSERROR                    0x2fff000D
#define VOS_RTC_ERRNO_RELTM_FREE_RECEPTION                      0x2fff000E


#define VOS_RTC_ERRNO_SYSTIMER_FULL                             0x2060FFFF


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                          ulMsgId;
    VOS_UINT8                           aucValue[4];
}VOS_RTC_OM_INFO_STRU;

#ifndef FLOAT_SUPPORT
VOS_UINT32 RTC_MUL_32_DOT_768(VOS_UINT32 ulValue,VOS_UINT32 ulFileID, VOS_INT32 usLineNo);

VOS_UINT32 RTC_DIV_32_DOT_768(VOS_UINT32 ulValue,VOS_UINT32 ulFileID, VOS_INT32 usLineNo);

VOS_UINT32 RTC_MUL_DOT_32768(VOS_UINT32 ulValue,VOS_UINT32 ulFileID, VOS_INT32 usLineNo);
#endif

VOS_UINT32 RTC_TimerCtrlBlkInit(VOS_VOID);

VOS_UINT32 RTC_TimerTaskCreat(VOS_VOID);

VOS_UINT32 R_Stop32KTimer( HTIMER *phTm, VOS_UINT32 ulFileID, VOS_INT32 usLineNo, VOS_TIMER_OM_EVENT_STRU *pstEvent );

#define RTC_StopTimer( phTm )\
    V_StopRelTimer( (phTm), VOS_FILE_ID, __LINE__ )

VOS_UINT32 R_Get32KRelTmRemainTime( HTIMER * phTm, VOS_UINT32 * pulTime,
                                 VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

#define RTC_GetRelTmRemainTime( phTm, pulTime )\
    R_Get32KRelTmRemainTime( (phTm), (pulTime), VOS_FILE_ID, __LINE__ )

VOS_UINT32 V_Start32KCallBackRelTimer( HTIMER *phTm, VOS_PID Pid, VOS_UINT32 ulLength,
    VOS_UINT32 ulName, VOS_UINT32 ulParam, VOS_UINT8 ucMode, REL_TIMER_FUNC TimeOutRoutine,
    VOS_UINT32 ulPrecision, VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

VOS_UINT32 V_Start32KRelTimer( HTIMER *phTm, VOS_PID Pid, VOS_UINT32 ulLength,
    VOS_UINT32 ulName, VOS_UINT32 ulParam, VOS_UINT8 ucMode, VOS_UINT32 ulPrecision,
    VOS_UINT32 ulFileID, VOS_INT32 usLineNo);

#define RTC_StartTimer( phTm, Pid, ulLength, ulName, ulParam, ucMode, TimeOutRoutine)\
    V_StartCallBackRelTimer((phTm), (Pid), (ulLength), (ulName), (ulParam), (ucMode), (TimeOutRoutine), VOS_FILE_ID, __LINE__ )

VOS_UINT32 RTC_timer_running( VOS_VOID );

VOS_UINT32 RTC_CheckTimer( HTIMER  *phTm, VOS_UINT32 *ulTimerID,
                           VOS_UINT32 ulFileID, VOS_INT32 usLineNo );

VOS_UINT32 R_Restart32KRelTimer( HTIMER *phTm, VOS_UINT32 ulFileID,
                                 VOS_INT32 usLineNo );

VOS_UINT32 VOS_GetFirst32KTimer( VOS_VOID );

VOS_UINT32 VOS_GetSlice(VOS_VOID);

VOS_UINT64 VOS_Get64BitSlice(VOS_VOID);

VOS_UINT32 VOS_GetSliceUnit(VOS_VOID);

VOS_BOOL VOS_CalcTimerInfo(VOS_VOID);

VOS_BOOL RTC_CalcTimerInfo(VOS_VOID);

#if (VOS_WIN32 == VOS_OS_VER)
VOS_VOID VOS_MagnifyTimerLength(VOS_UINT32 ulRatio );
#endif

VOS_VOID RTC_GetDebugSocInfo(VOS_UINT32 *pulAction, VOS_UINT32 *pulSlice, VOS_UINT32 *pulCoreId);

VOS_VOID RTC_ReportOmInfo(VOS_VOID);

/* RTC timer end */


VOS_UINT32 VOS_DrxTimerCtrlBlkInit(VOS_VOID);
VOS_UINT32 VOS_DrxTimerTaskCreat(VOS_VOID);


#if ((OSA_CPU_CCPU == VOS_OSA_CPU) || (OSA_CPU_NRCPU == VOS_OSA_CPU)) && (FEATURE_ON == FEATURE_VOS_18H_TIMER)
VOS_UINT32 VOS_Bit64RtcTimerCtrlBlkInit(VOS_VOID);

VOS_UINT32 VOS_Bit64TimerTaskCreat(VOS_VOID);

VOS_UINT32 VOS_StartBit64Timer( HTIMER *phTm,
                                     VOS_PID Pid,
                                     VOS_UINT32 ulLength,
                                     VOS_UINT32 ulName,
                                     VOS_UINT32 ulParam,
                                     VOS_UINT32 ulFileID,
                                     VOS_INT32 lLineNo);
VOS_UINT32 VOS_StopBit64Timer( HTIMER *phTm,
                                     VOS_UINT32 ulFileID,
                                     VOS_INT32 lLineNo,
                                     VOS_TIMER_OM_EVENT_STRU *pstEvent);
VOS_UINT32 VOS_Bit64TimerCheckTimer( HTIMER *phTm );
VOS_UINT32 VOS_RestartBit64Timer( HTIMER *phTm,
                                        VOS_UINT32 ulFileID,
                                        VOS_INT32 lLineNo );
VOS_UINT32 VOS_GetBit64RemainTime( HTIMER *phTm,
                                   VOS_UINT32 *pulTime,
                                   VOS_UINT32 ulFileID,
                                   VOS_INT32 lLineNo );
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* _VOS_TIMER_H */

