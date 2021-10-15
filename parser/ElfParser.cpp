#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

void *readFile(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd != -1)
    {
        struct stat buf_stat;
        fstat(fd, &buf_stat);
        void *buffer = new char[buf_stat.st_size];
        if (read(fd, buffer, buf_stat.st_size) != -1)
        {
            return buffer;
        }
    }
    return nullptr;
}

Elf64_Shdr *findSection(void *elfFile, const char *sectionName)
{
    auto elfHeader = static_cast<Elf64_Ehdr *>(elfFile);
    auto sectionHeader = static_cast<Elf64_Shdr *>(elfFile + elfHeader->e_shoff);
    char *shstr = static_cast<char *>(elfFile + (sectionHeader + elfHeader->e_shstrndx)->sh_offset); //strings for section headers

    for (size_t i = 0; i < elfHeader->e_shnum; i++)
    {
        auto currentSectionName = &shstr[sectionHeader[i].sh_name];
        if (strcmp(currentSectionName, sectionName) == 0)
        {
            return &sectionHeader[i];
        }
    }
    return nullptr;
}

void parseSymtab(void *elfFile)
{
    char *strtab = static_cast<char *>(elfFile + findSection(elfFile, ".strtab")->sh_offset);
    auto *symtab = findSection(elfFile, ".symtab");
    int countSymtabItems = symtab->sh_size / symtab->sh_entsize;

    for (size_t i = 0; i < countSymtabItems; i++)
    {
        auto symtabItem = (Elf64_Sym *)(elfFile + symtab->sh_offset) + i;
        std::cout << i << " ";
        switch (symtabItem->st_info)
        {
        case STT_FUNC:
            std::cout << "FUNC " << &strtab[symtabItem->st_name];
            break;
        case STT_OBJECT:
            std::cout << "OBJECT " << &strtab[symtabItem->st_name];
            break;
        case STT_FILE:
            std::cout << "FILE " << &strtab[symtabItem->st_name];
            break;
        default:
            std::cout << "ANY " << &strtab[symtabItem->st_name];
        }
        std::cout << std::endl;
    }
}