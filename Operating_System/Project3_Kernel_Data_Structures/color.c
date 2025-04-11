#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>


struct color {
    char* name;
    int red;
    int blue;
    int green;
    struct list_head list;
};

static LIST_HEAD(color_list);

int proc_init(void) {
    printk(KERN_INFO "Loading Kernel Module for Data Struct of Color\n");

    //Initialization
    struct color* violet;
    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->name = "violet";
    violet->red = 138;
    violet->blue = 43;
    violet->green = 226;
    INIT_LIST_HEAD(&violet->list);
    //Add this struct to the tail of the list.
    list_add_tail(&violet->list, &color_list);

    struct color* tomato;
    tomato = kmalloc(sizeof(*tomato), GFP_KERNEL);
    tomato->name = "tomato";
    tomato->red = 255;
    tomato->blue = 71;
    tomato->green = 996;
    INIT_LIST_HEAD(&tomato->list);
    list_add_tail(&tomato->list, &color_list);

    struct color* olive;
    olive = kmalloc(sizeof(*olive), GFP_KERNEL);
    olive->name = "olive";
    olive->red = 128;
    olive->blue = 0;
    olive->green = 128;
    INIT_LIST_HEAD(&olive->list);
    list_add_tail(&olive->list, &color_list);

    //Go through the list and print the data of the colors
    struct color *ptr;
    list_for_each_entry(ptr, &color_list, list) {
        printk(KERN_INFO "Color-%s: Red %d, Blue %d, Green %d\n",
                        ptr->name, ptr->red, ptr->blue, ptr->green);
    }
    return 0;
}

void proc_exit(void) {
    printk(KERN_INFO "Removing Module\n");

    //Go through the list and free the memory.
    struct color *ptr, *next;
    list_for_each_entry_safe(ptr, next, &color_list, list) {
        printk(KERN_INFO "Removing Colors: %s\n", ptr->name);
        list_del(&ptr->list);
        kfree(ptr);    
    }

    printk(KERN_INFO "Colors Delete and Memory free are done\n");
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module Datat Struture of Colors");
MODULE_AUTHOR("Xiaoqi");
