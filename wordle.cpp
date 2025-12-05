#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
    // in -> word being constructed
    // index -> current position in the word being constructed
    // floating -> letters that must be included but not yet placed
    // dict -> dictionary of valid words
    // results -> set of valid words found

  // generate words to fill in blanks
  void prune(
      std::string& in,
      size_t index,
      std::string floating,
      const std::set<std::string>& dict,
      std::set<std::string>& results);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;

    std::string inCopy = in; // make editable copy of word

    prune(inCopy, 0, floating, dict, results);

    return results;


}

// Define any helper functions here
void prune(
    std::string& in,
    size_t index,
    std::string floating,
    const std::set<std::string>& dict,
    std::set<std::string>& results)
{
    // BASE CASE
    if(index == in.size()) {
        if(floating.empty() && dict.find(in) != dict.end()) {
            results.insert(in);
        }
        return;
    }

    // FIXED CHARACTER -> skip
    if(in[index] != '-') {
        prune(in, index + 1, floating, dict, results);
        return;
    }

    // count blanks left
    size_t blanksLeft = 0;
    for(size_t i = index; i < in.size(); i++) {
        if(in[i] == '-') blanksLeft++;
    }

    // prune impossible case
    if(floating.size() > blanksLeft) return;

    // CASE 1: look through floating letters (permutation)
    if(floating.size() == blanksLeft) {

        for(size_t i = 0; i < floating.size(); i++) {
            char c = floating[i];
            in[index] = c;

            std::string nextFloating = floating;
            nextFloating.erase(i, 1);

            prune(in, index + 1, nextFloating, dict, results);
        }
        in[index] = '-';
        return;
    }

    // CASE 2: try all letters a-z
    size_t futureBlanks = blanksLeft - 1;
    for(char c = 'a'; c <= 'z'; c++) {
        in[index] = c;

        // remove 1 floating occurrence of c
        std::string nextFloating = floating;

        bool removed = false;

        // match floating letter
        for(size_t i = 0; i < nextFloating.size(); i++) {
            if(!removed && nextFloating[i] == c) {
                nextFloating.erase(i, 1);
                removed = true;
            }
        }

        // prune: floating must fit future blanks
        if(nextFloating.size() > futureBlanks) {
            continue;
        }

        prune(in, index + 1, nextFloating, dict, results);
    }

    // restore
    in[index] = '-';
}