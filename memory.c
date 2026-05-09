#include "banking.h"

static MemoryFootprint memory_footprints[MAX_CUSTOMERS];
static int num_footprints = 0;

void init_memory_footprints(void)
{
    memory_footprints[num_footprints++] = (MemoryFootprint){
        0, "Zain", {0, 1, 2, -1, -1}, 3};
    memory_footprints[num_footprints++] = (MemoryFootprint){
        1, "Sara", {3, 4, 5, -1, -1}, 3};
    memory_footprints[num_footprints++] = (MemoryFootprint){
        2, "Hamza", {6, 7, 8, 9, -1}, 4};
    memory_footprints[num_footprints++] = (MemoryFootprint){
        3, "Fatima", {10, 11, 12, 13, 14}, 5};
    memory_footprints[num_footprints++] = (MemoryFootprint){
        4, "Ali", {15, 16, 17, -1, -1}, 3};
    
    printf("[MEMORY] Initialized memory footprints for %d customers\n", num_footprints);
}

static int page_in_frames(int frames[], int nf, int page)
{
    for (int i = 0; i < nf; i++)
        if (frames[i] == page) return 1;
    return 0;
}

static void print_frame(int frames[], int nf, int page, int fault)
{
    printf("  Page %2d | [", page);
    for (int i = 0; i < nf; i++) {
        if (frames[i] < 0) printf(" -- ");
        else               printf(" %2d ", frames[i]);
    }
    printf("] %s\n", fault ? "<- PAGE FAULT" : "   HIT");
}

static int fifo(int pages[], int np, int nf)
{
    int frames[NUM_FRAMES];
    for (int i = 0; i < nf; i++) frames[i] = -1;
    int ptr = 0, faults = 0;

    printf("\n  -- FIFO (frames=%d) --\n", nf);
    for (int i = 0; i < np; i++) {
        int pg = pages[i];
        if (!page_in_frames(frames, nf, pg)) {
            frames[ptr] = pg;
            ptr = (ptr + 1) % nf;
            faults++;
            print_frame(frames, nf, pg, 1);
        } else {
            print_frame(frames, nf, pg, 0);
        }
    }
    return faults;
}

static int lru(int pages[], int np, int nf)
{
    int frames[NUM_FRAMES];
    int last[NUM_FRAMES];
    for (int i = 0; i < nf; i++) { frames[i] = -1; last[i] = -1; }
    int faults = 0;

    printf("\n  -- LRU (frames=%d) --\n", nf);
    for (int t = 0; t < np; t++) {
        int pg = pages[t];

        int hit = -1;
        for (int i = 0; i < nf; i++)
            if (frames[i] == pg) { hit = i; break; }

        if (hit >= 0) {
            last[hit] = t;
            print_frame(frames, nf, pg, 0);
        } else {
            int replace = 0, min_t = INT_MAX;
            for (int i = 0; i < nf; i++) {
                if (frames[i] < 0) { replace = i; min_t = -1; break; }
                if (last[i] < min_t) { min_t = last[i]; replace = i; }
            }
            frames[replace] = pg;
            last[replace]   = t;
            faults++;
            print_frame(frames, nf, pg, 1);
        }
    }
    return faults;
}

void run_memory_demo(void)
{
    printf("\n==========  MODULE 5 : MEMORY MANAGEMENT (FIFO vs LRU)  ==========\n");
    
    init_memory_footprints();

    int pages[100];
    int np = 0;
    
    for (int i = 0; i < num_footprints; i++) {
        for (int j = 0; j < memory_footprints[i].num_pages; j++) {
            pages[np++] = memory_footprints[i].page_ids[j];
        }
        if (memory_footprints[i].num_pages >= 2) {
            pages[np++] = memory_footprints[i].page_ids[0];
            pages[np++] = memory_footprints[i].page_ids[memory_footprints[i].num_pages - 1];
        }
    }
    
    int extra[] = {0, 3, 6, 9, 12, 15, 0, 3, 6, 1, 4, 7, 10, 13, 16, 2, 5, 8, 11, 14, 17};
    for (int i = 0; i < 21 && np < 100; i++)
        pages[np++] = extra[i];
    
    int nf = NUM_FRAMES;

    printf("\nCustomer Memory Footprints:\n");
    printf("  Zain (Regular)   : Pages 0-2\n");
    printf("  Sara (Premium)   : Pages 3-5\n");
    printf("  Hamza (Loan-App) : Pages 6-9\n");
    printf("  Fatima (Corp)    : Pages 10-14\n");
    printf("  Ali (VIP)        : Pages 15-17\n");
    printf("\nNumber of frames: %d\n", nf);

    int ff = fifo(pages, np, nf);
    int lf = lru(pages, np, nf);

    printf("\n+-----------+--------------+-----------+-----------+------------------+\n");
    printf("| Algorithm | Page Faults  |   Hits    | Hit Ratio | Performance       |\n");
    printf("+-----------+--------------+-----------+-----------+------------------+\n");
    printf("|   FIFO    |     %2d       |    %2d     |   %3.0f%%   |                  |\n",
           ff, np - ff, 100.0 * (np - ff) / np);
    printf("|   LRU     |     %2d       |    %2d     |   %3.0f%%   | %s              |\n",
           lf, np - lf, 100.0 * (np - lf) / np,
           (lf < ff) ? "BETTER" : (ff < lf) ? "WORSE" : "EQUAL");
    printf("+-----------+--------------+-----------+-----------+------------------+\n");

    printf("\nConclusion: ");
    if (lf < ff)       
        printf("LRU has fewer page faults (%d vs %d)\n", lf, ff);
    else if (ff < lf)  
        printf("FIFO has fewer page faults (%d vs %d)\n", ff, lf);
    else               
        printf("FIFO and LRU produce equal page faults\n");
}