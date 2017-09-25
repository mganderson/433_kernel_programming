/*
 * hello1.c - First part of CS433 HW2
 * Created by Michael Anderson
 */

#include <linux/module.h> // Per LKMPG, required for *ALL* modules
#include <linux/kernel.h> // Needed for macro expansion for log levels

static int __init hello1_init(void) {
	/*
         * NB: On Mint (based on Ubuntu) printk()  will print to
	 * /var/log/syslog - there is no /var/log/messages
	 * 
	 * Value of KERN_INFO is "6" 
	 */
	printk(KERN_INFO "Michael Anderson: Loading Hello1 module - Hello World 1.\n");
	return 0; // Non-0 indicates init_module failed - module can't be loaded (LKMPG)
}

static void __exit hello1_exit(void) {
	printk(KERN_INFO "Michael Anderson: Exiting Hello1 module - Goodbye world 1.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
