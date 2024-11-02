#include "application.h"

void JAST::cmd_help(uint64_t argc, char* prompt) {

   get_terminal_dimensions(state.width, state.height);

   byte command = command_t::_help;
   if (argc) command = access_map(COMMANDS, COMMAND_LIST_SIZE, prompt);

   switch (command) {

      case command_t::_help: {
         std::cout.write(
            "Lista de todos os comandos:\n\n"
            "   chfocus Muda a estrutura em foco\n"
            "   create  Cria um registro e o insere numa estrutura\n"
            "   exit    Mesmo que `quit`\n"
            "   help    Mostra essa tela\n"
            "   init    Mostra a tela de entrada\n"
            "   load    Carrega dados de um arquivo para alguma estrutura\n"
            "   new     Cria uma nova estrutura\n"
            "   print   Imprime estruturas\n"
            "   quit    Encerra a aplicação\n"
            "   save    Salva os dados de uma estrutura para algum arquivo\n"
            "   test    Realisa testes de busca na estrutura em foco\n\n"
            "Digite `help <comando>` para obter mais informações sobre um comando\n"
            , 560);
         break;
      }

      case command_t::_chfocus: {
         std::cout.write(
            "chfocus $<id>  - Foca a estrutura de id <id>\n"
            "chfocus remove - Remove o foco de estrutrura\n"
            "chfocus        - Mostra a estrutrura em foco\n"
            , 135);
         break;
      }

      case command_t::_create: {
         std::cout.write(
            "create $<id> <key> <data> <name> - Cria um registro e insere na estrutura de id <id>\n"
            "create $<id> random              - Cria um registro aleatório e insere na estrutura de id <id>\n"
            "create <key> <data> <name>       - Cria um registro e insere na estrutrura em foco\n"
            "create random                    - Cria um registro aleatório e insere na estrutura em foco\n"
            , 357);
         break;
      }

      case command_t::_init: {
         std::cout.write(
            "init - Mostra a tela de entrada\n"
            , 0);
         break;
      }

      case command_t::_load: {
         std::cout.write(
            "load <struct> <filename> - Carrega os registros de <filename> para a estrutura em foco\n"
            "   <struct>: tree; avl ou avl_tree; linked_list ou ll\n"
            , 141);
         break;
      }

      case command_t::_new: {
         std::cout.write(
            "new <struct>                        - Cria uma nova estrutura vazia\n"
            "new <struct> <count> <rule>         - Cria uma nova estrutura com <count> registros aleatórios\n"
            "new <struct> <count> <rule> <order> - Cria uma nova estrutura com <count> registros aleatórios,\n"
            "                                      seguindo uma regra <rule> e chaves em [0, <count> * <order>)\n"
            "   <struct>: tree; avl ou avl_tree; linked_list ou ll\n"
            "   <rule>: ordered, random, inversed\n"
            , 451);
         break;
      }

      case command_t::_print: {
         print_in_column(
            "print               - Imprime a estrutura em foco, se nenhuma estrutura em foco, imprime o primeiro registro de todas estruturas e seus id's\n"
            "print <limit>       - Imprime a estrutura em foco, se existir, limitando a profundidade em <limit>\n"
            "print $<id>         - Imprime a estrutura de id <id>, se existir\n"
            "print $<id> <limit> - Imprime a estrutura de id <id>, se existir, limitando a profundidade em <limit>\n"
            "print all           - imprime o primeiro registro de todas estruturas e seus id's\n"
            , 0, state.width);
         break;
      }

      case command_t::_quit: {
         std::cout.write(
            "exit - Encerra a aplicação\n"
            "quit - Encerra a aplicação\n"
            , 58);
         break;
      }

      case command_t::_save: {
         std::cout.write(
            "save <filename>       - Salva os registros da estrutura em foco no arquivo <filename>\n"
            "save $<id> <filename> - Salva os registros da estrutura de id <id> no arquivo <filename>\n"
            , 175);
         break;
      }

      case command_t::_test: {
         uint64_t width = 25;

         print_in_column("test $<id> <sample_size>", 0, state.width);
         print_in_column("Realiza testes <sample_size> de busca na estrutura de id <id>\n", width, state.width);

         print_in_column("test $<id> <sample_size>", 0, state.width);
         print_in_column("test $<id> <sample_size> - Realiza testes <sample_size> de busca na estrutura em foco\n", width, state.width);
         
         break;
      }

      default: {
         std::cout.write(COMMAND_NOT_FOUND, sizeof(COMMAND_NOT_FOUND) - 1);
         break;
      }
   }
}

