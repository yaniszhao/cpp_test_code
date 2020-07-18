#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <asm/atomic.h>
#include <linux/rwsem.h>
#include <asm/uaccess.h>
#include "DriverFileOperations.h"
#include "DriverMain.h"
#include "ToolFunctions.h"

DEFINE_PER_CPU(long, gUsage) = 0;

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
	int i;
	long *pUsage = NULL;

	DEBUG_PRINT(DEVICE_NAME ": write invoked, do nothing\n");

	pUsage = this_cpu_ptr((long *)(&gUsage));
	
	DEBUG_PRINT(DEVICE_NAME ": pUsage = 0x%lx", (unsigned long)pUsage);

	for(i = 0; i < 10; i++)
	{
		DEBUG_PRINT(DEVICE_NAME ": CPU %d base = %lx\n", i, __per_cpu_offset[i]);
	}

	DEBUG_PRINT(DEVICE_NAME ": NR_CPUS = %d\n", NR_CPUS);

	return 0;
}

long DriverIOControl(struct file *pslFileStruct, unsigned int uiCmd, unsigned long ulArg)
{
	long *pUsage = NULL;

	DEBUG_PRINT(DEVICE_NAME ": ioctl invoked, do nothing\n");

	preempt_disable();
	pUsage = this_cpu_ptr((long *)(&gUsage));
	(*pUsage)++;
	int num=(*pUsage);
	preempt_enable();

	//DEBUG_PRINT(DEVICE_NAME ": gUsage = 0x%lx", (unsigned long)&gUsage);
	DEBUG_PRINT(DEVICE_NAME ": pUsage = 0x%lx", (unsigned long)pUsage);
	//DEBUG_PRINT(DEVICE_NAME ": num = %d", num);

	return 0;
}

int DriverMMap(struct file *pslFileStruct, struct vm_area_struct *pslVirtualMemoryArea)
{
	DEBUG_PRINT(DEVICE_NAME ": mmap invoked, do nothing\n");
	return 0;
}

void CountTotalUsage(void)
{
	int i=0;
	int num=0;
	int total=0;

	for_each_online_cpu(i)
	{
		num = *( (unsigned long*)(__per_cpu_offset[i] + (unsigned long)(&gUsage)) );
		total += num;
		DEBUG_PRINT(DEVICE_NAME ":cpu%d = %d", i, num);
	}

	DEBUG_PRINT(DEVICE_NAME ":total cpu usage = %d", total);
	return;
}