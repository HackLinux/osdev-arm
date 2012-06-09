#define NUM_PAGE_TABLE_ENTRIES 4096 /* 1 entry per 1MB, so this covers 4G address space */
#define CACHE_DISABLED    0x12
#define SDRAM_START       0x80000000
#define SDRAM_END         0x8fffffff
#define CACHE_WRITEBACK   0x1e

void mmu_init(void)
{
    static unsigned int __attribute__((aligned(16384))) page_table[NUM_PAGE_TABLE_ENTRIES];
    int i;
    unsigned int reg;

    /* Set up an identity-mapping for all 4GB, rw for everyone */
    for (i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++)
        page_table[i] = i << 20 | (3 << 10) | CACHE_DISABLED;

    /* Then, enable cacheable and bufferable for RAM only */
    for (i = SDRAM_START >> 20; i < SDRAM_END >> 20; i++)
        page_table[i] = i << 20 | (3 << 10) | CACHE_WRITEBACK;

    /* Copy the page table address to cp15 */
    asm volatile("mcr p15, 0, %0, c2, c0, 0"
            : : "r" (page_table) : "memory");
    /* Set the access control to all-supervisor */
    asm volatile("mcr p15, 0, %0, c3, c0, 0" : : "r" (~0));

    /* Enable the MMU */
    asm("mrc p15, 0, %0, c1, c0, 0" : "=r" (reg) : : "cc");
    reg|=0x1;
    asm volatile("mcr p15, 0, %0, c1, c0, 0" : : "r" (reg) : "cc");
}