void JAST::init() {
   get_terminal_dimensions(state.width, state.height);

   esc::clear();
   esc::move_to(0, 0);

   for (uint64_t i = 0; i < state.width; i++)
      std::cout.put('-');
   std::cout.put('\n');

   esc::color(esc::FOREGROUND, 0xF1E6B8);
   std::cout.write(LITTLE_CHICK, sizeof(LITTLE_CHICK) - 1);
   const uint64_t little_chick_width = 31;

   esc::move_to(1, 0);
   esc::reset();

   uint64_t lines = 0;

   lines += print_in_column(ABOUT, little_chick_width, state.width - little_chick_width);
   lines += print_in_column(AUTHORS, little_chick_width, state.width - little_chick_width);

   esc::italic();
   lines += print_in_column(AUTHORS_NAMES, little_chick_width, state.width - little_chick_width);

   esc::reset();
   esc::move_to(2 + maximum(lines, 14 /* little_chick_height */), 0);
}

exit_t JAST::interpreter(char* prompt) {

   uint64_t argc = process_args(prompt);
   // std::cout << argc << std::endl;

   if (!argc) return exit_t::SUCCESS;

   // char* c = prompt;
   // for (uint64_t i = 0; i < argc; c++) {
   //    if (!*c) { std::cout.put('#'); i++; }
   //    else std::cout.put(*c);
   // }

   // std::cout.put('\n');

   switch (access_map(COMMANDS, COMMAND_LIST_SIZE, prompt)) {

      case command_t::_chfocus: cmd_chfocus(argc - 1, prompt); break;
      case command_t::_create: return cmd_create(argc - 1, prompt);

      case command_t::_init: init(); break;

      case command_t::_help: cmd_help(argc - 1, prompt); break;
      case command_t::_new: return cmd_new(argc - 1, prompt);
      case command_t::_load: return cmd_load(argc - 1, prompt);

      case command_t::_print: cmd_print(argc - 1, prompt); break;

      case command_t::_quit:

         while (true) {
            std::cout.write(CONFIRM_EXIT, sizeof(CONFIRM_EXIT) - 1);

            char answer = std::cin.get();
            std::cin.ignore(64, '\n');

            if (answer == 's') return exit_t::EXIT_APPLICATION;
            if (answer == 'n') break;
         }

         break;

      case command_t::_save: return cmd_save(argc - 1, prompt);
      case command_t::_test: cmd_test(argc - 1, prompt); break;

      case NOT_FOUND:
         std::cout.write(COMMAND_NOT_FOUND, sizeof(COMMAND_NOT_FOUND) - 1);
         break;
   }

   return exit_t::SUCCESS;
}

