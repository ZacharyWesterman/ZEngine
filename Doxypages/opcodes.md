All `{var}` and such indicate a variable index. The variable index `{0}` reserved as **NULL** and as such any modify instructions on it are ignored.<BR>
Variable index `{1}` is defined as parameters from the parent program. This will always be an array.<BR>
Aside from these, all memory must be allocated by the program.
If a program attempts to read from or assign to a variable index that is not currently allocated, A **BADMEMORY** exception is thrown.

Also of note is that the memory index is determined by all but the last bit, shifted right once.<BR>
If the last bit of the index is `1`, then it is an offset from the top of the memory stack,
otherwise it references an exact memory index.

Boolean values:
- Arrays: Null arrays evaluate as **false**, all others evaluate as **true**.
- Arithmetic types: value of exactly **0** evaluates as **false**, all others evaluate as **true**.
- Strings: **true** if it evaluates to a non-zero arithmetic value, **false** otherwise.
- Null: Evaluates to **false**.

##No-OP
| OP  | Description |
| --- | ----------- |
| NOP | Do nothing  |

##Arithmetic
If any of the inputs cannot be evaluated to an arithmetic value, the operation assumes the input(s) to be **0** and performs the operation, then generates an **ARITHMETIC** exception of **NONARITHMETIC**.<BR>
Division by **0** generates an **ARITHMETIC** exception of **DIVBYZERO**.<BR>
Overflow or underflow generates an **ARITHMETIC** exception of **OVERFLOW** or **UNDERFLOW**, and returns MAX-INT or MIN-INT respectively.<BR>
Any other invalid inputs are assumed to be **0**, and generate an **ARITHMETIC** exception of **BADINPUT**.<BR>
Note that all boolean or bitwise operations only operate on the *real integral* part of arithmetic values.
The rest is ignored and truncated.

| OP   |Input 1|Input 2| Output | Description                                     |
| ---- | ----- | ----- | ------ | ----------------------------------------------- |
|ADD   |{var1} |{var2} |{varOut}|add {var1} and {var2}, storing result in {varOut}|
|SUB   |{var1} |{var2} |{varOut}|subtract {var2} from {var1}, storing result in {varOut}|
|MUL   |{var1} |{var2} |{varOut}|multiply {var1} by {var2}, storing result in {varOut}|
|DIV   |{var1} |{var2} |{varOut}|divide {var1} by {var2}, storing result in {varOut}|
|IDIV  |{var1} |{var2} |{varOut}|divide {var1} by {var2}, truncating fractional values and storing result in {varOut}|
|REMD  |{var1} |{var2} |{varOut}|divide {var1} by {var2}, storing remainder in {varOut}|
|POW   |{var1} |{var2} |{varOut}|raise {var1} to the {var2} power, storing result in {varOut}|
|FAC   |{var}  |       |{varOut}|calculate the factorial of ABS(INT({var})), storing the result in {varOut}|
|AND   |{var1} |{var2} |{varOut}|perform boolean AND on {var1} and {var2}, storing the result in {varOut}|
|OR    |{var1} |{var2} |{varOut}|perform boolean OR on {var1} and {var2}, storing the result in {varOut}|
|NAND  |{var1} |{var2} |{varOut}|perform boolean NAND on {var1} and {var2}, storing the result in {varOut}|
|NOR   |{var1} |{var2} |{varOut}|perform boolean NOR on {var1} and {var2}, storing the result in {varOut}|
|XOR   |{var1} |{var2} |{varOut}|perform boolean XOR on {var1} and {var2}, storing the result in {varOut}|
|XNOR  |{var1} |{var2} |{varOut}|perform boolean XNOR on {var1} and {var2}, storing the result in {varOut}|
|NOT   |{var}  |       |{varOut}|perform boolean NOT on {var1}, storing the result in {varOut}|
|ANDB  |{var1} |{var2} |{varOut}|perform bitwise AND on {var1} and {var2}, storing the result in {varOut}|
|ORB   |{var1} |{var2} |{varOut}|perform bitwise OR on {var1} and {var2}, storing the result in {varOut}|
|NANDB |{var1} |{var2} |{varOut}|perform bitwise NAND on {var1} and {var2}, storing the result in {varOut}|
|NORB  |{var1} |{var2} |{varOut}|perform bitwise NOR on {var1} and {var2}, storing the result in {varOut}|
|XORB  |{var1} |{var2} |{varOut}|perform bitwise XOR on {var1} and {var2}, storing the result in {varOut}|
|XNORB |{var1} |{var2} |{varOut}|perform bitwise XNOR on {var1} and {var2}, storing the result in {varOut}|
|NOTB  |{var}  |       |{varOut}|perform bitwise NOT on {var1}, storing the result in {varOut}|
|REAL  |{var}  |       |{varOut}|store the real part of {var} in {varOut}|
|IMAG  |{var}  |       |{varOut}|store the imaginary part of {var} in {varOut}|
|INT   |{var}  |       |{varOut}|store the real integral part of {var} in {varOut}|
|CONCAT|{var1} |{var2} |{varOut}|If either input is an array, concatenates sub-elements into the same array. Otherwise, concatenates into a string|
|CHAR  |{var}  |       |{varOut}|Convert the integer value {var} into its character equivalent|
|VALUE |{var}  |       |{varOut}|Convert the first character of the string {var} into its integer equivalent|

