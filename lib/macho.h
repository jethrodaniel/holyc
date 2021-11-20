
#include <lib/stdint.h>

// https://opensource.apple.com/source/xnu/xnu-2050.18.24/EXTERNAL_HEADERS/mach-o/loader.h



#define cpu_type_t    int32_t
#define cpu_subtype_t int32_t

#define MH_MAGIC_64            0xfeedfacf          // 64-bit mach magic number

#define CPU_ARCH_ABI64          0x01000000         // 64 bit ABI
#define CPU_TYPE_X86           ((cpu_type_t) 7)
#define CPU_TYPE_X86_64         (CPU_TYPE_X86 | CPU_ARCH_ABI64)

#define CPU_SUBTYPE_X86_64_ALL ((cpu_subtype_t)3)
#define	MH_EXECUTE	           0x2		             // demand paged executable file

typedef struct mach_header_64 {
	uint32_t	magic;		      // mach magic number identifier
	cpu_type_t	cputype;	    // cpu specifier
	cpu_subtype_t	cpusubtype;	// machine specifier
	uint32_t	filetype;	      // type of file
	uint32_t	ncmds;		      // number of load commands
	uint32_t	sizeofcmds;	    // the size of all the load commands
	uint32_t	flags;
	uint32_t	reserved;
} mach_header_64;

typedef struct load_command {
	uint32_t cmd;		/* type of load command */
	uint32_t cmdsize;	/* total size of command in bytes */
} load_command;