void JAST::cmd_test(uint64_t argc, char* prompt) {

   // Identifies whether the supplied args are enough
   if (!argc || (prompt[0] == '$' && argc == 1)) {
      std::cout.write(NOT_ENOUGH_ARGS, sizeof(NOT_ENOUGH_ARGS) - 1);
      return;
   }

   StructStack* structure;

   if (prompt[0] == '$') {

      // Tries to load an id and find its structure
      structure = find_structure(__aut_str_to_int(++prompt));

      // If not found, quits
      if (!structure) {
         std::cout.write(STRUCT_NOT_FOUND, sizeof(STRUCT_NOT_FOUND) - 1);
         return;
      }

   // If an id was not supplies verifies whether there's a focused structure
   } else if (!state.focused) {

      std::cout.write(NO_STRUCT_FOCUS, sizeof(NO_STRUCT_FOCUS) - 1);
      return;

   } else structure = state.focused;

   // Load the sample size
   uint64_t sample_size = __aut_str_to_int(prompt);

   struct {
      struct {
         uint64_t count, span;
      } in, out;
   } sample;

   sample.out.span = 0;
   sample.in.span = 0;

   sample.out.count = 0;
   sample.in.count = 0;

   switch (structure->kind) {

      case kind_t::LINKED_LIST: {

         // Store the min and max keys, will use them to generate key in the
         // range of the tree
         Record::key_t min_key = structure->node.linked_list->content->key;
         Record::key_t max_key = structure->node.linked_list->content->key;

         if (!structure->node.linked_list) {
            std::cout.write("Não é possivel realizar testes em estruturas vazias!\n", 55);
            return;
         }

         LinkedList::Node* current = structure->node.linked_list->next_node;
         while (current) {

            if (false);
            else if (current->content->key < min_key)
               min_key = current->content->key;
            else if (current->content->key > max_key)
               max_key = current->content->key;

            current = current->next_node;
         }

         // The ranged random function is upper exclusive
         max_key++;

         // Execute a limited hard-fixed amount of times, to avoid undecidability
         for (
            uint64_t iteration_limit = ITERATION_LIMIT * sample_size;
            sample.in.count < sample_size || sample.out.count < sample_size;
            iteration_limit--) {

            if (iteration_limit <= 0) {
               std::cout.write("Não foram encontradas chaves de teste suficientes!\n", 53);
               return;
            }

            // Generates a random key in range, doesn't actully check if it
            // hasn't already been generated
            Record::key_t key = Random::rand(min_key, max_key);
            Record* record;

            // Mark the start
            auto start = std::chrono::high_resolution_clock::now();

            // Search for the key
            record = LinkedList::search(structure->node.linked_list, key);

            // Mark the end
            auto end = std::chrono::high_resolution_clock::now();

            // If the key has been found, count as in, if not, count as out
            if (record) {
               if (sample.in.count < sample_size) {
                  sample.in.span += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                  sample.in.count++;
               }
            } else {
               if (sample.out.count < sample_size) {
                  sample.out.span += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                  sample.out.count++;
               }
            }
         }

      } break;

      case kind_t::AVL_TREE:
      case kind_t::TREE: {

         Tree::Node* current = structure->node.tree;
         Record::key_t min_key, max_key;

         if (current) {
            while (current->left_child)
               current = current->left_child;
            min_key = current->content->key;

            current = structure->node.tree;

            while (current->rght_child)
               current = current->rght_child;

            // The ranged random function is upper exclusive
            max_key = current->content->key + 1;
         } else {
            std::cout.write("Não é possivel realizar testes em estruturas vazias!\n", 55);
            return;
         }

         // Execute a limited hard-fixed amount of times, to avoid undecidability
         for (
            uint64_t iteration_limit = ITERATION_LIMIT * sample_size;
            sample.in.count < sample_size || sample.out.count < sample_size;
            iteration_limit--) {

            if (iteration_limit <= 0) {
               std::cout.write("Não foram encontradas chaves de teste suficientes!\n", 53);
               return;
            }

            // Generates a random key in range, doesn't actully check if it
            // hasn't already been generated
            Record::key_t key = Random::rand(min_key, max_key);
            Record* record;

            // Mark the start
            auto start = std::chrono::high_resolution_clock::now();

            // Search for the key
            record = Tree::search(structure->node.tree, key);

            // Mark the end
            auto end = std::chrono::high_resolution_clock::now();

            // If the key has been found, count as in, if not, count as out
            if (record) {
               if (sample.in.count < sample_size) {
                  sample.in.span += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                  sample.in.count++;
               }
            } else {
               if (sample.out.count < sample_size) {
                  sample.out.span += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                  sample.out.count++;
               }
            }
         }

      } break;
   }

   std::cout << "Teste de " << sample_size << (sample_size == 1 ? " chave" : " chaves");

   // Present keys
   std::cout.write("\nChaves presentes:", 18);
   std::cout << "\n   Tempo médio: " << ((double)sample.in.span / (double)sample_size) << "ms";

   // Non-existing keys
   std::cout.write("\nChaves inexistentes:", 21);
   std::cout << "\n   Tempo médio: " << ((double)sample.out.span / (double)sample_size) << "ms";

   std::cout.put('\n');
}

