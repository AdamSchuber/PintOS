#include <stdlib.h>
#include "pagedir.h"
#include "thread.h"

/* verfy_*_lenght are intended to be used in a system call that accept
 * parameters containing suspisious (user mode) adresses. The
 * operating system (executng the system call in kernel mode) must not
 * be fooled into using (reading or writing) addresses not available
 * to the user mode process performing the system call.
 *
 * In pagedir.h you can find some supporting functions that will help
 * you dermining if a logic address can be translated into a physical
 * addrerss using the process pagetable. A single translation is
 * costly. Work out a way to perform as few translations as
 * possible.
 *
 * Recommended compilation command:
 *
 *  gcc -Wall -Wextra -std=gnu99 -pedantic -m32 -g pagedir.o verify_adr.c
 */
#error Read comment above and then remove this line.

// void *pg_round_down(const void* adr);
// Returnerar första adressen i samma sida som adr.

// unsigned pg_no(const void* adr);
// Returnerar numret på sidan som innehåller adr. Sidnummer börjar räkna på 0, som allt annat i C.

// void *pagedir_get_page (void *pd, const void *adr);
// Använder översättningstabellen pd för att slå upp fysisk adress motsvarande adr. Om översättningen misslyckas returneras NULL. Översättningstabellen finns i struct thread, och heter pagedir.
// Notera att denna funktion är relativt dyr. Du vill alltså se till att inte anropa den fler gånger än vad du
// faktiskt behöver! I testprogrammet tar varje anrop till pagedir_get_page några hundra millisekunder, så
// om testerna tar lång tid att köra anropar du antagligen funktionen för många gånger!

// bool is_end_of_string(char* adr);
// Returnerar true om adressen adr innehåller ett noll-tecken, '\0'. Eftersom koden endast simulerar systemet
// går inga adresser att läsa eller skriva. Därför måste du använda denna funktion för att avgöra om en sträng
// är slut. Funktionen ersätter alltså testet *adr == '\0'.

/* Verify all addresses from and including 'start' up to but excluding
 * (start+length). */
bool verify_fix_length(void* start, unsigned length)
{
  

  if (length > PGSIZE)
    return false;

  
}

/* Verify all addresses from and including 'start' up to and including
 * the address first containg a null-character ('\0'). (The way
 * C-strings are stored.)
 */
bool verify_variable_length(char* start)
{
  // ADD YOUR CODE HERE
}

/* Definition of test cases. */
struct test_case_t
{
  void* start;
  unsigned length;
};

#define TEST_CASE_COUNT 6

const struct test_case_t test_case[TEST_CASE_COUNT] =
{
  {(void*)100, 100}, /* one full page */
  {(void*)199, 102},
  {(void*)101, 98},
  {(void*)250, 190},
  {(void*)250, 200},
  {(void*)250, 210}
};

/* This main program will evalutate your solution. */
int main(int argc, char* argv[])
{
  int i;
  bool result;

  if ( argc == 2 )
  {
    simulator_set_pagefault_time( atoi(argv[1]) );
  }
  thread_init();

  /* Test the algorithm with a given intervall (a buffer). */
  for (i = 0; i < TEST_CASE_COUNT; ++i)
  {
    start_evaluate_algorithm(test_case[i].start, test_case[i].length);
    result = verify_fix_length(test_case[i].start, test_case[i].length);
    evaluate(result);
    end_evaluate_algorithm();
  }

  /* Test the algorithm with a C-string (start address with
   * terminating null-character).
   */
  for (i = 0; i < TEST_CASE_COUNT; ++i)
  {
    start_evaluate_algorithm(test_case[i].start, test_case[i].length);
    result = verify_variable_length(test_case[i].start);
    evaluate(result);
    end_evaluate_algorithm();
  }
  return 0;
}
