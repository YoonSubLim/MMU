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
	// page table 상에서 VFN 값으로 indexing 하여 PTE를 찾는다.
	// 이때 entry = (ku_cr3의 주소값) + (PTE_offset * 1(PTE_SIZE)) 한, PTE의 시작 주소값이다.
	entry = (char *)ku_cr3 + pte_offset; // xx + 25

	// entry인 PTE가 가지고 있는 값(*entry)의 비트 값 판별하여, page fault 여부 판단.
	// present bit 0 이거나, Unused bit 1인 경우 // page fault 발생
	if( !(*entry & 0x01) || (*entry & 0x02) ) // 1byte PTE 에서 비트 추출
		return 0;

	// PTE의 하위 2비트는 status 값이고, 상위 6비트가 실제 물리메모리 상 PFN 나타낸다.
	// 여기에 앞서 va에서 구해놓은 page 내 offset인 page_offset을 더해 Physical Address 값 구한다.
	// 하위 2비트에 offset 더함
	pa = (*entry & PFN_MASK) + page_offset; // PFN + 0 -> 실제 메모리 주소
	return pa;
}

