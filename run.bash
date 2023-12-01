#!/bin/bash

qemu-system-x86_64 -boot d -cdrom os.iso -m 64M $1
