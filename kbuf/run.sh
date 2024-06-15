qemu-system-x86_64 \
     -m 64M \
     -nographic \
     -kernel bzImage \
     -initrd rootfs_updated.cpio \
     -drive file=flag.txt,format=raw \
     -snapshot \
     -append "console=ttyS0 loglevel=3 oops=panic panic=-1 pti=on nokaslr" \
     -no-reboot \
     -cpu qemu64,+smap,+smep \
     -monitor /dev/null \
     -net nic,model=virtio \
     -net user \
	 -gdb tcp::12345