void JAST::cmd_chfocus(uint64_t argc, char* prompt) {
   if (!argc) {

      if (!state.focused) {
         std::cout.write(NO_STRUCT_FOCUS, sizeof(NO_STRUCT_FOCUS) - 1);
         return;
      }

      if (state.focused->kind == kind_t::LINKED_LIST) {
         std::cout.write(LL_IDENTIFIER, sizeof(LL_IDENTIFIER) - 1);
         std::cout.put('\n');

         LinkedList::print(state.focused->node.linked_list);

      } else {
         if (state.focused->kind == kind_t::TREE) std::cout.write(TREE_IDENTIFIER, sizeof(TREE_IDENTIFIER) - 1);
         else std::cout.write(AVL_TREE_IDENTIFIER, sizeof(AVL_TREE_IDENTIFIER) - 1);
         std::cout.put('\n');

         Tree::print(state.focused->node.tree);
      }

      return;
   }

   if (prompt[0] == '$') {
      state.focused = find_structure(__aut_str_to_int(++prompt));
      if (!state.focused) {
         std::cout.write(STRUCT_NOT_FOUND, sizeof(STRUCT_NOT_FOUND) - 1);
      }

      return;
   }

   if (__aut_match_word("remove", prompt)) {
      state.focused = nullptr;
      return;
   }

   std::cout.write(INVALID_INPUT, sizeof(INVALID_INPUT) - 1);
}

exit_t JAST::cmd_create(uint64_t argc, char* prompt) {

   if (argc) {

      StructStack* structure;

      if (prompt[0] == '$') {
         structure = find_structure(__aut_str_to_int(++prompt));
         argc--;

         if (!structure) {
            std::cout.write(STRUCT_NOT_FOUND, sizeof(STRUCT_NOT_FOUND) - 1);
            return exit_t::SUCCESS;
         }

      } else if (!state.focused) {

         std::cout.write(NO_STRUCT_FOCUS, sizeof(NO_STRUCT_FOCUS) - 1);
         return exit_t::SUCCESS;

      } else structure = state.focused;

      Record* record = new (std::nothrow) Record;
      if (!record) return exit_t::BAD_ALLOCATION;

      // Check whether the record should be random
      if (!__aut_match_word("random", prompt)) {

         if (argc < 3) goto invalid_input;

         record->key = __aut_str_to_int(prompt);
         int data = __aut_str_to_int(prompt);
         write_record(record, data, prompt);

         exit_t response = exit_t::SUCCESS;

         switch (structure->kind) {
            case LINKED_LIST: response = LinkedList::insert(&structure->node.linked_list, record); break;
            case AVL_TREE: response = Tree::AVL::insert(&structure->node.tree, record); break;
            case TREE: response = Tree::insert(&structure->node.tree, record); break;
         }

         if (response == exit_t::BAD_ALLOCATION) {
            delete record;
            return exit_t::BAD_ALLOCATION;
         }

         if (response == exit_t::KEY_ALREADY_EXISTS) {
            esc::color(esc::BRIGHT_RED, esc::FOREGROUND);
            std::cout << "A chave " << record->key << " já existe!\n";
            esc::reset();

            delete record;
         }

         return exit_t::SUCCESS;
      }

      // Populate randomly and search for a valid key up to 64 times
      populate_record_randomly(record);

      // The loop must execute at least once
      exit_t response = exit_t::KEY_ALREADY_EXISTS;

      for (uint64_t i = 0; response == exit_t::KEY_ALREADY_EXISTS; i++) {

         if (i >= ITERATION_LIMIT) {
            esc::color(esc::BLUE, esc::FOREGROUND);
            std::cout.write("Uma nova chave única não foi encontrada!\n", 43);
            esc::reset();

            return exit_t::SUCCESS;
         }

         record->key = Record::key_t(Random::rand() & 0xFFFFF);

         switch (structure->kind) {
            case LINKED_LIST: response = LinkedList::insert(&structure->node.linked_list, record); break;
            case AVL_TREE: response = Tree::AVL::insert(&structure->node.tree, record); break;
            case TREE: response = Tree::insert(&structure->node.tree, record); break;
         }

         if (response == exit_t::BAD_ALLOCATION) {
            delete record;
            return exit_t::BAD_ALLOCATION;
         }
      }

      return exit_t::SUCCESS;
   }

invalid_input:
   std::cout.write(INVALID_INPUT, sizeof(INVALID_INPUT) - 1);
   return exit_t::SUCCESS;
}

