# OS-project-1-2

This repository contains the OS-project-1-2. Run the Makefile to compile the code. The executable for the shell is generated as `shell`.

## Shell Server Test

To test the shell server:

1. Run `serverCore` with arguments:
   ```bash
   ./serverCore <ip_address> <port>
   ```

> **Note** or run following command:
   ```bash
   ./shell <Port>
   ```

2. In a separate terminal, run a client-side service to connect to the server.

   ```bash
   telnet localhost <port>
   ```
> **Note** in this case , your server should be run on localhost,

   ```bash
   nc <ip_address> <port>
   ```

> **Note:** You may need to run `make` to compile the executables.