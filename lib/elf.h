#ifndef HOLYC_LIB_ELF
#define HOLYC_LIB_ELF

typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Xword;

#define EI_NIDENT 16

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off  e_phoff;
  Elf64_Off  e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct {
  Elf64_Word  p_type;
  Elf64_Word  p_flags;
  Elf64_Off   p_offset;
  Elf64_Addr  p_vaddr;
  Elf64_Addr  p_paddr;
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz;
  Elf64_Xword p_align;
} Elf64_Phdr;

#define EI_MAG0  0
#define ELFMAG0  0x7f

#define EI_MAG1  1
#define ELFMAG1  'E'

#define EI_MAG2  2
#define ELFMAG2  'L'

#define EI_MAG3  3
#define ELFMAG3  'F'

#define EI_CLASS 4
#define ELFCLASS64 2

#define EI_DATA  5
#define ELFDATA2LSB 1

#define EI_VERSION 6

#define EI_OSABI 7
#define ELFOSABI_SYSV  0

#define EI_ABIVERSION 8

#define EI_PAD  9

#define ET_EXEC  2

#define EM_X86_64	62

#define EV_CURRENT	1

#endif // HOLYC_LIB_ELF
