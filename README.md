# sol

A **S**tack **O**riented **L**anguage for code golfing

## Contents
1. Basics
2. Commands
3. Running
___

### 1. Basics
sol can only store one type on the stack, which is the integer.

To push an integer to the stack, simply type it:
```
\ this is a comment \
10
```
To push multiple numbers, separate them with either commands or no-op characters like spaces or newlines:
```
1 2
3
```
Leading zeros, however, can be pushed without the need of spaces:
```
\ pushes 0, 0, and 1 to the stack \
001

\ pushes 0 and 10 to the stack \
010
```
Negative numbers can also be pushed, but you must subtract a positive number from zero (this may be changed to be simpler later):
```
\ pushes -1 to the stack \
1 0-
```

Operators allow you to do things with the stack. Operators are always single characters and thus do not require whitespace between them.
```
\ print fibonacci sequence indefinitely \
01 1@<+#!_10._$1j
```

You can also use quotes to specify a string that should be printed, making string output much easier. Here's a "Hello world" program:
```
"Hello, world!"
```
Strings are not pushed to the stack.

### 2. Commands
NOTE: Any operation that has **POPS** in its description will automatically pop elements that it uses from the stack.

NOTE 2: `a` = 1st stack value, `b` = 2nd stack value

#### Output
- `!` -- Print `a`
- `.` -- Print ascii equivalent of `a`

#### Stack Manipulation
- `#` -- Duplicate `a` and push `a`
- `_` -- Pop the top value (drop)
- `$` -- Swap `a` and `b`
- `>` -- Move `a` to 3rd position (rot)
- `<` -- Duplicate `b` and push to stack (over)
- `b` -- Move `a` to the bottom of the stack
- `t` -- Move bottom value to the top of the stack

#### Math
- `+` -- Add top 2 stack values (a+b) (**POPS**)
- `-` -- Subtract top 2 stack values (b-a) (**POPS**)
- `*` -- Multiply top 2 stack values (a*b) (**POPS**)
- `/` -- Divide top 2 stack values (b/a) (**POPS**)
- `%` -- Modulo top 2 stack values (b%a) (**POPS**)
- `^` -- Exponentiate top 2 stack values (b^a) (**POPS**)

#### Boolean Logic
NOTE: 0 = `false`, nonzero = `true` (**POPS**)
- `=` -- Check a==b (**POPS**)
- `&` -- Check a&&b (**POPS**)
- `|` -- Check a||b (**POPS**)
- `n` -- Not operation on `a`
- `g` -- Check a>b (**POPS**)
- `l` -- Check a<b (**POPS**)
- `G` -- Check a>=b (**POPS**)
- `L` -- Check a<=b (**POPS**)
- `?` -- If `a` is zero, skip the next line (**POPS**)

#### Iteration
- `@` -- Set a label; named according to the previous token
  - Initialized before interpretation, allows for jumping to later sections of code
- `j` -- Jump to label named `a` (**POPS**)

#### Input
- `i` -- Get integer input from stdin and push to the stack.
- `I` -- Get string input from stdin. The ascii equivalent of each charater will be pushed to the stack in reverse order, followed by the length of the inputted string. For example, here's the result when `string` is inputted:
```
(left is top of the stack)
[6, 115, 116, 114, 105, 110, 103]
len  s    t    r    i    n    g
```

#### Other
- `r` -- Push random integer in range `[a, b)` to the stack (**POPS**)
- `s` -- Push the size of the stack to the stack

___

### 3. Running

To run from cli, use the following command:
```
./main [script path] (flags)
```

List of Flags:
- `-d` -- Print debug info after program is finished
- `-t` -- Time execution
- `-s` -- Step through script
