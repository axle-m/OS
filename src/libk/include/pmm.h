#pragma once

#include "ktypes.h"
#include "../../kernel/boot_info.h"

#define PAGE_SIZE 4096

void pmm_init(boot_info *boot);

void *pmm_alloc_page(void);
void pmm_free_page(void *page);

uint32_t pmm_free_pages(void);
uint32_t pmm_total_pages(void);