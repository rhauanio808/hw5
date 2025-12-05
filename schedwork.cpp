#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

  // Recursive backtracking helper:
  //   day        - current day we are filling
  //   slot       - index (0..dailyNeed-1) within that day
  //   avail      - availability matrix (n x k)
  //   dailyNeed  - number of workers needed per day
  //   maxShifts  - max shifts per worker over all days
  //   sched      - schedule being built (n x dailyNeed)
  //   shiftsUsed - shiftsUsed[w] = how many shifts worker w has
  bool backtrackHelper(
      size_t day,
      size_t slot,
      const AvailabilityMatrix& avail,
      const size_t dailyNeed,
      const size_t maxShifts,
      DailySchedule& sched,
      std::vector<size_t>& shiftsUsed
  );


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
      const size_t nDays = avail.size();        // number of days
      const size_t nWorkers = avail[0].size();  // number of workers

      // edge cases
      if (nWorkers == 0 || dailyNeed == 0 || dailyNeed > nWorkers) {
          return false;
      }

      // initialize schedule: 
      sched.assign(nDays, std::vector<Worker_T>(dailyNeed, INVALID_ID));

      // rack how many shifts each worker has been assigned
      std::vector<size_t> shiftsUsed(nWorkers, 0);

      // start backtracking from day 0, slot 0
      bool validSchedule = backtrackHelper(0, 0, avail, dailyNeed, maxShifts, sched, shiftsUsed);

      // if no solution, sched can be anything per spec; we already cleared/assigned it
      return validSchedule;
}

// backtrack helper
bool backtrackHelper(
    size_t day,
    size_t slot,
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsUsed
)
{
    const size_t nDays = avail.size();         // number of days
    const size_t nWorkers = avail[0].size();   // number of workers

    // if all days r filled -> complete valid schedule
    if (day == nDays) {
        return true;
    }

    // If we filled all slots for this day, move to next day
    if (slot == dailyNeed) {
        return backtrackHelper(day + 1, 0, avail, dailyNeed, maxShifts, sched, shiftsUsed);
    }

    // Try assigning each worker to (day, slot)
    for (size_t i = 0; i < nWorkers; ++i) {

        // worker must be available on this day
        if (!avail[day][i]) {
            continue;
        }

        // worker must not exceed max shifts
        if (shiftsUsed[i] >= maxShifts) {
            continue;
        }

        // worker cannot already be assigned earlier on the same day
        if (std::find(sched[day].begin(),
                      sched[day].begin() + slot,
                      static_cast<Worker_T>(i)) != sched[day].begin() + slot) {
            continue;
        }

        // assign this worker to the current slot
        sched[day][slot] = static_cast<Worker_T>(i);
        shiftsUsed[i]++;

        // recurse to fill next slot/day
        if (backtrackHelper(day, slot + 1, avail, dailyNeed, maxShifts, sched, shiftsUsed)) {
            return true;  // found a valid complete schedule
        }

        // undo assignment
        shiftsUsed[i]--;
        sched[day][slot] = INVALID_ID;
    }

    // no worker could be placed in this slot 
    return false;
}

