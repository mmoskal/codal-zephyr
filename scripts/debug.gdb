target remote | ./scripts/openocd.sh -c 'gdb_port pipe; gdb_memory_map disable'
define rst
  monitor reset halt
  continue
end
