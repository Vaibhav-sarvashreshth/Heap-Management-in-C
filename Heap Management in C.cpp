#include<stdio.h>
#include<stdlib.h>           /* Used only for malloc function*/
#define HEAP_SIZE 100000

typedef struct ID_heap
{
	int Status;               // Status==1 menas memory is currently occuiped Status==0 menas it is free to use
    int ID;
    int Block_Size;
    struct ID_heap *next;
    struct ID_heap *before;
}Heap;

Heap* Initialize_Heap(Heap* p)
{
    p->Status = 0;
    p->ID = -1;
    p->Block_Size = 4096;   //2^12
    p->next = NULL;
    p->before = NULL;

    return p;
}

Heap* Pointer_To_Heap; /* Global pointer to Heap structure */


void Allocate_Memory(int Space) /* Allocation of Blocks */
{
    Heap* Next_Memory;
    Heap* Current_Pointer = Pointer_To_Heap; 	/* Copying Pointer_To_Heap into temporory heap pointer*/ 
    
    static int ID = 0;			/* Static Int */
    int reserve_mem = 0, Avaible = 1;

    if(Current_Pointer->Block_Size<=Space || Current_Pointer->Status==1)
    {
        Avaible = 0;
        while(Current_Pointer->next!=NULL && Avaible==0)
        {
            Current_Pointer = Current_Pointer->next;

            if(Current_Pointer->Block_Size>=Space  && Current_Pointer->Status==0)
            {
                Avaible = 1;
            }
        }
    }

    if(Avaible==1)                           // if Space is avaible then
    {
        reserve_mem = Current_Pointer->Block_Size;

        Current_Pointer->ID = ID;                  // Give it a ID
        Current_Pointer->Status = 1; 				//Make Status =1
        Current_Pointer->Block_Size = Space;       

		// Now after allocation of memory set new memory in heap
        Next_Memory = Current_Pointer + Space;        // Incrementing the pointer by Space if Space is Avaibleble
        Next_Memory = Initialize_Heap(Next_Memory);		    // Initialize it
        Next_Memory->Block_Size = reserve_mem - Space; // Current_Pointer->Block_Size-Space

        Next_Memory->next = Current_Pointer->next;
        Current_Pointer->next = Next_Memory;
        Next_Memory->before = Current_Pointer;

        printf("Size of block created = %d\n ID of block = %d\n", Current_Pointer->Block_Size, Current_Pointer->ID);

        ID++;                            // incrementing ID as it is static and will be use for other structures
    }
    else
    {
        printf("The block size you have entered exeeds the maximum Avaibleable Space. Please try again.");
    }
}


void Free_The_memory(int block_id) 			/* Removal of Blocks*/
{		
    Heap* free_ptr = NULL;
    int Avaible = 0,yes=1;       				 /* To check if free block is avaible or not*/
    Heap* Current_Pointer = Pointer_To_Heap;			 /* Current_Pointer pointing to Pointer_To_Heap*/
    
	/* To free memory we check the id */
    while(Current_Pointer != NULL && Avaible==0) /* Travers till we didnt get block_id*/
    {
        if(Current_Pointer->ID == block_id)
        {	
        	/* Now as we got the id, free the memory*/
            free_ptr = Current_Pointer;
            free_ptr->ID = -1;
            free_ptr->Status = 0;
            
            Avaible = 1; 					/* It will break the while loop */
        }

        Current_Pointer = Current_Pointer->next;
    }

    if(Avaible==yes)
    {
        printf("\nMemory block of size: %d and ID: %d has been freed\n", free_ptr->Block_Size, block_id);
    }
    else
    {
        printf("\nBlock of ID:  %d not found\n", block_id);
    }
}


void merge_adjacent() /* Merging Consecutive Free Blocks Into One Block*/
{
    Heap* ptr_temp = NULL;
    int final_size;                        
    Heap* Current_Pointer = Pointer_To_Heap;               

    while(Current_Pointer!=NULL)
    {	/*We are looking for status=1, till we find it, let loop run*/
        if(Current_Pointer->Status==0)
        {
            final_size = Current_Pointer->Block_Size;
            ptr_temp = Current_Pointer->next;

            while(ptr_temp!=NULL && ptr_temp->Status==0)
            {
                final_size += ptr_temp->Block_Size;  // increment the final size 
                ptr_temp = ptr_temp->next;
            }

            Current_Pointer->Block_Size = final_size;
            Current_Pointer->next = ptr_temp;

            if(ptr_temp != NULL)
            {
                ptr_temp->before = Current_Pointer;         // traverse back 
            }

            Current_Pointer = ptr_temp;
        }
        else
        {
            Current_Pointer = Current_Pointer->next;
        }
    }
}



void Display() /* Displaying All The Actions And Current Size of The Block */
{
    Heap* Current_Pointer = Pointer_To_Heap;
    printf("\nDisplaying the heap allocations as follows:\n");
    while(Current_Pointer != NULL)
    {
        printf("Size Status = %d\t", Current_Pointer->Block_Size);
        Current_Pointer = Current_Pointer->next;
    }

    printf("\n\n");
}

/*displaying all possible choices in front of the user*/
void options()
{
    int op;
    int Block_Size;
	
    printf("PLease Choose One Of The Following Options - \n\n");
    printf("1) Allocate Memory\n");
    printf("2) Free Memory\n");
    printf("3) Display Memory Status\n");
    printf("4) Exit Program\n");

    printf("\nEnter the action you would like to implement: ");
    scanf("%d", &op);

	if(op==1)         /* User want to allocate memory*/
	{
		printf("\nEnter the size of the block to be allocated: ");
        scanf("%d", &Block_Size);
        Allocate_Memory(Block_Size);
        Display();
        options();
	}
	else if(op==2)  /* User want to free up Space*/
	{
		printf("\nEnter the ID of the block to be free: ");
        scanf("%d", &Block_Size);
        Free_The_memory(Block_Size);
        merge_adjacent();
        Display();
        options();
	}
	else if(op==3)   /* User want to see Heap Allocation Satus*/
	{
		Display();
        options();
	}
	else if(op==4)  /* User want to exit the program*/
	{
		printf("\n\nSuccessfully Exited From The Program\n");
		printf("\nHave A Nice Day !!!\n");
	}
	else            /* User haven`t entered the right option*/
	{
		printf("\n\nThe Option Is Not Valid. Please Try Again.");
        options();
	}
    
}

int main()
{
	printf("\n\t\t\t----------------------------------------------------------------------------\n");
	printf("\t\t\t###########################################################################");
    printf("\n\t\t\t############                                                   ############");
    printf("\n\t\t\t############             Heap Managment in c                   ############");
    printf("\n\t\t\t############                                                   ############");
    printf("\n\t\t\t###########################################################################");
    printf("\n\t\t\t---------------------------------------------------------------------------\n\n");
    
    
    Pointer_To_Heap = (Heap*)malloc(HEAP_SIZE);
    Pointer_To_Heap = Initialize_Heap(Pointer_To_Heap);
    options();
    return 0;
}
