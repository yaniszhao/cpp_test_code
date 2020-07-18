#include "DriverFileOperations.h"
#include "ToolFunctions.h"

int DriverOpen(struct inode *pslINode, struct file *pslFileStruct)
{
	DEBUG_PRINT(DEVICE_NAME ": open invoked, do nothing\n");
	return 0;
}

int DriverClose(struct inode *pslINode, struct file *pslFileStruct)
{
	DEBUG_PRINT(DEVICE_NAME ": close invoked, do nothing\n");
	return 0;
}

ssize_t DriverRead(struct file *pslFileStruct, char __user *pBuffer, size_t nCount, loff_t *pOffset)
{
	DEBUG_PRINT(DEVICE_NAME ": read invoked, do nothing\n");
	return 0;
}

ssize_t DriverWrite(struct file *pslFileStruct, const char __user *pBuffer, size_t nCount, loff_t *pOffset)
{
	DEBUG_PRINT(DEVICE_NAME ": write invoked, do nothing\n");
	return 0;
}

long DriverIOControl(struct file *pslFileStruct, unsigned int uiCmd, unsigned long addr)
{
	int ret = 0;

	DEBUG_PRINT(DEVICE_NAME ": in DriverIOControl\n");
	DEBUG_PRINT(DEVICE_NAME":addr = %p\n", addr);

	if(1 == uiCmd)
	{
		DEBUG_PRINT(DEVICE_NAME ": uiCmd = 1\n");
		int a, b, sum;

		ret = copy_from_user(&a, (unsigned long *)addr, sizeof(a));
		DEBUG_PRINT(DEVICE_NAME":a = %d\n", a);

		ret = copy_from_user(&b, (unsigned long *)(addr+sizeof(a)), sizeof(b));	
		DEBUG_PRINT(DEVICE_NAME":b = %d\n", b);

		sum = a + b;

		ret = copy_to_user((unsigned long*)(addr+sizeof(a)+sizeof(b)), (unsigned long *)&sum, sizeof(sum));
		
		return 0;	
	}
	else
	{
		DEBUG_PRINT(DEVICE_NAME ": uiCmd != 1\n");
		return 2;
	}	
}

int DriverMMap(struct file *pslFileStruct, struct vm_area_struct *pslVirtualMemoryArea)
{
	DEBUG_PRINT(DEVICE_NAME ": mmap invoked, do nothing\n");
	return 0;
}
