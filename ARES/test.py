from pwn import *

context.log_level = 'debug'

p = remote("ares.beginners.seccon.games", 5000)

p.recvuntil(b"> ")

p.sendline(b"1")

p.interactive()
