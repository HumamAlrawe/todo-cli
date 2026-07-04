# todo 📋

A command-line task manager written in C. Tasks persist between sessions using a local binary file — no dependencies, no setup, just compile and run.

## Demo

```
=== Todo CLI ===

  a  Add task
  l  List tasks
  c  Complete task
  d  Delete task
  x  Clear completed
  q  Quit

> l

  ID    Status  Title
  ----  ------  ------------------------------
  1     [ -- ]  Buy groceries
  2     [done]  Finish assignment
  3     [ -- ]  Push code to GitHub
```

## Build & Run

```bash
gcc -o todo todo.c
./todo        # Linux / macOS
todo.exe      # Windows
```

Requires only a C compiler (GCC, Clang, or MSVC).

## Features

- Add tasks with a title
- List all tasks with status
- Mark tasks as complete
- Delete individual tasks
- Clear all completed tasks at once
- Tasks saved to `tasks.dat` — persists between runs

## What this demonstrates

- File I/O in C (`fread` / `fwrite` with binary format)
- Structs and arrays as a simple in-memory database
- Input handling and menu-driven CLI design
- Manual memory layout without external libraries

## License

MIT
