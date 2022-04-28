#include "wrap/thread.h"
#include "wrap/synch.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Denna struktur representerar innehållet i vår (stora) datafil.
 */
struct data_file {
  // Hur många gånger har filen blivit öppnad?
  int open_count;

  struct lock file_lock;

  // Filens ID.
  int id;

  // Data i filen.
  char *data;
};

// Håll koll på den fil vi har öppnat. Om ingen fil är öppen är denna variabel NULL.
// Tänk er att detta är en array av två pekare, dvs. struct data_file *open_files[2];
struct data_file **open_files;
struct lock lock;

// Initiera de datastrukturer vi behöver. Anropas en gång i början.
void data_init(void) NO_STEP {
  open_files = malloc(sizeof(struct data_file *)*2);
  lock_init(&lock);
}

// Öppna datafilen med nummer "file" och se till att den finns i RAM. Om den
// redan råkar vara öppnad ger funktionen tillbaka en pekare till instansen som
// redan var öppen. Annars laddas filen in i RAM.
struct data_file *data_open(int file) {
  lock_acquire(&lock);
  struct data_file *result = open_files[file];
  if (result == NULL) {
    // Skapa en ny data_file.
    result = malloc(sizeof(struct data_file));
    result->open_count = 0;
    result->id = file;

    lock_init(&result->file_lock);

    // Simulera att vi läser in data...
    timer_msleep(100);
    if (file == 0)
      result->data = strdup("File 0");
    else
      result->data = strdup("File 1");

    // Spara data i "open_files".
    open_files[file] = result;
  }
  lock_release(&lock);

  lock_acquire(&result->file_lock);
  result->open_count++;
  lock_release(&result->file_lock);

  return result;
}

// Stäng en datafil. Om ingen annan har filen öppen ska filen avallokeras för
// att spara minne.
void data_close(struct data_file *file) {
  lock_acquire(&file->file_lock);
  int open_count = --file->open_count;
  lock_release(&file->file_lock);
  
  if (open_count <= 0) {
    // Ingen har filen öppen längre. Då kan vi ta bort den!
    lock_acquire(&lock);
    open_files[file->id] = NULL;
    free(file->data);
    free(file);
    lock_release(&lock);
  }
}


/**
 * Testprogram.
 *
 * Med en korrekt implementation av funktionerna ovan ska du inte behöva ändra i
 * dessa funktioner. Det kan dock vara intressant att modifiera koden nedan för
 * att kunna testa bättre.
 */


void thread_main(int *file_id) {
  struct data_file *f = data_open(*file_id);
  printf("Data: %s\n", f->data);
  data_close(f);
}

int main(void) {
  data_init();

  int zero = 0;
  int one = 1;
  thread_new(&thread_main, &zero);
  thread_new(&thread_main, &one);

  thread_main(&zero);

  return 0;
}
