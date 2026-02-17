# logscan-c

A tiny log pattern scanner written in C. It searches for a substring in
a text file and prints matching lines with line numbers. Designed for
quick log triage during security investigations.

## Features

-   Substring search (minimal `grep`-like behavior)
-   Optional case-insensitive mode (`-i`) using a simple
    ASCII/byte-based comparison
-   Prints matching lines prefixed with `[line_number]`
-   Prints total match count at the end

## Usage

``` bash
logscan <file> <pattern> [-i]
```

### Examples

Case-sensitive:

``` bash
logscan auth.log "Failed password"
```

Case-insensitive:

``` bash
logscan auth.log "failed password" -i
```

## Output

Each match is printed like:

    [123] original log line here

And at the end:

    Total matches: N

## Build

### Windows 11 (MSVC `cl`)

Open the "Developer Command Prompt for VS" (or "x64 Native Tools Command
Prompt") and run:

``` bat
cl /std:c11 /W4 /O2 logscan.c
```

Run:

``` bat
logscan.exe <file> <pattern> [-i]
```

### Windows 11 (clang)

If `clang` is available in your PATH:

``` bat
clang -std=c11 -Wall -Wextra -Wpedantic -O2 -o logscan.exe logscan.c
```

### Linux/macOS (clang or gcc)

``` bash
cc -std=c11 -Wall -Wextra -Wpedantic -O2 -o logscan logscan.c
./logscan <file> <pattern> [-i]
```

## Design decisions & trade-offs

-   Case-sensitive search uses `strstr()` for simplicity and speed.
-   Case-insensitive search (`-i`) uses a small custom matcher and
    `tolower((unsigned char)c)` to avoid undefined behavior on platforms
    where `char` is signed.
-   Fixed line buffer (`MAX_LINE = 1024`): very long lines may be
    truncated (future improvement).

## License

MIT
