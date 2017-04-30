////////////////////////////////////////////
//                                        //
//        LRU replacement policy          //
//     Jinchun Kim, cienlux@tamu.edu      //
//                                        //
////////////////////////////////////////////

#include "../inc/champsim_crc2.h"

#define NUM_CORE 1
#define LLC_SETS NUM_CORE*2048
#define LLC_WAYS 16
// Global Variables////////////////////////////////////////////////////////////
uint32_t table[LLC_SETS][LLC_WAYS];
int rfo = 0;      // request for access
int b = 0;
int wbck = 0;     // Write back
int d = 0;
int miss = 0;
int tot_hit = 0;  // Total hits
int g = 0;
int all_ups = 0;  // All policy state updates
// initialize replacement state
void InitReplacementState()
{
    cout << "Initialize LRU replacement state" << endl;

    // LRU
    for (int i=0; i<LLC_SETS; i++) {
        for (int j=0; j<LLC_WAYS; j++) {
            table[i][j] = j;
        }
    }
    
    // LFU = least frequently used = starts counting from 0
    // for (int i=0; i<LLC_SETS; i++) {
    //     for (int j=0; j<LLC_WAYS; j++) {
    //         table[i][j] = 0;
    //     }
    // }
}

// find replacement victim
// return value should be 0 ~ 15 or 16 (bypass)
uint32_t GetVictimInSet (uint32_t cpu, uint32_t set, const BLOCK *current_set, uint64_t PC, uint64_t paddr, uint32_t type)
{
    // LRU - Return least recently hit way (it'll be == to LLC_WAYS)
    // for (int i=0; i<LLC_WAYS; i++)
    //     if (table[set][i] == (LLC_WAYS-1))
    //         return i;
    
    // LFU - Return the least frequently hit item (lowest value)
    // uint32_t min = 0;
    // uint8_t  max_index = 0;
    // for (int i=0; i<LLC_WAYS; i++)
    //     if (table[set][i] <= min)
    //         min = table[set][i];

    switch (type) {
      case 0:
      break;
      case 1:
      rfo++;
      break;
      case 2:
      b++;
      break;
      case 3:
      wbck++;
      break;
      case 4:
      d++;
      break;
    }
    miss++;
    return 0; // The returned number is which block to evict
}

// called on every cache hit and cache fill
// hit = 1 means there was a hit
// cache fill = all-hits
void UpdateReplacementState (uint32_t cpu, uint32_t set, uint32_t way, uint64_t paddr, uint64_t PC, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    // update lru replacement state
    for (uint32_t i=0; i<LLC_WAYS; i++) {
        if (table[set][i] < table[set][way]) {
            table[set][i]++;
            if (table[set][i] == LLC_WAYS)
                assert(0);
        }
    }
    table[set][way] = 0; // promote to the MRU position
    switch (type) {
      case 0:
      
      break;
      case 1:
      rfo++;
      break;
      case 2:
      b++;
      break;
      case 3:
      wbck++;
      break;
      case 4:
      d++;
      break;
    }
    if (hit) {
      tot_hit++;
    }
      all_ups++;
}

// use this function to print out your own stats on every heartbeat 
void PrintStats_Heartbeat()
{

}

// use this function to print out your own stats at the end of simulation
void PrintStats()
{
  std::cout << "rfo: " << rfo << '\n';
  std::cout << "b: " << b << '\n';
  std::cout << "wbck: " << wbck << '\n';
  std::cout << "d: " << d << '\n';
  std::cout << "miss: " << miss << '\n';
  std::cout << "tot_hit: " << tot_hit << '\n';
  std::cout << "g: " << g << '\n';
  // std::cout << "all_ups: " << all_ups << '\n';  
}
