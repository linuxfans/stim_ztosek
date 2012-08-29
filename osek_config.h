





	
#define TASK(TaskName) StatusType Task ## TaskName(void)
#define ISR(ISRName) StatusType ISR ## ISRName(void)

enum	StatusType
{
	E_OS_OK         = 0,
	E_OS_ACCESS     = 1,
	E_OS_CALLEVEL   = 2,
	E_OS_ID         = 3,
	E_OS_LIMIT      = 4,
	E_OS_NOFUNC     = 5,
	E_OS_RESOURCE   = 6,
	E_OS_STATE      = 7,
	E_OS_VALUE      = 8
};