exit_t JAST::cmd_load(uint64_t argc, char* prompt) {

   if (argc >= 2) {

      RecordFile::File rc_file;
      byte kind = access_map(STRUCTURES, STRUCTURE_LIST_SIZE, prompt);
      if (kind == NOT_FOUND) goto invalid_input;

      // Try to load the file
      RecordFile::exit_t response = RecordFile::open(prompt, &rc_file, RecordFile::READ);

      if (response == RecordFile::BAD) {
         std::cout.write(FILE_OPENING_ERROR, sizeof(FILE_OPENING_ERROR) - 1);
         return exit_t::SUCCESS;
      }

      if (response == RecordFile::EMPTY) {
         esc::color(esc::BLUE, esc::FOREGROUND);
         std::cout.write("O arquivo fornecido está vazio\n", 32);
         esc::reset();

         RecordFile::close(&rc_file);
         return exit_t::SUCCESS;
      }

      id_t next_id = state.stack ? state.stack->id + 1 : 0;
      StructStack* structure = new (std::nothrow) StructStack{ next_id, kind_t(kind), nullptr, state.stack };
      if (!structure) {
         RecordFile::close(&rc_file);
         return exit_t::BAD_ALLOCATION;
      }

      state.stack = structure;
      bool not_complete = true;

      while (not_complete) {
         Record* record = new (std::nothrow) Record;
         if (!record) {
            RecordFile::close(&rc_file);
            return exit_t::BAD_ALLOCATION;
         }

         not_complete = RecordFile::read(&rc_file, record);
         exit_t response = exit_t::SUCCESS;

         switch (structure->kind) {
            case LINKED_LIST: response = LinkedList::insert(&structure->node.linked_list, record); break;
            case AVL_TREE: response = Tree::AVL::insert(&structure->node.tree, record); break;
            case TREE: response = Tree::insert(&structure->node.tree, record); break;
         }

         if (response == exit_t::BAD_ALLOCATION) {
            delete record;
            return exit_t::BAD_ALLOCATION;
         }

         if (response == exit_t::KEY_ALREADY_EXISTS) {
            delete record;
         }
      }

      RecordFile::close(&rc_file);
      return exit_t::SUCCESS;
   }

invalid_input:
   std::cout.write(INVALID_INPUT, sizeof(INVALID_INPUT) - 1);
   return exit_t::SUCCESS;
}

