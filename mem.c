/*
 *
 * Import statements
 *
 * */

#include <stdio.h>
#include<stddef.h>
#include <stdlib.h>

#define MAX_SIZE 100000

char *memory_chunk;

/*
 * struct mem_block
 * Since we need to know if a particular mem_block is allocated or not and what is its size, we need this linkedlist mem_block structure
 * */
struct mem_block{
    size_t size; // size of the mem_block
    int free_flag; // free_flag flag (if mem_block is free_flag 1, otherwise 0)
    struct mem_block *next; // pointer to next mem_block
};

// Pointer pointing to the main memory_chunk mem_block which is initially free
struct mem_block *list=NULL;

//----------------------------------------------
/*
 * Function declarations
 *
 * */

void Mem_Init(int sizeofRegion);
void Seperate(struct mem_block *first_fit_slot, size_t size);
void *TEDU_alloc(size_t noOfBytes);
void Join();
int Mem_IsValid(void *ptr);
int Mem_GetSize(void *ptr);
int TEDU_GetStats();
void TEDU_Free(void* ptr);
void start(int sizeOfRegion);

//----------------------------------------------
// Function definitions

/*
 *
 * Function Mem_Init
 * Initializes the mem_block of memory_chunk
 *
 * */
void Mem_Init(int sizeOfRegion){
    list->size= sizeOfRegion - sizeof(struct mem_block); // assigns freelists size to MEMSIZE - size of the mem_block
    list->free_flag=1; // to indicate mem_block isn't allocated yet, we set the free_flag to 1
    list->next=NULL; // assigns next to null since the first memory_chunk mem_block hasn't been initialized yet
}
/*
 * This function starts our memory by malloc function.
 * It takes an integer for size
 * */
void start(int sizeOfRegion){
    memory_chunk = malloc(sizeOfRegion*sizeof(char));
    list = (void*) memory_chunk;
}
/*
 * Function Seperate
 * To find a free_flag mem_block to allocate memory_chunk this function uses the "First Fit Algorithm"
 * When we find a space that has enough memory_chunk for our allocation and use the first one
 * If we find a memory_chunk mem_block for our allocation which fits perfectly, splitting is not required
 * So, this function will execute only  when we have more than required
 * It takes two arguments respectively a pointer to mem_block which refers to the mem_block of memory_chunk which has size more than the required size the and required size of the memory_chunk mem_block to be allocated
 *
 * */
void Seperate(struct mem_block *first_fit_slot, size_t size){

    // creates a mem_block pointer called next_block. It points to the location which is obtained by first_fit_slot + size + size of the mem_block
    struct mem_block *next_block=(void*)((void*)first_fit_slot + size + sizeof(struct mem_block));

    // next_block pointer points to next memory location
    next_block->size= (first_fit_slot->size) - size - sizeof(struct mem_block);
    next_block->free_flag=1; // indicates next_block mem_block is free_flag
    next_block->next=first_fit_slot->next;


    first_fit_slot->size=size;
    first_fit_slot->free_flag=0;
    first_fit_slot->next=next_block;
}


/*
 * Function TEDU_alloc
 *
 *
 * */

void *TEDU_alloc(size_t noOfBytes){

    // Pointers for traversing through the list
    struct mem_block *current_block;
    struct mem_block *previous_block;

    // Void pointer to return the starting address of the allocated memory_chunk
    void *start_adress;

    /*
     * If the memory isn't already initialized this block will be executed and  will initialize the memory
     * */
    if(!(list->size)){
        Mem_Init(MAX_SIZE); // Calls MemInit
        printf("Memory initialization is successful! \n");
    }

    current_block=list; // points to the start of the memory block

    /*
     * this condition means that current block cannot be used for allocation. So we continue checking the new blocks
     * */
    while((((current_block->size) < noOfBytes) || ((current_block->free_flag) == 0)) && (current_block->next != NULL)){
        previous_block=current_block;
        current_block=current_block->next;
    }

    /*
     * this condition means that current blocks size exactly fits the required size
     * */
    if((current_block->size) == noOfBytes){

        current_block->free_flag=0; // sets the free flag to zero (since it is allocated)
        start_adress=(void*)(++current_block);
        return start_adress;
    }

        /*
         * this condition means, the block we checked has a memory which is greater than the required memory.
         * */
    else if((current_block->size) > (noOfBytes + sizeof(struct mem_block))){

        Seperate(current_block, noOfBytes); // splits the memory
        start_adress=(void*)(++current_block);
        // returns the starting address of the allocated memory
        return start_adress;
    }

        /* this block will be executed when there is not enough memory for allocation
         * prints an error message and returns a null pointer
         * */
    else{
        start_adress=NULL;
        printf("Error!! There aren't enough memory to allocate!!\n");
        //
        return start_adress;
    }
}

