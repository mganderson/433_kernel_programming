Output screenshots for both parts 1 and 2 of the assignment
are found within output_screenshots.  

The following kernel files were modified (based on v 4.9.54):

- show_mem.c
	- Modify show_mem() printk statements (Part 1)
	- Create a new function, show_buddyinfo, that replicates
	  the output of cat /proc/buddyinfo as printk statements
	  by printing nr_free for free_area[0] through [10] for
	  each populated zone for each node (Part 2)

- page_alloc.c
	- Modify show_free_areas() and show_node() (Part 1)

- main.c
	- Insert show_mem() and show_buddyinfo() towards the end
	  of start_kernel()

- mm.h
	- Add show_buddyinfo() prototype
