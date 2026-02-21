# logscan-c

A tiny log pattern scanner written in C. It searches for a substring in
a text file and prints matching lines with line numbers. Designed for
quick log triage during security investigations.

## Features

-   Substring search (minimal `grep`-like behavior)
-   Optional case-insensitive mode (`-i`) using a simple
    ASCII/byte-based comparison
-   Optional column indicator (`--col`) showing match position with `^`
-   Optional count-only mode (`--count`) for scripting/automation
-   Prints matching lines prefixed with `[line_number]`
-   Prints total match count at the end (unless `--count` is used)

## Usage

``` bash
logscan <file> <pattern> [-i] [--col] [--count]
```

### Options

-   `-i`\
    Case-insensitive search (ASCII-based).

-   `--col`\
    Prints a second line with `^` characters under the matched
    substring.

-   `--count`\
    Suppresses line output and prints only the total number of matches.

### Examples

Case-sensitive:

``` bash
logscan auth.log "Failed password"
```

Case-insensitive:

``` bash
logscan auth.log "failed password" -i
```

Show match column:

``` bash
logscan auth.log "Failed" --col
```

Count matches only (useful in scripts):

``` bash
logscan auth.log "error" --count
```

## Output

Default mode:

    [123] original log line here
    Total matches: N

With `--col`:

    [123] original log line here
          ^^^^^
    Total matches: N

With `--count`:

    5

## Build

### Windows 11 (MSVC `cl`)

Open the "Developer Command Prompt for VS" and run:

``` bat
cl /std:c11 /W4 /O2 logscan.c
```

Run:

``` bat
logscan.exe <file> <pattern> [-i] [--col] [--count]
```

### Windows 11 (clang)

``` bat
clang -std=c11 -Wall -Wextra -Wpedantic -O2 -o logscan.exe logscan.c
```

### Linux/macOS (clang or gcc)

``` bash
cc -std=c11 -Wall -Wextra -Wpedantic -O2 -o logscan logscan.c
./logscan <file> <pattern> [-i] [--col] [--count]
```

## Design decisions & trade-offs

-   Case-sensitive search uses `strstr()` for simplicity and speed.
-   Case-insensitive search (`-i`) uses a small custom matcher and
    `tolower((unsigned char)c)` to avoid undefined behavior when `char`
    is signed.
-   `--count` provides a stable, machine-friendly output for scripting.
-   Fixed line buffer (`MAX_LINE = 1024`): very long lines may be
    truncated (future improvement).

## License

MIT
