# CalculusOS

A minimal, from-scratch x86 kernel skeleton: no applications, just
drivers and a login system, exactly as requested.

**This has been compiled and linked successfully in a clean sandbox**
(gcc 13, `-m32 -ffreestanding`, `ld -m elf_i386`) — the multiboot2
header, C code, and assembly all build without errors. Producing a
bootable `.iso` and running it needs two extra tools (`grub-mkrescue`,
`qemu`) that weren't available in that sandbox, so that final step is
untested by me — see "Build" below.

## What's included

| Piece | File(s) | What it does |
|---|---|---|
| Bootloader glue | `src/boot.s` | Multiboot2 header. Tells GRUB to switch the graphics card into a linear-framebuffer **true-color VESA mode** (requests 1280×1024×32) before handing control to the kernel. |
| VESA / framebuffer driver | `src/framebuffer.c/h`, `src/multiboot.c/h`, `src/font8x8.h` | Reads the mode GRUB set up, and gives you `fb_put_pixel` / `fb_put_string` for HD color graphics + text. |
| Keyboard driver | `src/keyboard.c/h` | PS/2 keyboard driver on IRQ1. **This is also your USB keyboard driver in practice** — see note below. |
| USB roadmap (not implemented) | `src/usb_stub.c` | Honest placeholder explaining what a *native* USB (xHCI) driver would require, and why it's out of scope for a "ready to build" skeleton. |
| Login system | `src/login.c/h` | Draws a login screen on the framebuffer, reads username/password from the keyboard driver, loops until correct. |
| CPU setup | `src/gdt.c`, `src/gdt_flush.s`, `src/idt.c/h`, `src/isr.s` | Flat GDT, IDT, PIC remap, IRQ dispatch — the plumbing everything else needs. |
| Kernel entry | `src/kernel.c` | Wires it all together. No app loading, no shell — just drivers + login, then halts. |
| Build system | `Makefile`, `src/linker.ld`, `iso/boot/grub/grub.cfg` | Compiles, links, and (optionally) packages a bootable ISO. |

Default login: **username `admin`, password `calculus`** — change the
`CALC_USER`/`CALC_PASS` defines at the top of `src/login.c`.

## About "USB keyboard driver"

Writing a *native* USB driver (PCI enumeration, xHCI ring buffers,
device enumeration, HID report parsing) is its own multi-thousand-line
subsystem — a separate project, not something that fits in a "drivers
+ login" skeleton. In practice, every real PC's firmware has **USB
Legacy Support**, which makes any attached USB keyboard appear to
software as a plain PS/2 device on ports 0x60/0x64. That's on by
default, works in QEMU/VirtualBox/VMware and on real hardware, and is
literally how BIOS setup screens, DOS, and most hobby OSes "support"
USB keyboards. `src/keyboard.c` drives that. `src/usb_stub.c` is left
in the tree as the honest placeholder for real xHCI work later.

## Build

You need a compiler that can target 32-bit freestanding x86. Either
works:
- Your distro's regular `gcc`/`ld` with `-m32` (what I tested with), **or**
- An `i686-elf` cross-compiler (the traditional OSDev.org approach) — if
  you use one, change `CC`/`LD` at the top of the `Makefile` accordingly.

```bash
# 1. Build the kernel binary (build/calculusos.bin)
make

# 2. Build a bootable ISO (needs: grub-mkrescue, xorriso)
make iso

# 3. Run it in an emulator (needs: qemu)
make run
```

On Debian/Ubuntu, the extra tools are:
```bash
sudo apt install grub-pc-bin grub-common xorriso qemu-system-x86
```

To boot on real hardware: `make iso` produces `build/CalculusOS.iso`
— write it to a USB stick with `dd` or Rufus/Balena Etcher and boot
from it.

## What's deliberately NOT here

No shell, no filesystem, no process scheduler, no apps — you asked for
drivers + login only, so that's the whole kernel. `kernel_main()` in
`src/kernel.c` is the obvious place to start adding more once you're
ready.
