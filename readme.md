# RTOS Lab1

## What

- Implement periodic tasks with uC/OS
    - Task sets 
        - (1, 3), (3, 6)
        - (1, 3), (3, 6), (4, 9)
- Show context switch behaviors
- Show deadline violations

## Issues
- `RTOS_LAB1-1` How to create a task executes exactly as (c, p)
    - add `compTime`,`period` infomation in `uCOS_II.H:OS_TCB`
    - implement `PeriodTask()` function in `TEST.C`
    - create TaskSet in `TEST.C:TaskStartCreateTasks`
    - current task decrement its `compTime` in `OS_CORE.c:OSTimeTick()`
- `RTOS_LAB1-2` How to print context switches info?
    - define a variable for ISR in `uCOS_II.C` `uCOS_II.H`
    - Initialize it in `TEST.C:TaskStart()`
    - log preemption in `OS_CORE.C:OSIntExit()`
    - log complete in `OS_CORE.C:OSSched()`
    - log deadline violation in `TEST.C:PeriodTask()`
    - print message in `TEST.c:TaskStart()`

If using vscode, view all change by searching `RTOS_LAB1` symbol 

## Implementation
```c
// Task Create 
OSTaskCreate(
    Task1,      // Task function pointer
    (void *)0,  // Extra Parameter
    &TaskStk[0][TASK_STK_SIZE-1], // Stack Pointer
    1,          // Priority
);

// Period Task
void Task(){
    int start;
    int end;
    int toDelay;
    start = OSTimeGet();
    while(1){

        // add compTime in uCOS-II.h:OS_TCB
        while(OSTCBCur->compTime>0){    // c ticks
            // busy occupying cpu
        }

        end = OSTimeGet();
        toDelay = (OSTCBCur->period) - (end-start);
        
        start = start + (OSTCBCur->period); // next start time
        OSTCBCur->compTime = c; // reset the counter (c ticks for computation)
        OSTimeDly(toDelay);
    }
}

/*
OS_CORE.c: OSTimeTick()
- called when a clock interrupt arrives
- add a code that decrement compTime

OS_CORE.c: OSIntExit()
- manage the scheduling after the system has come back from the calling of ISR
- print preemption here

OS_CORE.c: OS_Sched()
- called when a task voluntarily gives up its CPU
- print complete event here

OS_CORE.c: OSStart()
- find the hightest priority task and run it
*/

OSTimeDly(); // delay based on cycle
OSTimeDlyHMSM(); // delay based on actual time 
```

Printing message
- Print in evaluation board
    - printf("format string", prams...);
- Print in Dosbox
    - PC_DispStr()
- Don't print in ISR, it may sleep
    - Save outputs buffer and have a task print the results
    - Use critical section to access the buffer
 

## Slide Note

### uCOS-II
- a tiny open-source real-time kernel

### Run uCOS-II Example on Dosbox
open Dosbox
```powershell
mount c <path_to_this_dir>
cd c:\SOFTWARE\uCOS-II\EX1_x86L\BC45\test
maketest.bat
test.exe
```