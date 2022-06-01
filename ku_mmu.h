// 구조체 선언

// ku_mmu_init
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size){

}

// ku_run_proc
int ku_run_proc(char pid, struct ku_pte **ku_cr3){
    // pid process 로 context switch
    // new process 이면 PCB 생성 // search 하면 될 듯
    // ku_cr3 값 변경
    // 성공시 0

}

// ku_page_fault
int ku_page_fault(char pid, char va){
    // mapping 안된 건지 // swaped out 된 건지 판단 후
    // mapping 안되었다면 매핑
    // swapped out 되었다면 swap in
        // full 이면 eviction 

    // 무조건 조건 성립하도록 조치되어야 함.
}