The following OPs work on array-like data. If the data they operate on is not a string or an array, it is first converted to a string and then operated on.<BR>
If the index is not an integer or an array of integers, an **ARITHMETIC** exception of **BADINDEX** is thrown.<BR>
However, if the index is out of bounds or operates on a null item, the operation just returns NULL.

If {var2} of the following is an array, then operations proceed on sub-values of {var1}.<BR>
Suppose we have the example items stored in memory:
~~~
2: {1,{2,3},4,{5},"six"}
3: 0
4: 4
5: {4}
6: {4,0}
7: {1,1}
8: {1}
9: {0,0}
A: 3
B: {3}
C: {3,0}
D: {3,7}
E: {4,1,0}
F: NULL
~~~

| Operation     | Result   |
| ------------- | -------- |
|`GETELEM 2 3 F`|`9: 1`    |
|`GETELEM 2 4 F`|`9: "six"`|
|`GETELEM 2 5 F`|`9: "six"`|
|`GETELEM 2 6 F`|`9: "s"`  |
|`GETELEM 2 7 F`|`9: 3`    |
|`GETELEM 2 8 F`|`9: {2,3}`|
|`GETELEM 2 9 F`|`9: NULL` |
|`GETELEM 2 A F`|`9: {5}`  |
|`GETELEM 2 B F`|`9: {5}`  |
|`GETELEM 2 C F`|`9: 5`    |
|`GETELEM 2 D F`|`9: NULL` |
|`GETELEM 2 E F`|`9: "i"`  |
<BR>

| OP    |Input 1|Input 2|Input 3| Output | Description                                     |
| ----- | ----- | ----- | ------| ------ | ----------------------------------------------- |
|INSERT |{var1} |{var2} |{var3} |{varOut}|Insert {var3} as an element before the {var2} element of {var1}, storing in {varOut}|
|REMOVE |{var1} |{var2} |       |{varOut}|Remove the {var2} element from {var1}, storing result in {varOut}|
|GETELEM|{var1} |{var2} |       |{varOut}|Get the {var2} subvalue of {var1}, storing result in {varOut}|
|SETELEM|{var1} |{var2} |{var3} |{varOut}|Set the {var2} subvalue of {var1} to {var3}, storing result in {varOut}|
|COUNT  |{var1} |{var2} |       |{varOut}|If {var2} is NULL, set {varOut} to the element/character count of {var1}. Otherwise do so to the sub-index specified by {var2}|


##Comparison
| OP  |Input 1|Input 2| Output | Output is true if:                              |
| --- | ----- | ----- | ------ | ----------------------------------------------- |
|EQ   |{var1} |{var2} |{varOut}|the values of {var1} and {var2} are equivalent|
|NE   |{var1} |{var2} |{varOut}|the values of {var1} and {var2} are **not** equivalent|
|EQS  |{var1} |{var2} |{varOut}|the exact types and values of {var1} and {var2} are the same|
|NES  |{var1} |{var2} |{varOut}|the exact types or values of {var1} and {var2} are **not** the same|

