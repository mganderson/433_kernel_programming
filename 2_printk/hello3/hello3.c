/*
 * hello3.c - Third part of CS433 HW2
 * Pass command line arguments using MODULE_PARM() macro
 * Created by Michael Anderson
 */

#include <linux/module.h> // Per LKMPG, required for *ALL* modules
#include <linux/kernel.h> // Needed for macro expansion for log levels
#include <linux/init.h>   // Needed for __init, __exit, __initdata macros
#include <linux/moduleparam.h>

static char *mystring = "YOLO";

module_param(mystring, charp, 0000);

static int __init hello3_init(void) {
	/*
         * NB: On Mint (based on Ubuntu) printk()  will print to
	 * /var/log/syslog - there is no /var/log/messages
	 * 
	 * Value of KERN_INFO is "6" 
	 */
	printk(KERN_INFO "Michael Anderson: Loading Hello3 module - Hello World 3.\n");
	printk(KERN_INFO "============================================\n");
	printk(KERN_INFO "Name: %s\n", mystring);
	printk(KERN_INFO "Street: %s\n", mystring);
	printk(KERN_INFO "City: %s\n", mystring);
	printk(KERN_INFO "ZIP code: %s\n", mystring);
	return 0; // Non-0 indicates init_module failed - module can't be loaded (LKMPG)
}

static void __exit hello3_exit(void) {
	printk(KERN_INFO "Michael Anderson: Exiting Hello3 module - Goodbye world 3.\n");
}

module_init(hello3_init);
module_exit(hello3_exit);
