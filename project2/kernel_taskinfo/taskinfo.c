#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#define BUFFER_SIZE 128
#define PROC_NAME "taskinfo"

char* bash;
int state, pid_val, error;

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
ssize_t proc_write(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.read = proc_read,
	.write = proc_write,
};

/* This function is called when the module is loaded. */
int proc_init(void)
{
	/* creates the /proc/hello entry */
	proc_create(PROC_NAME, 0666, NULL, &proc_ops);
	return 0;
}
/* This function is called when the module is removed. */
void proc_exit(void)
{
	/* removes the /proc/hello entry */
	remove_proc_entry(PROC_NAME, NULL);
}

/* This function is called each time /proc/~ is read */
ssize_t proc_read(struct file *file, char __user *usr_buf,
size_t count, loff_t *pos)
{
	int rv = 0;
	char buffer[BUFFER_SIZE];
	static int completed = 0;
	if (completed) {
		completed = 0;
		return 0;
	}
	completed = 1;
	if (error == 0){
		rv = sprintf(buffer, "command = [%s] pid = [%d] state = [%d]\n", bash, pid_val, state);
	}
	else{
		rv = sprintf(buffer, "Invalid PID: %d\n", pid_val);
	}
	/* copies kernel space buffer to user space usr_buf */
	copy_to_user (usr_buf, buffer, rv);
	return rv;
}

ssize_t proc_write(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
	int rv = 0;
	char *k_mem;
	/* allocate kernel memory */
	k_mem = kmalloc (count, GFP_KERNEL);
	/* copies user space usr buf to kernel memory */
	copy_from_user (k_mem, usr_buf, count);
	k_mem[count - 1] = '\0';
	long npid = simple_strtol(k_mem,NULL,10);
	printk(KERN_INFO "%s \t %ld \n", k_mem,npid);
	struct pid* kpid = find_vpid(npid);
	struct task_struct* task = pid_task(kpid,PIDTYPE_PID);
	/* return kernel memory */
	if (task == NULL) error = 1;
	else{
		state = task->state;
		pid_val = task->pid;
		bash = task->comm;
		kfree (k_mem);
		error = 0;
	}
	return count;
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Taskinfo Module");
MODULE_AUTHOR("SGG");