| OP  |Input 1|Input 2| Output |(both are arithmetic) True if:|(one or both are strings) True if:|
| --- | ----- | ----- | ------ | ---------------------------- | -------------------------------- |
|GT   |{var1} |{var2} |{varOut}| {var1} > {var2}              |{var1} comes after {var2} alphabetically|
|GE   |{var1} |{var2} |{varOut}| {var1} ≥ {var2}              |{var1} does not come before {var2} alphabetically|
|LT   |{var1} |{var2} |{varOut}| {var1} < {var2}              |{var1} comes before {var2} alphabetically|
|LE   |{var1} |{var2} |{varOut}| {var1} ≤ {var2}              |{var1} does not come after {var2} alphabetically|

##Memory Management
| OP    |Input | Output | Description                                     |
| ----- | ---- | ------ | ----------------------------------------------- |
|MOVE   |{var} |{varOut}|Assign the value in {var} to {varOut}|
|MEMSET |$const|{varOut}|Assign {varOut} to the static value $const|
|ALLOC  |$const|        |Allocate exactly $const memory slots, incrementing the memory stack|
|ALLOCV |{var} |        |Allocate the number of memory slots specified in {var}|
|FREE   |$const|        |Deallocate $const memory slots, decrementing the memory stack|
|FREEV  |{var} |        |Deallocate the number of memory slots specified in {var}|
|MEMSIZE|      |{varOut}|set {varOut} to the number of memory slots currently allocated|

##Processes
| OP    |Input | Output | Set output to unique handle:           |Otherwise set to:|
| ----- | ---- | ------ | -------------------------------------- | --------------- |
|DEFCMD |{var} |{varOut}|if {var} contains a valid command alias | **NULL** |
|DEFFUNC|{var} |{varOut}|if {var} contains a valid function alias| **NULL** |

The following OPs must be given a valid handle as input, otherwise they will throw a **BADHANDLE** exception of type **CALL**.<BR>
Note that third-party commands or functions may throw their own exceptions even on successful execution.
These exceptions may be of any type, and are up to the programmer to catch.

| OP      |Input | Output | On call if valid handle:                                        |
| ------- | ---- | ------ | --------------------------------------------------------------- |
|CALLCMD  |{var} |        |Run command, continuing program execution when it finishes       |
|CALLFUNC |{var} |{varOut}|Call function, continuing program execution when it finishes, and setting {varOut} to the return value|
|ASYNCCMD |{var} |{varOut}|Run command, continuing program execution immediately and storing process handle in {varOut}|
|ASYNCFUNC|{var} |{varOut}|Call function, continuing program execution immediately and storing process handle in {varOut}|
|CMDSTAT  |{var} |{varOut}|{varOut} is set to a positive value if the command finished successfully, negative if errored, and 0 if still running|
|FUNCSTAT |{var} |{varOut}|{varOut} is set to a positive value if the function finished successfully, negative if errored, and 0 if still running|
|JOINFUNC |{var} |{varOut}|if {var} is a valid handle, wait for the function to complete then set {varOut} to its return value. Otherwise set to **NULL** |
|JOINCMD  |{var} |        |if {var} is a valid handle, wait for the command to complete|
|FSTATIC  |{var} |{varOut}|{varOut} is set to true if {var} is a handle to a function that always gives the same output for a given input|


The following OPs throw a **BADPATH** exception if any of the input paths are non-existant.

