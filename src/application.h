#pragma once

#include "base.h"
#include "system.h"

#include "../rc/strings.cpp"

#include "../lib/terminal.h"
#include "../lib/color.h"

#include "linked_list.h"
#include "tree.h"
#include "file.h"

// Just Another Shell for testing sTructures
namespace JAST {

   enum kind_t : byte { LINKED_LIST, TREE, AVL_TREE };
   enum command_t : byte { _chfocus, _create, _help, _init, _load, _new, _print, _quit, _save, _search, _test };
   const byte NOT_FOUND = -1;

   typedef long id_t;

   typedef struct StructStack {
      id_t id;
      kind_t kind;

      union {
         LinkedList::Node* linked_list;
         Tree::Node* tree;
      } node;

      StructStack* next_node;
   } StructStack;

   template <uint64_t str_size>
   struct Map {
      const char alias[str_size];
      byte command;
   };

   static struct State {
      StructStack* stack;
      StructStack* focused;
      word width, height;
   } state = { nullptr, nullptr, 0, 0 };

   Map<8> COMMANDS[] = {
      "chfocus", _chfocus,
      "create",  _create,
      "exit",    _quit,
      "help",    _help,
      "init",    _init,
      "load",    _load,
      "new",     _new,
      "print",   _print,
      "quit",    _quit,
      "save",    _save,
      "search",  _search,
      "test",    _test,
   };

   Map<12> STRUCTURES[] = {
      "avl", AVL_TREE,
      "avl_tree", AVL_TREE,
      "linked_list", LINKED_LIST,
      "ll", LINKED_LIST,
      "tree", TREE,
   };

   const uint64_t COMMAND_LIST_SIZE = sizeof(COMMANDS) / sizeof(COMMANDS[0]);
   const uint64_t STRUCTURE_LIST_SIZE = sizeof(STRUCTURES) / sizeof(STRUCTURES[0]);
   
   // Functionalities

   uint64_t process_args(char* prompt) {

      char* cursor = prompt;
      bool read = false;

      uint64_t argc = 0;
      uint64_t i;

      for (i = 0; prompt[i] == ' '; i++)
         if (!prompt[i]) return 0;

   normal:
      switch (prompt[i]) {
         case '\0': *cursor = '\0'; if (read) argc++; return argc;
         case '"': read = false; i++; goto quoted;
         case ' ': if (read) argc++; read = false; *cursor++ = '\0'; i++; goto space;
         default: read = true; *cursor++ = prompt[i++]; goto normal;
      }

   space:
      switch (prompt[i]) {
         case '\0': return argc;
         case ' ': i++; goto space;
         default: goto normal;
      }

   quoted:
      switch (prompt[i]) {
         case '\0': *cursor = '\0'; if (read) argc++; return argc;
         case '"': if (!read) cursor--; i++; goto normal;
         default: read = true; *cursor++ = prompt[i++]; goto quoted;
      }
   }

   StructStack* find_structure(id_t id) {
      for (StructStack* s = state.stack; s; s = s->next_node)
         if (id == s->id) return s;

      return nullptr;
   }

   template <uint64_t str_size>
   byte access_map(Map<str_size>*, uint64_t, char*&);
   bool match_consume_word(const char*, char*&);
   uint64_t string_to_int_consume(char*&);


   uint64_t print_in_column(const char*, word, word);

   // Commands

   void init();
   void __cmd_help(uint64_t, char*);

   exit_t __cmd_create(uint64_t, char*);
   exit_t __cmd_load(uint64_t, char*);
   exit_t __cmd_new(uint64_t, char*);
   exit_t __cmd_save(uint64_t, char*);
   
   void __cmd_chfocus(uint64_t, char*);
   void __cmd_print(uint64_t, char*);
   void __cmd_search(uint64_t, char*);
   void __cmd_test(uint64_t, char*);

   // The interpreter
   exit_t interpreter(char* prompt);
};