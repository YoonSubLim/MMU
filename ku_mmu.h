// 구조체 선언
typedef struct PCB
{
    /* data */
    char pid;
    // Page Table Base Register(PTBR) address
    
}PCB;

// 8비트 PTE
typedef struct ku_pte
{   
    // 포맷만 지켜주는 범위 내에서 나름의 정의 가능.
};

// ku_mmu_init
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size){
    // mem_size 만큼 메모리 할당
    void* pmem = malloc(mem_size);
    // swap_size 만큼 swap space 할당
    malloc(swap_size);
    // free list 관리. 생성. 11 slide.
    // 메모리 free list 에 추가 @ 전역 변수 필요
    // swap space 에 추가 @ 전역 변수 필요
    return pmem;
}

// ku_run_proc
int ku_run_proc(char pid, struct ku_pte **ku_cr3){
    // pid process 로 context switch
    // new process 이면 PCB 생성 // search 해서 없으면 malloc
    // ku_cr3 값 변경
    // 성공시 0

}

// ku_page_fault // 재배치 정책은 FIFO
int ku_page_fault(char pid, char va){
    // mapping 안된 건지 // swaped out 된 건지 판단 후
    // mapping 안되었다면 매핑
    // swapped out 되었다면 swap in
        // full 이면 eviction 

    // 무조건 조건 성립하도록 조치되어야 함.
}