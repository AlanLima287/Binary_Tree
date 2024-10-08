#pragma once

#include "base.h"

typedef int32_t key_t;
static const uint64_t NAME_SIZE = 1000;

typedef struct Record {
   key_t key;
   int   data;
   char  name[NAME_SIZE + 1];
} Record;

void write_record(Record*, int, const char*);
void print_record(Record*, const char* = "{$0, $1, $2}");

void write_record(Record* record, int data, const char* name) {

   for (uint64_t i = 0; name[i]; i++) {

      if (i >= NAME_SIZE) {
         record->name[i] = '\0';
         break;
      }

      record->name[i] = name[i];
   }

   record->data = data;
}

void print_record(Record* record, const char* format) {

   while (*format) {

      if (*format == '$') {

         format++;
         switch (*format) {

            case '0': std::cout << record->key; break;
            case '1': std::cout << record->data; break;
            case '2': std::cout << record->name; break;

            default:
               std::cout.put(*format);
               break;
         }

      } else std::cout.put(*format);
      format++;
   }
}

void destruct_record(Record* record) {
   delete record;
   record = nullptr;
}
