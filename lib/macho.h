
#include <lib/stdint.h>

// https://opensource.apple.com/source/xnu/xnu-2050.18.24/EXTERNAL_HEADERS/mach-o/loader.h



#define cpu_type_t    int32_t
#define cpu_subtype_t int32_t
typedef int           vm_prot_t;

#define MH_MAGIC_64            0xfeedfacf          // 64-bit mach magic number

#define CPU_ARCH_ABI64          0x01000000         // 64 bit ABI
#define CPU_TYPE_X86           ((cpu_type_t) 7)
#define CPU_TYPE_X86_64         (CPU_TYPE_X86 | CPU_ARCH_ABI64)

#define CPU_SUBTYPE_X86_64_ALL ((cpu_subtype_t)3)
#define MH_EXECUTE            0x2               // demand paged executable file

#define MH_NOUNDEFS 0x1  // the object file has no undefined references

#define LC_SEGMENT_64 0x19 // 64-bit segment of this file to be mapped

#define VM_PROT_READ    ((vm_prot_t) 0x01) // read permission
#define VM_PROT_WRITE   ((vm_prot_t) 0x02) // write permission
#define VM_PROT_EXECUTE ((vm_prot_t) 0x04) // execute permission

typedef struct mach_header_64 {
 uint32_t magic;           // mach magic number identifier
 cpu_type_t cputype;       // cpu specifier
 cpu_subtype_t cpusubtype; // machine specifier
 uint32_t filetype;        // type of file
 uint32_t ncmds;           // number of load commands
 uint32_t sizeofcmds;      // the size of all the load commands
 uint32_t flags;
 uint32_t reserved;
} mach_header_64;

typedef struct segment_command_64 {
 uint32_t cmd;       // LC_SEGMENT_64
 uint32_t cmdsize;   // includes sizeof section_64 structs
 char  segname[16];  // segment name
 uint64_t vmaddr;    // memory address of this segment
 uint64_t vmsize;    // memory size of this segment
 uint64_t fileoff;   // file offset of this segment
 uint64_t filesize;  // amount to map from the file
 vm_prot_t maxprot;  // maximum VM protection
 vm_prot_t initprot; // initial VM protection
 uint32_t nsects;    // number of sections in segment
 uint32_t flags;
} segment_command_64;

typedef struct section_64 { /* for 64-bit architectures */
  char    sectname[16]; // name of this section
  char    segname[16];  // segment this section goes in
  uint64_t  addr;       // memory address of this section
  uint64_t  size;       // size in bytes of this section
  uint32_t  offset;     // file offset of this section
  uint32_t  align;      // section alignment (power of 2)
  uint32_t  reloff;     // file offset of relocation entries
  uint32_t  nreloc;     // number of relocation entries
  uint32_t  flags;      // flags (section type and attributes
  uint32_t  reserved1;  // reserved (for offset or index)
  uint32_t  reserved2;  // reserved (for count or sizeof)
  uint32_t  reserved3;
} section_64;

// the pagezero segment which has no protections and
// catches NULL references for MH_EXECUTE files
#define	SEG_PAGEZERO	"__PAGEZERO"

// the tradition UNIX text segment
#define	SEG_TEXT	"__TEXT"

// the real text part of the text section no headers,
// and no padding
#define	SECT_TEXT	"__text"

// section contains only true machine instructions
#define S_ATTR_PURE_INSTRUCTIONS 0x80000000

// section contains some machine instructions
#define S_ATTR_SOME_INSTRUCTIONS 0x00000400

// the segment containing all structs created and maintained
// by the link editor.  Created with -seglinkedit option to
// ld(1) for MH_EXECUTE and FVMLIB file types only
#define	SEG_LINKEDIT	"__LINKEDIT"