/*
 * Function Join
 * This function is for joining the consecutive free blocks
 * It removes the them by removing blocks lying between them
 *
 * */
void Join(){

    struct mem_block *current_block;
    struct mem_block *previous_block;

    current_block=list;

    // loop through memory blocks
    while((current_block->next) != NULL){

        // if the current block is free and next block is also free this condition will execute
        if((current_block->free_flag) && (current_block->next->free_flag)){

            // assigns current blocks size to currentblocks size + next blocks size + size of the memoryblock
            current_block->size+= (current_block->next->size) + sizeof(struct mem_block);

            // assigns current blocks next to current blocks next's next
            current_block->next=current_block->next->next;

        }
        // previous block will be set to current block and current block will be set to next
        previous_block=current_block;
        current_block=current_block->next;
    }
}

/* Function TEDU_Free
 * It takes the void pointer ptr as parameter and frees the memory_chunk space pointed to by ptr
 * which must have been returned by a previous call to TEDU_alloc().
 * Otherwise, or if free_flag(ptr) has already been called before, undefined behaviour occurs.
 * If ptr is NULL, no operation is performed.
 * */
void TEDU_Free(void* ptr){
    // checks if the address to of the pointer given as an argument to the function that if it actually lies within the address range of the memory_chunk array we used
    if(((void*)memory_chunk <= ptr) && (ptr <= (void*)(memory_chunk + MAX_SIZE))){
        struct mem_block* current_block=ptr;
        --current_block;
        current_block->free_flag=1; // sets free_flag flag to 1 (it means its freed)
        Join(); // merges the consecutive blocks that are freed
    }

        //If the pointer is not valid this message will be printed
    else printf("Given pointer is not valid. Please provide a valid one allocated by TEDU_alloc\n");
}

/*
 * Mem_IsValid Function
 * */
int Mem_IsValid(void *ptr){
    struct mem_block *current_block = list;
    int size,minus,memorylocation;

    // loops through the list
    while(list->next !=NULL){
        // operations to find the location of the memory
        size = current_block->size;
        minus = ((void*)memory_chunk + MAX_SIZE) - (void*)(current_block);
        memorylocation = (void*)memory_chunk + MAX_SIZE - minus;

        // checks if the memory location we obtained is valid. If it is, this block will be executed and  return 1
        if((((void*)memory_chunk+MAX_SIZE-(((void*)memory_chunk+MAX_SIZE)-(void*)(ptr)))>memorylocation) && (((void*)memory_chunk+MAX_SIZE-(((void*)memory_chunk+MAX_SIZE)-(void*)(ptr))) <=(memorylocation+=(int)current_block->size))){
            return 1;
        }
        current_block = current_block->next;
    }
    // if it is not valid returns 0
    return 0;

}

/*
 * Mem_GetSize Function
 * Takes a void pointer as parameter
 * Loops through the list until the end of the list. If Mem_IsValid function returns 1, Mem_GetSize function returns the current size of the current block as bytes,
 * else returns -1
 * */
int Mem_GetSize(void *ptr){
    struct mem_block *current_block = list;

    while(list->next !=NULL){
        if (Mem_IsValid(ptr)){
            return ((int)current_block->size);
        }
        current_block = current_block->next;
    }
    return -1;
}

int TEDU_GetStats(){
    struct mem_block* currentBlock = list;
    int count = 0;
    int fragmentcount= 0;
    double average_Fragment_Size;
    while(currentBlock->next !=NULL){
        if(currentBlock->free_flag == 0){
            count = count +(int)currentBlock->size;
        }
        fragmentcount++;
        currentBlock = currentBlock->next;
    }
    average_Fragment_Size =count/fragmentcount;
    if(count<=0){
        return 0;
    }
    printf("Fragment Count: %d\n",fragmentcount);
    printf("Memory is being used(bytes): %d \n",count);
    printf("Percent of the memory currently being used: %.4lf percent.\n",(double)count/MAX_SIZE);
    printf("Average Fragment Size(Bytes): %.2lf\n",average_Fragment_Size);
    return 1;
}