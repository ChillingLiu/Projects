#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

static int start = 25;
module_param(start, int, 0);

struct collatz {
    int value;
    struct list_head list;
};
static LIST_HEAD(collatz_list);

int proc_init(void) {
    printk(KERN_INFO "Loading Kernel Module for Data Struct of Collatz Sequence\n");

    //Initialization
    struct collatz* s_start;
    s_start = kmalloc(sizeof(*s_start), GFP_KERNEL);
    s_start->value = start;
    INIT_LIST_HEAD(&s_start->list);
    //Add this struct to the tail of the list.
    list_add_tail(&s_start->list, &collatz_list);

    //Collatz Sequence Generation
    int tmp = start;
    struct collatz* s_tmp;
    while (tmp > 1) {
        s_tmp = kmalloc(sizeof(*s_tmp), GFP_KERNEL);
        if (tmp % 2 == 0) {
            tmp = tmp / 2;
            s_tmp->value = tmp;
            INIT_LIST_HEAD(&s_tmp->list);
            list_add_tail(&s_tmp->list, &collatz_list);
        }
        else {
            tmp = 3 * tmp + 1;
            s_tmp->value = tmp;
            INIT_LIST_HEAD(&s_tmp->list);
            list_add_tail(&s_tmp->list, &collatz_list);
        }
    }

    //Go through the list and print the collatz sequence
    struct collatz *ptr;
    int i = 1;
    list_for_each_entry(ptr, &collatz_list, list) {
        printk(KERN_INFO "%dth iteration: %d", i, ptr->value);
        i++;
    }
    return 0;
}

void proc_exit(void) {
    printk(KERN_INFO "Removing Module\n");

    //Go through the list and free the memory.
    struct collatz *ptr, *next;
    int i;
    list_for_each_entry_safe(ptr, next, &collatz_list, list) {
        printk(KERN_INFO "Removing Collatz Sequence: %dth element: %d\n", i, ptr->value);
        list_del(&ptr->list);
        kfree(ptr);
    }

    printk(KERN_INFO "Collatz Sequence Delete and Memory free are done\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module Datat Struture of Collatz Sequence");
MODULE_AUTHOR("Xiaoqi");
