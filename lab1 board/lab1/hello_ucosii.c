#include "includes.h"
#include <stdio.h>
#include <string.h>

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                         5       /* Number of identical tasks                          */

typedef struct {
    INT16U compTime;
    INT16U period;
    int start;
} PERIOD_ARG;

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
PERIOD_ARG    Period_Args[N_TASKS];

void PeriodTask(void *data);                 /* Function prototypes of tasks                  */
void TaskStart(void *data);                  /* Function prototypes of Startup task           */
static void TaskStartCreateTasks(void);

ISR_MESG ISR_Mesg;

int  main (void)
{
    OSInit();                                              /* Initialize uC/OS-II                      */
//    PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
//    PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

    OSStart();                                             /* Start multitasking                       */
    return 0;
}

void  TaskStart (void *pdata)
{
//#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
//    OS_CPU_SR  cpu_sr = 0;;
//#endif
//    INT16S     key;

    // RTOS_LAB1-3
//    INT16U     start;

    pdata = pdata;                                         /* Prevent compiler warning                 */\

//    OS_ENTER_CRITICAL();
//    PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
//    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
//    OS_EXIT_CRITICAL();



    // RTOS_LAB1-2 initialize ISR_Mesg
//    for(int i=0; i<50; i++)
//    	ISR_Mesg.mesg[0][i] = '\0';
    ISR_Mesg.top = 0;
    OSTimeSet(0);


    TaskStartCreateTasks();                                /* Create all the application tasks         */
    for (;;) {
//        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
//            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
//                PC_DOSReturn();                            /* Return to DOS                            */
//            }
//        }

          static int k=0;
          for(; k<ISR_Mesg.top; k++)
        	  printf("%s", ISR_Mesg.mesg[k]);
//        if(ISR_Mesg.length > 0){
//            printf("%s", ISR_Mesg.mesg);
//            OS_ENTER_CRITICAL();
//            ISR_Mesg.length = 0;
//            OS_EXIT_CRITICAL();
//            if(OSTime > 100){
//                printf("Press any key to exit...\n");
//                while(PC_GetKey(&key) == 0);
//                PC_DOSReturn();
//            }
//        }

        OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
        OSTimeDlyHMSM(0,0,1,0);                       /* Wait one second                          */
    }
}

// RTOS_LAB1-1: Create Period Task
static  void  TaskStartCreateTasks (void) {
    Period_Args[0].compTime = 1;
    Period_Args[0].period   = 3;
    Period_Args[0].start   = 0;
    Period_Args[1].compTime = 3;
    Period_Args[1].period   = 6;
    Period_Args[1].start   = 0;
    Period_Args[2].compTime = 4;
    Period_Args[2].period   = 9;
    Period_Args[2].start   = 0;
    OSTaskCreate(PeriodTask,
                (void *) &Period_Args[0],
                &TaskStk[0][TASK_STK_SIZE-1],
                (INT8U) 1);
    OSTaskCreate(PeriodTask,
                (void *) &Period_Args[1],
                &TaskStk[1][TASK_STK_SIZE-1],
                (INT8U) 2);
    OSTaskCreate(PeriodTask,
                (void *) &Period_Args[2],
                &TaskStk[2][TASK_STK_SIZE-1],
                (INT8U) 3);

}

// RTOS_LAB1-1: Period task prototype
void PeriodTask(void *pdata) {
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0;;
#endif
    int start;
    int end;
    int toDelay;
    PERIOD_ARG *arg = (PERIOD_ARG *) pdata;

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = arg->compTime;
    OSTCBCur->period = arg->period;
    OS_EXIT_CRITICAL();
//    arg->start = OSTimeGet();
    for (;;) {
    	int flag=1;
//    	sprintf(&ISR_Mesg.mesg[ISR_Mesg.top++],"Task%2d start: %d.\n",(int)OSTCBCur->OSTCBPrio, arg->start);
        while (OSTCBCur->compTime > 0) {
            // RTOS_LAB1-2 log deadline violation
        	if(flag == 1){
        		int i=0;
        		for(; i<3; i++){
					if(((Period_Args[i].period) - (OSTimeGet() - Period_Args[i].start)) == 0){
						sprintf(&ISR_Mesg.mesg[ISR_Mesg.top++],"%5d Task%2d exceed the deadline.\n",(int)OSTime , i+1);
						flag  = 0;
					}
        		}
        	}

            /* Computing */
        }
        OS_ENTER_CRITICAL();
        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end - arg->start);
        arg->start += (OSTCBCur->period);        // Next start time
//        if(toDelay <0){
//             sprintf(&ISR_Mesg.mesg[ISR_Mesg.top++],"%5d Task%2d exceed the deadline.\n",(int)OSTime , (int)OSTCBCur->OSTCBPrio);
//        }

        OSTCBCur->compTime = arg->compTime;        // Reset the counter
        OSTimeDly(toDelay);
        OS_EXIT_CRITICAL();
    }
}
