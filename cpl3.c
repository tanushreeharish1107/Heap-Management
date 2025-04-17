#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Symbol {
    char name[50];
    int value;
    struct Symbol *next;
} Symbol;

typedef struct Scope {
    Symbol *table[TABLE_SIZE];
    struct Scope *parent;
} Scope;

Scope *current_scope = NULL;

unsigned int hash(const char *name) {
    unsigned int hash = 0;
    while (*name) {
        //random hash function
        hash = (hash * 31) + *name++;
    }
    return hash % TABLE_SIZE;
}

Scope *create_scope(Scope *parent) {
    Scope *new_scope = (Scope *)malloc(sizeof(Scope));
    for (int i = 0; i < TABLE_SIZE; i++) {
        new_scope->table[i] = NULL;
    }
    new_scope->parent = parent;
    return new_scope;
}

void begin_scope() {
    current_scope = create_scope(current_scope);
}

void end_scope() {
    if (current_scope) {
        Scope *old_scope = current_scope;
        current_scope = current_scope->parent;
        free(old_scope);
    }
}

void insert(const char *name, int value) {
    if (!current_scope) return;
    unsigned int index = hash(name);
    Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    strcpy(new_symbol->name, name);
    new_symbol->value = value;
    new_symbol->next = current_scope->table[index];
    current_scope->table[index] = new_symbol;
}

int lookup(const char *name) {
    Scope *scope = current_scope;
    while (scope) {
        unsigned int index = hash(name);
        Symbol *symbol = scope->table[index];
        while (symbol) {
            if (strcmp(symbol->name, name) == 0) {
                return symbol->value;
            }
            symbol = symbol->next;
        }
        scope = scope->parent;
    }
    return -1; // Not found
}

void process_command(const char *command, const char *var, int value) {
    if (strcmp(command, "begin") == 0) {
        begin_scope();
    } else if (strcmp(command, "end") == 0) {
        end_scope();
    } else if (strcmp(command, "assign") == 0) {
        insert(var, value);
    } else if (strcmp(command, "print") == 0) {
        int result = lookup(var);
        if (result != -1) {
            printf("%s : %d\n",var , result);
        } else {
            printf("Variable %s not found\n", var);
        }
    }
}

int main() {
    begin_scope();
    process_command("assign", "a", 1);
    process_command("assign", "b", 2);
    process_command("begin", "", 0);
    process_command("assign", "a", 3);
    process_command("assign", "c", 4);
    process_command("print", "b", 0);
    process_command("begin", "", 0);
    process_command("assign", "c", 5);
    process_command("print", "b", 0);
    process_command("print", "a", 0);
    process_command("print", "c", 0);
    process_command("end", "", 0);
    process_command("print", "a", 0);
    process_command("print", "c", 0);
    process_command("end", "", 0);
    process_command("print", "a", 0);
    return 0;
}
