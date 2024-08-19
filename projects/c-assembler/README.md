# `C Assembler`

[![Trello Board](https://img.shields.io/badge/Trello-Main_Board-026AA7?logo=trello)](https://trello.com/b/LkRTtcwC/trello-agile-sprint-board-template)

## Description

This project is a simple assembler written in C. It reads a file with assembly code and translates it into machine code.

### NOTE 📝

Running this project requires a Unix-like environment. It was tested on Ubuntu 20.04.
It might work on some Windows environments, but it is not guaranteed.

## Compilation

### With Nx

-   Run `npx nx run c-assembler:build`

### With Make

-   Run `make -C {projectRoot}/src/ target='$(PWD)/dist/c-assembler' name=c-assembler`

## Run with arguments

```bash
nx run c-assembler:run --args="--input projects/c-assembler/assets/with-macros"
```

## Testing

### Manual Testing

-   Run `npx nx run c-assembler:run`
-   Enter the path to the file with assembly code, for example `--args="--input projects/c-assembler/assets/with-macros"`

### With `NodeJS` test-bed:

-   Run `npx nx run c-assembler:test`
-   See results in the console

## Screenshots

![Test Bed](screenshots/test-bed.png)

![Output Files](screenshots/output-files.png)
