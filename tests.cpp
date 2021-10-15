#include "parser/ElfParser.h"
#include <iostream>

void test()
{
    void *elf_file = readFile("./ElfParser.o");
    auto elfHeader = static_cast<Elf64_Ehdr *>(elf_file);

    auto sectionHeader = static_cast<Elf64_Shdr *>(elf_file + elfHeader->e_shoff);
    char *shstr = static_cast<char *>(elf_file + (sectionHeader + elfHeader->e_shstrndx)->sh_offset);
    char *text = static_cast<char *>(elf_file + findSection(elf_file, ".text")->sh_offset);

    auto rodata = findSection(elf_file, ".rodata");
    for (size_t i = 0; i < rodata->sh_size; i++)
    {
        std::cout << ((char *)elf_file + rodata->sh_offset)[i];
    }
}


int main(int argc, char const *argv[])
{
    test();
    return 0;
}

