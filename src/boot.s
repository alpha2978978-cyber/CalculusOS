/* ============================================================
 * CalculusOS - boot.s
 * Multiboot2 header + 32-bit entry point.
 *
 * We ask GRUB (the bootloader) to switch the video card into a
 * linear framebuffer VESA mode for us BEFORE our kernel runs.
 * That is what gives us "VESA HD color" without having to write
 * our own real-mode VBE code. GRUB does the BIOS int 0x10 work.
 * ============================================================ */

.set MAGIC,        0xE85250D6      /* multiboot2 magic */
.set ARCH,         0               /* i386 protected mode */
.set HDR_LEN,      (header_end - header_start)
.set CHECKSUM,     -(MAGIC + ARCH + HDR_LEN)

.section .multiboot
.align 8
header_start:
    .long MAGIC
    .long ARCH
    .long HDR_LEN
    .long CHECKSUM

    /* --- framebuffer tag: request a linear graphics mode --- */
    .align 8
fb_tag_start:
    .short 5            /* type = framebuffer */
    .short 0            /* flags */
    .long  fb_tag_end - fb_tag_start
    .long  1280         /* preferred width  */
    .long  1024         /* preferred height */
    .long  32           /* preferred bits per pixel (true color) */
fb_tag_end:

    /* --- end tag --- */
    .align 8
    .short 0
    .short 0
    .long 8
header_end:

/* ------------------------------------------------------------
 * Stack (16 KB)
 * ------------------------------------------------------------ */
.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

/* ------------------------------------------------------------
 * Entry point. GRUB leaves us in 32-bit protected mode with:
 *   eax = 0x36d76289 (multiboot2 bootloader magic)
 *   ebx = physical address of the multiboot2 info structure
 * ------------------------------------------------------------ */
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp
    push %ebx           /* multiboot info pointer -> arg 2 */
    push %eax           /* multiboot magic         -> arg 1 */
    call kernel_main
    cli
hang:
    hlt
    jmp hang
.size _start, . - _start
