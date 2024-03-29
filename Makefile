############################################################################
# Makefile
#
#   Copyright (C) 2012, 2018 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

# This is a top-level "kludge" Makefile that just includes the correct
# Makefile.  If you already know the Makefile that you want, you can skip
# this nonsense using:
#
#   make -f Makefile.unix, OR
#   make -f Makefile.win
#

.PHONY: update

-include .config
ifeq ($(CONFIG_WINDOWS_NATIVE),y)
include tools/Makefile.win
else
include tools/Makefile.unix
endif

update:
	openocd -f /usr/local/share/openocd/scripts/interface/stlink-v2.cfg  -f /usr/local/share/openocd/scripts/target/stm32f4x_stlink.cfg -c init -c halt -c "flash write_image erase nuttx.bin 0x08000000" -c reset -c shutdown
#	openocd -f /usr/local/share/openocd/scripts/interface/stlink-v2.cfg  -f /usr/local/share/openocd/scripts/target/stm32f1x_stlink.cfg -c init -c halt -c "flash write_image erase nuttx.bin 0x08000000" -c reset -c shutdown
#	openocd -f /usr/local/share/openocd/scripts/interface/jlink.cfg  -f /usr/local/share/openocd/scripts/target/stm32f4x.cfg -c init -c "reset halt" -c "flash write_image erase nuttx.bin 0x08000000" -c reset -c shutdown
#	st-flash write nuttx.bin 0x08000000

