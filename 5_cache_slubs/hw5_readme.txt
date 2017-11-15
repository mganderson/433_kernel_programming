Implements a function that recreates /proc/slabinfo; a screenshot of the syslog output is included in this directory.

Modified kernel files are contained in the NEW_modified_kernel_files
directory.

- A new function, show_caches(), has been added to slub.c.  It generates and prints statistics about caches in kmalloc_caches and their constituent slabs.

- main.c has been modified at line 960 (towards the end of kernel_init) to call show_caches()

- slab.h has been modified to include show_caches()
