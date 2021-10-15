#pragma once

#include <elf.h>

void *readFile(const char *filename);

Elf64_Shdr *findSection(void *elfFile, const char *sectionName);

void parseSymtab(void *elfFile);