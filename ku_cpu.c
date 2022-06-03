#include <stdio.h>
#include <stdlib.h>
// #include "./ku_mmu.h"

char ku_traverse(void *ku_cr3, char va);

void ku_mmu_fin(FILE *fd, void *pmem)
{
	if(fd) fclose(fd);
	if(pmem) free(pmem);
}

int main(int argc, char *argv[])
{
	FILE *fd=NULL;
	char fpid, pid=0, va, pa;
	unsigned int pmem_size, swap_size;
	void *ku_cr3, *pmem=NULL;

	// printf("%d", sizeof(ku_cr3)); // size 8
	// printf("%d", sizeof(temp)); // int* temp -> size 8

	if(argc != 4){
		printf("ku_cpu: Wrong number of arguments\n");
		return 1;
	}

	fd = fopen(argv[1], "r");
	if(!fd){
		printf("ku_cpu: Fail to open the input file\n");
		return 1;
	}

	pmem_size = strtol(argv[2], NULL, 10);
	swap_size = strtol(argv[3], NULL, 10);

	// 할당받은 pmem 주소값 return
	pmem = ku_mmu_init(pmem_size, swap_size);
	if(!pmem){
		printf("ku_cpu: Fail to allocate the physical memory\n");
		ku_mmu_fin(fd, pmem);
		return 1;
	}

	// %hhd 중 h는 자료형 한단계 감소. 숫자형 표시. 즉 char를 숫자로 표현.
	while(fscanf(fd, "%hhd %hhd", &fpid, &va) != EOF){

		// process ID가 달라졌다면
		if(pid != fpid){
			if(ku_run_proc(fpid, &ku_cr3) == 0) // 
				pid = fpid; /* context switch */
			else{
				printf("ku_cpu: Context switch is failed\n");
				ku_mmu_fin(fd, pmem);
				return 1;
			} 
		}

		// process의 cr3 값으로 traverse. 실패시 0.
		pa = ku_traverse(ku_cr3, va);
		if(pa == 0){
			if(ku_page_fault(pid, va) != 0){
				printf("ku_cpu: Fault handler is failed\n");
				ku_mmu_fin(fd, pmem);
				return 1;
			}
			printf("[%d] VA: %hhd -> Page Fault\n", pid, va);

			/* Retry after page fault */
			pa = ku_traverse(ku_cr3, va); 
			// 밑 내용 도달하면 안됨. 한번에 매핑 완료를 가정.
			if(pa == 0){
				printf("ku_cpu: Addr translation is failed\n");
				ku_mmu_fin(fd, pmem);
				return 1;
			}
		}

		printf("[%d] VA: %hhd -> PA: %hhd\n", pid, va, pa);
	}

	ku_mmu_fin(fd, pmem);
	return 0;
}
