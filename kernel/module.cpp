#include <stddef.h>
#include <stdint.h>

#include <kernel/module.hpp>
#include <kernel/tty.hpp>

#include <kernel/memory/manager.hpp>

void_fn get_module_funct(multiboot_info_t * mb_info, int mdl_index) {
	module_t * modules;
    if (mb_info->mods_count > 0) {
        modules = (module_t *) (mb_info->mods_addr + KERNEL_VIRT_BASE);
        for (uint8_t i = 0; i < mb_info->mods_count; i++)
			if(i == mdl_index)
            	return (void_fn)(modules[i].mod_start + KERNEL_VIRT_BASE);
    }
}