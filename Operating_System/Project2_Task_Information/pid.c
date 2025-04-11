#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "pid"

static long l_pid;

static ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
	.write = proc_write,
};

static int proc_init(void) {
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);
        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
	return 0;
}

static void proc_exit(void) {
        remove_proc_entry(PROC_NAME, NULL);
        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos) {
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
	struct task_struct *tsk = NULL;
        if (completed) {
                completed = 0;
                return 0;
        }
	tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);

	if(tsk == NULL) {
		printk(KERN_INFO "Invalid PID %ld!", l_pid);
		rv = -1;
		return rv;
	}
	completed = 1;
	rv = sprintf(buffer, "command = [%s], pid = [%ld], state = [%ld]\n",
					tsk->comm, l_pid, tsk->state);

        if(copy_to_user(usr_buf, buffer, rv)) {
		rv = -1;
	}
        return rv;
}

static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t * pos) {
	char *k_men;
	k_men = kmalloc(count, GFP_KERNEL);
	if(copy_from_user(k_men, usr_buf, count)) {
		printk(KERN_INFO "Error copying from user\n");
		return -1;
	}

	sscanf(k_mem, "%ld", &l_pid);

	kfree(k_men);
	return count;
}

module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module for Task Information");
MODULE_AUTHOR("Xiaoqi");
