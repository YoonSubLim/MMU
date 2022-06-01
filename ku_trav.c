#define OFFSET_MASK 0x03 // 0000 0011
#define PFN_MASK 0xFC // 1111 1100

char ku_traverse(void *ku_cr3, char va)
{
	char *entry;
	char pa;

	// 상위 6비트는 VPN 을 의미
	char pte_offset = (va & PFN_MASK) >> 2;
	// 하위 2비트는 PTE 내 offset을 의미한다.
	char page_offset = (va & OFFSET_MASK);

	if( va == 0 )
		return 0; /* NULL pointer */

	/* page table */
	entry = (char *)ku_cr3 + pte_offset;

	// present bit 0 이거나, Unused bit 1인 경우 // page fault 발생
	if( !(*entry & 0x01) || (*entry & 0x02) )
		return 0;

	// 하위 2비트에 offset 더함
	pa = (*entry & PFN_MASK) + page_offset;
	return pa;
}