| OP        |Input 1 |Input 2     |Input 3     |Output  | Description |
| --------- | ------ | ---------- | ---------- | ------ | ----------- |
|CPL        |{inPath}|{outPath}   |{options}   |        |Compile source {inPath} to dest {outPath} with flags {options}|
|CPLRUN     |{inPath}|{options}   |{callParams}|{varOut}|Compile source and immediately run with {callParams}. {varOut} is set to the return value of the program|
|RUN        |{inPath}|{callParams}|            |{varOut}|Run the source with params {callParams}. {varOut} is set to the return value of the program|
|ASYNCCPL   |{inPath}|{outPath}   |{options}   |{handle}|Start compile process on input, returning process handle in {handle}|
|ASYNCCPLRUN|{inPath}|{options}   |{callParams}|{handle}|Start compile and execution process, returning process handle in {handle}|
|ASYNCRUN   |{inPath}|{callParams}|            |{varOut}|Start execution process, returning process handle in {handle}|
|TCPL       |{var}   |{outPath}   |{options}   |        |Compile the text in {var} to dest {outPath} with flags {options}|
|TCPLRUN    |{var}   |{options}   |{callParams}|{varOut}|Compile text and immediately run with {callParams}. {varOut} is set to the return value of the program|
|ASYNCTCPL  |{var}   |{outPath}   |{options}   |{handle}|Start compile process on text, returning process handle in {handle}|
|ASYNCTCPLRUN|{var}  |{options}   |{callParams}|{handle}|Start compile and execution process on text, returning process handle in {handle}|
|JOINPROC   |{handle}|            |            |{varOut}|If {handle} is a valid handle, wait for the process to complete then set {varOut} to its output, if any|
|KILLPROC   |{handle}|            |            |        |If the specified child process, function, or command is still running, kill it|

##Shared Data
Note that for the following OPs, if an input that is not a handle to a currently existing shared variable is given, a **BADHANDLE** exception of type **VAR** will be thrown.<BR>
Also note that on exit, any shared variables that this process has locked will be unlocked.

| OP     |Input 1 |Input 2| Output | Description |
| ------ | ------ | ----- | ------ | ----------- |
|SHDEF   |{var}   |       |{handle}|Define a new shared variable if it doesn't exist. In all cases return a {handle} to the shared variable|
|SHEXISTS|{var}   |       |{varOut}|set {varOut} to true if the shared variable is defined. Set to false otherwise|
|SHUNDEF |{handle}|       |        |Undefine the shared variable. Note that this will make the handle subsequently invalid|
|SHSET   |{handle}|{var}  |        |set the shared variable to the value in {var}|
|SHGET   |{handle}|       |{varOut}|set {varOut} to the shared variable's current value|
|SHLOCK  |{handle}|       |        |Only allow this process to write to the shared variable. Other processes that attempt to write to the variable will wait until the lock is released.
|SHUNLOCK|{handle}|       |        |Release this process' lock on a shared variable. If the variable is not locked by this process, this does nothing.
|SHLOCKED|{handle}|       |{varOut}|set {varOut} to true if the shared variable is locked. set to false otherwise|

##Control
If a jump is specified to an index outside of the instruction space, a **JUMP** exception of **BADINDEX** is thrown.<BR>
If a jump is specified to a non-integral index, a **JUMP** exception of **BADTYPE** is thrown.<BR>

| OP     |Input 1 |Input 2| Description |
| ------ | ------ | ----- | ----------- |
|JTRUE   |{var}   |$instr |Jump to the instruction at $instr if {var} evaluates to true|
|JFALSE  |{var}   |$instr |Jump to the instruction at $instr if {var} evaluates to false|
|JUMP    |$instr  |       |Jump to the instruction at $instr unconditionally|
|JPUSH   |$instr  |       |Push the next instruction index to the routine stack and jump to the instruction at $instr|
|JVTRUE  |{var}   |{instr}|Jump to the instruction at the index stored in {instr} if {var} evauates to true|
|JVFALSE |{var}   |{instr}|Jump to the instruction at the index stored in {instr} if {var} evauates to false|
|JUMPV   |{instr} |       |Jump to the instruction at the index stored in {instr} unconditionally|
|JVPUSH  |{instr} |       |Push the next instruction index to the routine stack and jump to the instruction index stored in {instr}|
|RETURN  |        |       |Pop the last instruction index from the routine stack and jump there. If the routine stack is empty, returns to the parent process|
|EXIT    |        |       |Immediately return to the parent process|

