# Project1

## Build Instructions

1. **Compile all `.c` source files:**
   ```bash
   make
   ```
2. **Clean up executables:**
   ```bash
   make clean
   ```

---

## Copy

- **MyCopy.c**  
- **ForkCopy.c**  
- **PipeCopy.c**

### Benchmark Configurations and Programs

These are located in the `/benchmark` and `/benchmark_timespec` directories. They use different timing methods, and each directory contains a Python script to visualize performance.

#### Running Benchmarks

1. Navigate to the relevant benchmark directory (e.g., `/Project1/copy/benchmark`):
   ```bash
   ./benchmark
   ```
2. The results will be printed directly to the terminal.

### Run Programs

```bash
./MyCopy (or ForkCopy, PipeCopy) <InputFile> <OutputFile>
```
or
```bash
./MyCopy (or ForkCopy, PipeCopy) <InputFile> <OutputFile> <bufferSize>
```

---

# shell

## Run a Single Shell

1. Navigate to the `/shell` directory:
   ```bash
   cd /Project1/shell
   ```
2. Run the shell program:
   ```bash
   ./shell
   ```
   
## Run Shell in a Server

### Start Server Core

- **Default mode (localhost)**  
  ```bash
  ./severCore <Port>
  ```
  This assigns the server to `127.0.0.1` (localhost).

- **Custom host mode**  
  ```bash
  ./severCore <IP_Address> <Port>
  ```
  This assigns the server to the specified IP address and port.

### Connect to the Server

Use Telnet to connect to the server:
```bash
telnet <IP_Address> <Port>
```
The server supports multiple clients in parallel.

---

## Sort

- **MergesortSingle.c**  
- **MergesortMulti.c**

### How to run

- **For MergeSortSingle**  
  ```bash
  ./MergesortSingle
  ```
- **For MergesortMulti**  
  ```bash
  ./MergesortMulti <MaxThreads>
  ```

### Benchmark

Similar to the benchmark in **copy**:
1. Navigate to `/Project1/sort/benchmark`.
2. Run `benchmark.py`.