exit_t JAST::cmd_new(uint64_t argc, char* prompt) {

   if (argc) {

      kind_t structure = (kind_t)access_map(STRUCTURES, STRUCTURE_LIST_SIZE, prompt);
      if (structure == NOT_FOUND) goto not_enough_args;

      id_t next_id = state.stack ? state.stack->id + 1 : 0;

      StructStack* new_structure = new (std::nothrow) StructStack{ next_id, structure, nullptr, state.stack };
      if (!new_structure) return exit_t::BAD_ALLOCATION;
      state.stack = new_structure;

      if (argc < 3) return exit_t::SUCCESS;

      uint64_t n_of_records = __aut_str_to_int(prompt);
      if (!n_of_records) return exit_t::SUCCESS;

      rule_t rule = (rule_t)access_map(RULES, RULE_LIST_SIZE, prompt);

      uint64_t order = (argc >= 4) ? __aut_str_to_int(prompt) : 1;
      if (order < 1) order = 1;

      uint64_t max_key = order * n_of_records;

      BitTools::mword* helper = BitTools::initialize(
         Stack::allocate<BitTools::mword>(BitTools::size(max_key)),
         max_key
      );

      uint64_t key = (rule == _inversed) ? max_key : 0;

      for (uint64_t i = 0; i < n_of_records; i++) {

         switch (rule) {

            case _random:
               key = uint64_t(Random::rand() % max_key);

               while (BitTools::getbit(helper, key))
                  if (++key >= n_of_records) key = 0;

               BitTools::flipbit(helper, key);
               break;

            case _ordered: key += order; break;
            case _inversed: key -= order; break;
         }

         Record* record = new (std::nothrow) Record{ Record::key_t(key) };
         if (!record) {
            Stack::release(helper);
            return exit_t::BAD_ALLOCATION;
         }

         populate_record_randomly(record);

         exit_t response;
         switch (state.stack->kind) {

            case LINKED_LIST:
               response = LinkedList::insert(&state.stack->node.linked_list, record);
               break;

            case AVL_TREE:
               response = Tree::AVL::insert(&state.stack->node.tree, record);
               break;

            case TREE:
               response = Tree::insert(&state.stack->node.tree, record);
               break;
         }

         if (response == exit_t::BAD_ALLOCATION) {
            Stack::release(helper);
            return exit_t::BAD_ALLOCATION;
         }
      }

      Stack::release(helper);
      return exit_t::SUCCESS;
   }

not_enough_args:
   std::cout.write(NOT_ENOUGH_ARGS, sizeof(NOT_ENOUGH_ARGS) - 1);
   return exit_t::SUCCESS;
}

void JAST::cmd_print(uint64_t argc, char* prompt) {

   StructStack* structure;
   int64_t level = -1;

   if (!argc) {

      if (!state.focused) goto print_all;

      structure = state.focused;
      goto print_specific;

   } else switch (prompt[0]) {

      case '$':
         structure = find_structure(__aut_str_to_int(++prompt));
         if (!structure) {
            std::cout.write(STRUCT_NOT_FOUND, sizeof(STRUCT_NOT_FOUND) - 1);
            return;
         } else {
            if (argc > 1) level = __aut_str_to_int(prompt);
            goto print_specific;
         }

      case 'a':
         if (__aut_match_word("all", prompt)) goto print_all;

      default:
         level = __aut_str_to_int(prompt);
         if (!state.focused) {
            std::cout.write(INVALID_INPUT, sizeof(INVALID_INPUT) - 1);
            return;
         }

         structure = state.focused;
         goto print_specific;
   }

print_specific:
   if (structure->kind == kind_t::LINKED_LIST) {
      std::cout.write(LL_IDENTIFIER, sizeof(LL_IDENTIFIER) - 1);
      std::cout.put('\n');

      LinkedList::print(structure->node.linked_list, level);

   } else {
      if (structure->kind == kind_t::TREE) std::cout.write(TREE_IDENTIFIER, sizeof(TREE_IDENTIFIER) - 1);
      else std::cout.write(AVL_TREE_IDENTIFIER, sizeof(AVL_TREE_IDENTIFIER) - 1);
      std::cout.put('\n');

      Tree::print(structure->node.tree, level);
   }

   return;

print_all:
   for (StructStack* s = state.stack; s; s = s->next_node) {

      Record* record;
      std::cout << '$' << s->id << " - ";

      if (s->kind == kind_t::LINKED_LIST) {
         std::cout.write(LL_IDENTIFIER, sizeof(LL_IDENTIFIER) - 1);
         record = safe_access(s->node.linked_list, content, nullptr);
      } else {
         if (s->kind == kind_t::TREE) std::cout.write(TREE_IDENTIFIER, sizeof(TREE_IDENTIFIER) - 1);
         else std::cout.write(AVL_TREE_IDENTIFIER, sizeof(AVL_TREE_IDENTIFIER) - 1);
         record = safe_access(s->node.tree, content, nullptr);
      }

      print_record(record, "{$0: $1, $2\n");
   }
}

