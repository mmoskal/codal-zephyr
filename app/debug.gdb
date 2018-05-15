target remote | openocd -f debug.cfg
define rst
  monitor reset halt
  continue
end
