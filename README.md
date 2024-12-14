# Intro to the Course

The course consists of multiple parts:

1. **First part**: Theoretical prelude to things that make programs fast or slow.
    - Waste (in terms of clock cycles)
    - Dependency chains
    - SIMD

2. **Second part**: Decoding bits to ASM instructions like `mov` (`100010`), `cmp`, `add`, etc.

3. **Third part**: Simulating CPU behavior from input bits, using a library provided as a DLL and its interface (`sim86_shared.h`).

---

## Part 2

### Homework 1
Learning how the assembler decodes binary to assembly instructions, focusing on register-to-register `mov` operations.  
Finished in Java. *(Listing 38)*

Skipped the rest of the homework as it seemed mostly the same as the first one, just tedious bookkeeping.

---

## Part 3

### Homework 2 and 3
Start of simulating execution of assembler instructions:

- **Listing 43**: Immediate-to-register `mov` operations.
- **Listing 44**: Register-to-register `mov` operations.
- **Listing 46**: `cmp`, `add`, `sub`, `mov` instructions, as well as handling zero-flag and sign-flag.
