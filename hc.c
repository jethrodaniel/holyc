// = holyc
//
// == references
//
// - Terry Davis's mini compiler demo:
//   https://github.com/cia-foundation/TempleOS/blob/archive/Demo/Lectures/MiniCompiler.HC
//
// - Michael Lazear's assembler:
//   https://github.com/lazear/lass/blob/master/assembler.c#L53
//
// - Elf quine: https://medium.com/@MrJamesFisher/understanding-the-elf-4bd60daac571
//// https://medium.com/@MrJamesFisher/understanding-the-elf-4bd60daac571

// SysV x86_64 fn call registers (in order), rest on the stack
//
// - rdi
// - rsi
// - rdx
// - rcx
// - r8
// - r9

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#define EXIT_SUCCESS 0

asm(".globl _start");
asm("_start:");
  // main()
  asm("mov (%rsp), %rdi");           // argc
  asm("leaq 8(%rsp),%rsi");          // argv
  asm("leaq 16(%rsp,%rdi,8), %rdx"); // envp
  asm("call main");
  // exit()
  asm("movl %eax, %edi");
  asm("call exit");

int exit(int code) {
  asm("movq $60, %rax");
  asm("syscall");
}

int write(int fd, char *buf, int length) {
  asm("movq $1, %rax");
  asm("syscall");
}

int read(int fd, char *buf, int length) {
  asm("movq $0, %rax");
  asm("syscall");
}

int strlen(char *str) {
  int i = 0;
  while(str[i])
    i++;
  return i;
}

void print(char *str) {
  write(STDOUT_FILENO, str, strlen(str));
}


void puts(char *str) {
  print(str);
  write(STDOUT_FILENO, "\n", 1);
}

void putc(char c) {
  write(STDOUT_FILENO, &c, 1);
}

void warn(char *str) {
  write(STDERR_FILENO, str, strlen(str));
}

void die(char *str) {
  warn("[error]: ");
  warn(str);
  warn("\n");
}

int atoi(char *str) {
  int n = 0,
      i = 0,
      sign = 1;

  if (str[i] == '-') {
    i++;
    sign = -1;
  }

  for (; str[i] >= '0' && str[i] <= '9'; i++)
    n = n * 10 + str[i] - '0';

  return n * sign;
}

void reverse(char *str) {
  char tmp;
  int len = strlen(str);

  for (int i = 0, j = len - 1; i < j; i++, j--) {
    tmp = str[j];
    str[j] = str[i];
    str[i] = tmp;
  }
}

void itoa(int n, char *str) {
  int i = 0,
      orig = n;

  if (n < 0)
    n *= -1;

  do {
    str[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (orig < 0)
    str[i++] = '-';

  str[i] = '\0';
  reverse(str);
}

char lower(char c) {
  if (c >= 'A' && c <= 'Z')
    return c + 'a' - 'A';
  return c;
}

// https://blog.nelhage.com/2010/10/amd64-and-va_arg/

void _printf_print_itoa(int n) {
  char buf[] = "     "; // 5
  itoa(n, buf);
  print(buf);
}

void printf(char *fmt, ...) {
  asm("pushq %r9");  // arg5
  asm("pushq %r8");  // arg4
  asm("pushq %rcx"); // arg3
  asm("pushq %rdx"); // arg2
  asm("pushq %rsi"); // arg1

  char *c = fmt;
  int popped = 0;

  while (*c) {
    if (*c != '%') {
      putc(*c++);
      continue;
    }

    if (popped >= 5)
      die("printf with more than 5 args unsupported");

    switch (*++c) { // eat '%'
    case 's':
      asm("popq %rdi");
      popped++;
      asm("call print");
      break;
    case 'd':
      asm("popq %rdi");
      popped++;
      asm("call _printf_print_itoa");
      break;
    case 'f':
      die("unsupported printf format '%f'");
    default:
      warn("unknown printf format '");
      putc(*c);
    }
    c++;
  }

  for (int i = 0; i < popped; i++)
    asm("popq %r8;"); // can we trash this reg?
}

#define INPUT_SIZE 4096

// #define ELF_START 0x8048000
#define ELF_START 0x401000
#define ELF_SIZE  120

int main(int argc, char **argv, char **envp) {
  printf("argc: %d\n", argc);

  for (int i = 0; i < argc; i++)
    printf("argv[%d] = %s\n", i, argv[i]);

  for (int i = 0; *envp; i++)
    printf("envp[%d] = %s\n", i, *envp++);

  return exit(argc);

  char input[INPUT_SIZE];
  int num_read;

  if ((num_read = read(STDIN_FILENO, input, INPUT_SIZE)) < 0)
    die("read");

  // temp: cat for now
  // write(STDOUT_FILENO, input, num_read);

  printf("read %d bytes\n", num_read);

  // write_elf(num_read);
  // write(STDOUT_FILENO, &input, num_read - 1); // rm \n

  return EXIT_SUCCESS;
}

// int write_elf(int program_length) {
//   uint64_t elf_offset = 0;
//   uint8_t *elf_output;

//   info("Making elf header...\n");

//   elf_offset = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr);
//   Elf64_Ehdr *e = malloc(sizeof(Elf64_Ehdr));
//   e->e_ident[EI_MAG0]       = 0x7f; // magic
//   e->e_ident[EI_MAG1]       = 'E';
//   e->e_ident[EI_MAG2]       = 'L';
//   e->e_ident[EI_MAG3]       = 'F';
//   e->e_ident[EI_CLASS]      = ELFCLASS64;
//   e->e_ident[EI_DATA]       = ELFDATA2LSB;
//   e->e_ident[EI_VERSION]    = EV_CURRENT;
//   e->e_ident[EI_OSABI]      = ELFOSABI_SYSV;
//   e->e_ident[EI_ABIVERSION] = 0;
//   e->e_ident[EI_PAD]        = 0;
//   e->e_type                 = ET_EXEC;
//   e->e_machine              = EM_X86_64;
//   e->e_version              = EV_CURRENT;
//   e->e_entry                = ELF_START + ELF_SIZE;
//   e->e_phoff                = sizeof(Elf64_Ehdr);
//   e->e_shoff                = 0;
//   e->e_flags                = 0;
//   e->e_ehsize               = sizeof(Elf64_Ehdr);
//   e->e_phentsize            = sizeof(Elf64_Phdr);
//   e->e_phnum                = 1;
//   e->e_shentsize            = 0;
//   e->e_shnum                = 0;
//   e->e_shstrndx             = 0;

//   Elf64_Phdr *p = malloc(sizeof(Elf64_Phdr));
//   p->p_type   = 1;
//   p->p_flags  = 5;
//   p->p_offset = 0;
//   p->p_vaddr  = ELF_START;
//   p->p_paddr  = 0x8048000;
//   p->p_filesz = program_length + elf_offset;
//   p->p_memsz = p->p_filesz;
//   p->p_align = 0x1000;
//   info("p_filesz: %ld bytes\n", p->p_filesz);

//   elf_output = malloc(elf_offset);
//   memcpy(elf_output, e, sizeof(Elf64_Ehdr));
//   memcpy(elf_output + sizeof(Elf64_Ehdr), p, sizeof(Elf64_Phdr));

//   write(STDOUT_FILENO, elf_output, elf_offset);
// }


