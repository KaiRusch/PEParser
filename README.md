PEParser
========
Can parse and display information about Windows EXE and OBJ files;

**To Use:**

`>parser <filename> -options`

**Options:**

Specify a section to look at:

`-S <section_name>`

Only show header

`-H`

**Example:**

Output the .text section header of a.exe

`>parser a.exe -S .text -H`
