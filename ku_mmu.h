#include <stdio.h>
#include <stdlib.h>
#define ku_mmu_PAGESIZE 4

// ku_cr3

// 8비트 PTE
typedef struct ku_pte
{   
    // 포맷(크기)만 지켜주는 범위 내에서 나름의 정의 가능.
    int a; int b; // temp var for size
};

// 구조체 선언
typedef struct PCB
{
    /* data */
    char pid;
    // Page Table Base Register(PTBR) address
    struct ku_pte *ptbr; // 변수 이름 바꾸어야 하나 // 페이지 테이블 시작주소 가리킴
}PCB;

// 
typedef struct PAGE_LIST_NODE
{
    // page frame number
    // 이 페이지를 어떤 페이지 테이블 엔트리가 가리키고 있는지
    // int PFN;
    // 매핑 정보
    // 매핑되어 있다면 매핑하고 있는 PTE는 누구인지.
    void* address;
    PAGE_LIST_NODE* next;
}PAGE_LIST_NODE;

PAGE_LIST_NODE* ku_mmu_mem_freelist = NULL;
PAGE_LIST_NODE* ku_mmu_mem_freelist_cur = NULL;

PAGE_LIST_NODE* ku_mmu_mem_alloclist = NULL;
PAGE_LIST_NODE* ku_mmu_swap_freelist = NULL;
PAGE_LIST_NODE* ku_mmu_swap_alloclist = NULL;

void insertNode(PAGE_LIST_NODE* list){
    if(list == NULL){
        // 헤드 초기화
        list = (PAGE_LIST_NODE*)malloc(sizeof(PAGE_LIST_NODE));
        ku_mmu_mem_freelist_cur = list;
    }else{
        // 끝에 추가
        PAGE_LIST_NODE* newNode = (PAGE_LIST_NODE*)malloc(sizeof(PAGE_LIST_NODE));
        newNode->address = NULL; // 수정 0은 제외하고 page 사이즈 별로 할당하면 될 듯
        newNode->next = NULL;
        ku_mmu_mem_freelist_cur->next = newNode; // mem 이랑 
    }
}

// 메모리, 스왑공간 시작 주소 받아와서 count 만큼 free page 연결해주기
void* ku_mmu_initList(int count){
    for(int i=0; i<count; i++){
        insertNode(ku_mmu_mem_freelist);
        insertNode(ku_mmu_swap_freelist);
    }
}

// ku_mmu_init
// memory Maximum : 256 bytes(64 pages, 2^6) // swap Maximum 512 bytes(128 pages, 2^7)
void *ku_mmu_init(unsigned int mem_size, unsigned int swap_size){
    // mem_size 만큼 메모리 할당
    void* pmem = malloc(mem_size);
    int mem_list_count = mem_size / ku_mmu_PAGESIZE; // memory free alloc list 총 개수 // 256/4 = 64개
    ku_mmu_initList(mem_list_count); // 64개 free page node 다른 메모리 공간에 연결
    ku_mmu_mem_alloclist = NULL; // 비워두기

    // swap_size 만큼 swap space 할당
    void* pswap = malloc(swap_size);
    int swap_list_count = swap_size / ku_mmu_PAGESIZE; // swap space free alloc list 총 개수
    ku_mmu_initList(swap_list_count); // 연결해주기
    ku_mmu_swap_alloclist = NULL; // 비워두기

    return pmem;
}

// ku_run_proc
// main 에서 void* ku_cr3로 선언한 값이 넘어옴. 인자로 캐스팅.
int ku_run_proc(char pid, struct ku_pte **ku_cr3){
    // pid process 로 context switch
    // new process 이면 PCB 생성. PCB에 ku_cr3 값 포함 // search 해서 없으면 malloc PCB
    // pid로 찾되, 매핑 정보가 없으면 넘어감
    search PCB by pid
    if then // 매핑된 해당 pid가 있다면,
        that ku_cr3 = PCB->ku_cr3 // ku_cr3이 가리키는 값에 들어갈 값은 PCB 가 포함하는 ku_cr3값으로 갱신
    else // 매핑된 해당 pid가 없다면,
        new PCB // 새로 할당
        new PCB->ku_cr3 // PCB가 가지고 있는 ku_cr3 새로 할당
        PCB->pid = pid; // PCB 값들 초기화
        ku_cr3 = that PCB->ku_cr3 // dummy value(?). 해당 PCB가 가지고 있는 ku_cr3 값
    // ku_cr3 값 변경 // new PCB 가 가지는 ku_cr3 값으로.
    // 성공시 0

}

// ku_page_fault // 재배치 정책은 FIFO
int ku_page_fault(char pid, char va){
    // mapping 안된 건지 // swaped out 된 건지 판단 후 // PT의 PTE의 상태 값을 봐야함
    // ku_cr3 값을 알아야 함.
    entry = (char *)ku_cr3 + pte_offset; // xx + 25


    // mapping 안되었다면 매핑
    // swapped out 되었다면 swap in
        // full 이면 eviction 
    // 이때 free list와 alloc list 를 활용한다.
    // 무조건 조건 성립하도록 조치되어야 함.
}