exit_t JAST::cmd_save(uint64_t argc, char* prompt) {

   if (!argc && argc > 2) goto not_enough_args;

   RecordFile::File rc_file;
   StructStack* structure;

   // One args, so it's gonna be just a filename, verifies whether there's
   // a focused structure
   if (argc == 1) {

      if (!state.focused) {
         std::cout.write(NO_STRUCT_FOCUS, sizeof(NO_STRUCT_FOCUS) - 1);
         return exit_t::SUCCESS;
      }

      structure = state.focused;

   // Has two args, first one must be an id, second a filename
   } else if (prompt[0] == '$') {

      structure = find_structure(__aut_str_to_int(++prompt));
      if (!structure) {
         std::cout.write(STRUCT_NOT_FOUND, sizeof(STRUCT_NOT_FOUND) - 1);
         return exit_t::SUCCESS;
      }

   } else goto not_enough_args;

   // Try to load the file
   if (RecordFile::open(prompt, &rc_file, RecordFile::WRITE) == RecordFile::BAD) {
      std::cout.write(FILE_OPENING_ERROR, sizeof(FILE_OPENING_ERROR) - 1);
      return exit_t::SUCCESS;
   }

   switch (structure->kind) {

      case kind_t::TREE:
      case kind_t::AVL_TREE: {

         Tree::Node* node = structure->node.tree;

         if (!node) break;

         Tree::Node** stack = nullptr;
         if (node->height) {
            stack = Stack::allocate<Tree::Node*>(node->height);
            if (!stack) return exit_t::BAD_ALLOCATION;
         }

         int64_t top = -1;

         while (true) {

            if (node->left_child) stack[++top] = node->left_child;
            if (node->rght_child) stack[++top] = node->rght_child;

            RecordFile::write(&rc_file, node->content, top >= 0);

            if (top < 0) break;
            node = stack[top--];
         }

         Stack::release(stack);
      } break;

      case kind_t::LINKED_LIST: {

         LinkedList::Node* node = structure->node.linked_list;

         while (node) {
            RecordFile::write(&rc_file, node->content, bool(node->next_node));
            node = node->next_node;
         }

      } break;
   }

   RecordFile::close(&rc_file);
   return exit_t::SUCCESS;

not_enough_args:
   std::cout.write(NOT_ENOUGH_ARGS, sizeof(NOT_ENOUGH_ARGS) - 1);
   return exit_t::SUCCESS;
}

// match two strings (case sensitevely) and consume the second word if it matches
bool JAST::__aut_match_word(const char* compare, char*& consume) {

   for (uint64_t i = 0; compare[i] == consume[i]; i++) {
      if (compare[i]) continue;

      consume += i + 1;
      return true;
   }

   return false;
}

// parse a string containing a decimal integer representation while consuming the word
uint64_t JAST::__aut_str_to_int(char*& consume) {
   uint64_t num = 0;
   bool sign = false;

   if (*consume == '-') {
      sign = true;
      consume++;
   }

   while (*consume) {
      if ('0' <= *consume && *consume <= '9')
         num = 10 * num + *consume - '0';
      consume++;
   }

   consume++;

   if (sign) num = -num;
   return num;
}

template <uint64_t str_size>
byte JAST::access_map(Map<str_size>* map, uint64_t length, char*& prompt) {
   uint64_t index;

   for (index = 0; index < length && map[index].alias[0] <= prompt[0]; index++)
      if (__aut_match_word(map[index].alias, prompt)) return map[index].command;

   return NOT_FOUND;
}

uint64_t JAST::print_in_column(const char* text, word start, word width) {
   esc::move_to(start);

   uint64_t lines = 0, index = 0, length = 0;
   while (true) {

      switch (text[index]) {
         case '\0':
            if (index > 0) {
               std::cout.write(text, index);
               lines++;
            }

            return lines;

         case '\n':
            if (index > 0) {
               std::cout.write(text, ++index);
               text += index;

               length = 0;
               index = 0;
            }

            esc::move_to(start);
            lines++;

            continue;

         case ' ':
            length = index;
            break;
      }

      if (index >= width) {

         uint8_t inc = 0;

         if (!length) {
            length = width;
            index = 0;
         } else {
            index -= length;
            inc = 1;
         }

         std::cout.write(text, length);
         text += length + inc;
         length = 0;

         std::cout.put('\n');
         esc::move_to(start);
         lines++;

         continue;
      }

      index++;
   }
}