#ifdef CDD_PDURUPPER_START_SEC_CODE
# undef CDD_PDURUPPER_START_SEC_CODE
# define START_SEC_CODE
#endif
#ifdef CDD_PDURUPPER_STOP_SEC_CODE
# undef CDD_PDURUPPER_STOP_SEC_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_SchM_Task_CODE
# undef OS_START_SEC_SchM_Task_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_SchM_Task_CODE
# undef OS_STOP_SEC_SchM_Task_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_CanIsrErr_0_CODE
# undef OS_START_SEC_CanIsrErr_0_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_CanIsrErr_0_CODE
# undef OS_STOP_SEC_CanIsrErr_0_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_CanIsrRx_0_CODE
# undef OS_START_SEC_CanIsrRx_0_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_CanIsrRx_0_CODE
# undef OS_STOP_SEC_CanIsrRx_0_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_CanIsrTx_0_CODE
# undef OS_START_SEC_CanIsrTx_0_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_CanIsrTx_0_CODE
# undef OS_STOP_SEC_CanIsrTx_0_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_CanIsrWakeup_0_CODE
# undef OS_START_SEC_CanIsrWakeup_0_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_CanIsrWakeup_0_CODE
# undef OS_STOP_SEC_CanIsrWakeup_0_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_IO_Task_CODE
# undef OS_START_SEC_IO_Task_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_IO_Task_CODE
# undef OS_STOP_SEC_IO_Task_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_My_Task_CODE
# undef OS_START_SEC_My_Task_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_My_Task_CODE
# undef OS_STOP_SEC_My_Task_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_Os_TimerPitIsr_CODE
# undef OS_START_SEC_Os_TimerPitIsr_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_Os_TimerPitIsr_CODE
# undef OS_STOP_SEC_Os_TimerPitIsr_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_ERRORHOOK_CODE
# undef OS_START_SEC_ERRORHOOK_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_ERRORHOOK_CODE
# undef OS_STOP_SEC_ERRORHOOK_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_Init_Task_CODE
# undef OS_START_SEC_Init_Task_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_Init_Task_CODE
# undef OS_STOP_SEC_Init_Task_CODE
# define STOP_SEC_CODE
#endif

#ifdef OS_START_SEC_Background_Task_CODE
# undef OS_START_SEC_Background_Task_CODE
# define START_SEC_CODE
#endif
#ifdef OS_STOP_SEC_Background_Task_CODE
# undef OS_STOP_SEC_Background_Task_CODE
# define STOP_SEC_CODE
#endif

#include "Rte_MemMap.h"
#include "_MemMap.h"
