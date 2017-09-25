/*
 * hello2.c - First part of CS433 HW2
 * Created by Michael Anderson
 */

#include <linux/module.h> // Per LKMPG, required for *ALL* modules
#include <linux/kernel.h> // Needed for macro expansion for log levels
#include <linux/init.h>   // Required for __init, _exit, __initdata macros

static int hello2_data __initdata = 2;

static int __init hello2_init(void) {
	/*
         * NB: On Mint (based on Ubuntu) printk()  will print to
	 * /var/log/syslog - there is no /var/log/messages
	 * 
	 * Value of KERN_INFO is "6" 
	 */
	printk(KERN_INFO "Michael Anderson: Loading Hello2 module... Value of hello2_data: %d\n", hello2_data);
	return 0; // Non-0 indicates init_module failed - module can't be loaded (LKMPG)
}

static void __exit hello2_exit(void) {
	printk(KERN_INFO "Michael Anderson: Exiting Hello2 module...\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
