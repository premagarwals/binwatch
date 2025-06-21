# Binwatch

**Binwatch** is a Linux command-line tool to execute and monitor binaries with resource limits, sandboxing, and live memory usage visualization.

---

## Features

- **Resource Limiting:**  
  Limit the maximum memory (in KB) and execution time (in seconds) for any command.
- **Sandboxing:**  
  Drop privileges and optionally run the command in a chroot jail for extra security.
- **Live Memory Monitoring:**  
  Collects memory usage samples and displays a simple ASCII graph of memory usage after execution.
- **Logging:**  
  Execution details are logged to `log.txt`.

---

## Usage

```sh
binwatch [--max-mem <KB>] [--max-time <sec>] -exe <command> [args...]
sudo binwatch [--max-mem <KB>] [--max-time <sec>] [--sandbox] -exe <command> [args...]
sudo binwatch [--max-mem <KB>] [--max-time <sec>] [--chroot <path>] -exe <command> [args...]
```

### Options

- `--max-mem <KB>`, `-m <KB>`: Set maximum memory usage in kilobytes (default: 32768 KB).
- `--max-time <sec>`, `-t <sec>`: Set maximum execution time in seconds (default: 10).
- `--sandbox`, `-s`: Drop privileges and run in a restricted environment.
- `--chroot <path>`, `-c <path>`: Run in a chroot jail at the specified path.
- `--help`, `-h`: Show usage information.
- `-exe <command> [args...]`: The command and its arguments to execute and monitor.

### Example

```sh
binwatch --max-mem 16384 --max-time 5 -exe ls -l
sudo binwatch --max-mem 8192 --sandbox -exe ./myprog
sudo binwatch --max-mem 4096 --chroot /tmp/sandbox -exe python3 script.py
```

---

## Output

After execution, you will see a summary like:

```
:::::::::::::::::::: EXECUTION SUMMARY :::::::::::::::::::

PID: 12345
Runtime: 2.01s
Peak Memory: 2048 KB
Exit Code: 0
Killed: false

Memory usage graph (in KB):
|██████████████████████████████████████████████████| 2048 KB
|██████████████████████████████████████████████████| 2048 KB
|██████████████████████████████████████████████████| 2048 KB
...
```

---

## Building

```sh
make
```

Requires a C++17 compiler and Linux.

---

## Notes

- For sandboxing and chroot, you must run as root (`sudo`).
- The memory limit uses `RLIMIT_AS` (virtual memory); enforcement depends on your Linux kernel settings.
- All execution details are logged to `log.txt` in the current directory.

---

## License

MIT License

```
MIT License

Copyright (c) 2025 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## Author

[Prem Agarwal](https://github.com/premagarwals)
