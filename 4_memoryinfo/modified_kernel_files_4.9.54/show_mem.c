/*
 * Generic show_mem() implementation
 *
 * Copyright (C) 2008 Johannes Weiner <hannes@saeurebad.de>
 * All code subject to the GPL version 2.
 */

#include <linux/mm.h>
#include <linux/quicklist.h>
#include <linux/cma.h>

void show_mem(unsigned int filter)
{
	pg_data_t *pgdat;
	unsigned long total = 0, reserved = 0, highmem = 0;

	printk("Mem-Info:\n");
	show_free_areas(filter);

	for_each_online_pgdat(pgdat) {
		unsigned long flags;
		int zoneid;

		pgdat_resize_lock(pgdat, &flags);
		for (zoneid = 0; zoneid < MAX_NR_ZONES; zoneid++) {
			struct zone *zone = &pgdat->node_zones[zoneid];
			if (!populated_zone(zone))
				continue;

			total += zone->present_pages;
			reserved += zone->present_pages - zone->managed_pages;

			if (is_highmem_idx(zoneid))
				highmem += zone->present_pages;
		}
		pgdat_resize_unlock(pgdat, &flags);
	}

	printk("Michael Anderson: %lu pages RAM\n", total);
	printk("Michael Anderson: %lu pages HighMem/MovableOnly\n", highmem);
	printk("Michael Anderson: %lu pages reserved\n", reserved);
#ifdef CONFIG_CMA
	printk("Michael Anderson: %lu pages cma reserved\n", totalcma_pages);
#endif
#ifdef CONFIG_QUICKLIST
	printk("Michael Anderson: %lu pages in pagetable cache\n",
		quicklist_total_size());
#endif
#ifdef CONFIG_MEMORY_FAILURE
	printk("Michael Anderson: %lu pages hwpoisoned\n", atomic_long_read(&num_poisoned_pages));
#endif
}

/* Michael Anderson: Attempt to recreate /proc/buddyinfo information 
 * Based *HEAVILY* on show_mem() above
 */
void show_buddyinfo() {
	printk("Michael Anderson: Inside show_buddyinfo() within show_mem.c\n");
	
	// Each numa node is an entry in pgdat (?)
	// We expect to have only 1 node in pgdat
	pg_data_t *pgdat;
	int nodeCount;
	
	// Again, we expect only one node, but this
	// loop will iterate over each node
	nodeCount = 0;
	for_each_online_pgdat(pgdat) {
		unsigned long flags;
		int zoneid, order;
		
		// Semaphore related? Not sure.
		pgdat_resize_lock(pgdat, &flags);
		
		// Iterate over each zone in a given node	
		for (zoneid = 0; zoneid < MAX_NR_ZONES; zoneid++) {
			struct zone *zone = &pgdat->node_zones[zoneid];
			
			// Go on to the next iteration if zone not populated
			if (!populated_zone(zone))
				continue;

			// I know this code is awful, but easiest way
			// I am aware of of getting print k statement all
			// on one line, as in /proc/buddyinfo for orders 0-10	
			printk( "Michael Anderson: Node %lu\t"
				"Zone %s\t"
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\t" 
				"%d\n", 
				nodeCount,
				zone->name,
				zone->free_area[0].nr_free,
				zone->free_area[1].nr_free,
				zone->free_area[2].nr_free,
				zone->free_area[3].nr_free,
				zone->free_area[4].nr_free,
				zone->free_area[5].nr_free,
				zone->free_area[6].nr_free,
				zone->free_area[7].nr_free,
				zone->free_area[8].nr_free,
				zone->free_area[9].nr_free,
				zone->free_area[10].nr_free
				);
			// Forget this - just insert the 11 orders into
			// the printk statement above real ugly like
			/*			
			// Iterate over free_area[order] from 0 to 10
			for (order = 0; order < 11; order++) {
				struct free_area *free_area = &zone->free_area[order]; 
				printk("%d\t", free_area->nr_free);						
			} 
			*/
		}
		// Assuming this is also semaphore related
		pgdat_resize_unlock(pgdat, &flags);
		nodeCount++;
	}
	
}