##Exceptions
| OP     |Input 1 |Input 2| Output | Description |
| ------ | ------ | ----- | ------ |----------- |
|JEXCEPT |$instr  |       |        |Set the VM to jump to the instruction at $instr when an exception occurs, instead of terminating. If bad index, does nothing|
|JVEXCEPT|{instr} |       |        |Set the VM to jump to the instruction at the index in ${instr} when an exception occurs. If non-integral input or bad index, does nothing|
|DEXCEPT |        |       |        |Return to default exception handling behavior|
|EXCEPT  |{type}  |{desc} |        |Causes an exception of type {type} to be thrown, with a subtype/description of {desc}|
|TEXCEPT |        |       |{varOut}|Get the type of the last exception thrown. If none, sets {varOut} to **NULL**|
|SEXCEPT |        |       |{varOut}|Get the subtype/description of the last exception thrown. If none, sets {varOut} to **NULL**|


##Data Types
| Type  | ID |
| ----- | -- |
|NULL   |0   |
|INT    |1   |
|FLOAT  |2   |
|COMPLEX|3   |
|STRING |4   |
|ARRAY  |5   |

Convert the type of {var}, storing result in {varOut}. These OPs do not throw any exceptions.

| OP      | Input | Output | Description |
| ------- | ----- | ------ | ----------- |
|REDUCE   |{var}  |{varOut}|Reduce {var} to it's simplest type without losing information. Complex numbers with no imaginary part reduce to floats.<BR>Floats with no fractional part reduce to integers. Null strings and pointers reduce to NULL. Arrays do not reduce.|
|TOARRAY  |{var}  |{varOut}|Encase {var} in an array, storing in {varOut}. If {var} is **NULL**, {varOut} becomes a null array|
|TOSTRING |{var}  |{varOut}|Convert {var} to a string|
|TOCOMPLEX|{var}  |{varOut}|Convert {var} to a complex value if possible. If not, give `0+0i`.|
|TOFLOAT  |{var}  |{varOut}|Convert {var} to a floating-point value if possible. If not, give `0.0`|
|TOINT    |{var}  |{varOut}|Convert {var} to an integral value if possible. If not, give `0`|
|TOBOOL   |{var}  |{varOut}|If {var} is a non-null array, gives true. If {var} casts to a non-zero arithmetic value, gives true. Otherwise gives false.|

Get the type of {var}, or what it can cast to without losing information. These OPs do not throw any exceptions.

| OP      | Input | Output | Description |
| ------- | ----- | ------ | ----------- |
|TYPE     |{var}  |{varOut}|Set {varOut} to the type ID of {var}|
|TYPESTR  |{var}  |{varOut}|Set {varOut} to a string representing the type of {var}|
|ISARITH  |{var}  |{varOut}|Set {varOut} to true if {var} is any arithmetic type|
|ISCOMPLEX|{var}  |{varOut}|Set {varOut} to true if {var} can freely cast to a complex value. Set to false otherwise|
|ISFLOAT  |{var}  |{varOut}|Set {varOut} to true if {var} can freely cast to a floating-point value. Set to false otherwise|
|ISINT    |{var}  |{varOut}|Set {varOut} to true if {var} can freely cast to an integeral value. Set to false otherwise|
|ISNULL   |{var}  |{varOut}|Set {varOut} to true if {var} is NULL, a null string, or a null array. Set to false otherwise|

##VM Manipulation
| OP    | Input | Output | Description |
| ----- | ----- | ------ | ----------- |
|STDIN  |       |{varOut}|Read a line from stdin|
|STDOUT |{var}  |        |Convert {var} to a string then write to stdout|
|STDERR |{var}  |        |Convert {var} to a string then write to stderr|
|WAIT   |{var}  |        |Pause program execution for {var} milliseconds. If {var} does not evaluate to a positive number, this does nothing|
|SETIPS |{var}  |        |Set the (approximate) number of instructions to run per second. If 0 or less, then it is unlimited|
|GETIPS |       |{varOut}|Get the current goal IPS|

##Debug Information
These OPs are for helping pinpoint where exceptions occurred.

If no debug line is specified before an exception occurs, exceptions thrown will indicate the instruction number.<BR>
By default, the debug file is the currently running object file.<BR>

| OP      | Input | Description |
| ------- | ----- | ----------- |
|DEBUGLINE|$line  |Set the source line to indicate|
|DEBUGFUNC|{var}  |Set the source function to indicate|
|DEBUGFILE|{var}  |Set the source file to indicate|
|DEBUGOBJ |{var}  |Set the object or structure to indicate|
