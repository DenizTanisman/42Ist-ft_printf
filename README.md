*This project has been created as part of the 42 curriculum by itanisma.*

## Description

`ft_printf` is a reimplementation of the C standard library function `printf()`. This project teaches how to handle variadic functions in C and format string parsing.

The function supports the following conversions:
- `%c` - Print a single character
- `%s` - Print a string
- `%p` - Print a pointer in hexadecimal format
- `%d` - Print a decimal (base 10) number
- `%i` - Print an integer in base 10
- `%u` - Print an unsigned decimal (base 10) number
- `%x` - Print a number in hexadecimal (base 16) lowercase format
- `%X` - Print a number in hexadecimal (base 16) uppercase format
- `%%` - Print a percent sign

## Instructions

### Compilation
```bash
make
```

This will create the `libftprintf.a` library.

### Usage
Compile your program with the library:
```bash
cc your_program.c libftprintf.a -o output_name
```

### Makefile Rules

- `make` or `make all` - Compile the library
- `make clean` - Remove object files
- `make fclean` - Remove object files and the library
- `make re` - Recompile everything from scratch

## Algorithm & Data Structure

### Parsing Strategy

The format string is parsed character by character:
1. Regular characters are printed directly
2. When a `%` is encountered, the next character is checked
3. A dispatcher function (`parser`) routes to the appropriate conversion function
4. Each conversion function returns the number of characters printed

### Recursive Approach for Number Printing

Numbers are printed using recursive functions:
- For multi-digit numbers, the function recursively calls itself with `n / 10`
- The last digit is printed using `n % 10`
- The return value accumulates the total character count

**Why recursive?**
- Naturally handles the "most significant digit first" requirement
- Each recursive call maintains its own count
- Counts are propagated upward through return values

**Example for 542:**
```
ft_print_int(542)
├─ ft_print_int(54)    → returns 2
│  └─ ft_print_int(5)   → returns 1
│     └─ prints '5'
│  └─ prints '4'
└─ prints '2'
Total: 3 characters printed
```

## Resources

- [man 3 printf](https://man7.org/linux/man-pages/man3/printf.3.html)
- [stdarg.h documentation](https://en.cppreference.com/w/c/variadic)
- [Variadic Functions in C](https://www.gnu.org/software/libc/manual/html_node/Variadic-Functions.html)

### AI Usage

AI was used for:
- Understanding variadic functions (`va_list`, `va_start`, `va_arg`, `va_end`)
- Debugging recursive counting logic
- Clarifying C type promotion rules in variadic functions
- Code structure and organization guidance

All core logic and implementation were written manually.

### Moulinette (%100)
- test_spdxiucpercent: 8/8 correct functions 
- bonus_one: KO (Does not compile) // No bonus
- bonus_two: KO (Does not compile)
# 42Ist-ft_printf
