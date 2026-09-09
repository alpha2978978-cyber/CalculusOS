/* ============================================================
 * CalculusOS - usb_stub.c
 *
 * This is NOT a working USB controller driver. It exists so the
 * intended location of one is obvious in the tree, and so you
 * know exactly what's involved if you want to go past PS/2-
 * emulated USB keyboards (which keyboard.c already handles for
 * essentially every real machine and every emulator).
 *
 * A real, native USB HID keyboard driver needs, at minimum:
 *   1. PCI enumeration to find the USB host controller (xHCI is
 *      the modern standard; UHCI/EHCI are older).
 *   2. Mapping the controller's MMIO registers and bringing the
 *      controller out of BIOS ownership ("handoff").
 *   3. Building command rings / event rings (xHCI) or frame
 *      lists (UHCI) in memory the controller DMAs into.
 *   4. Device enumeration: reset port, get descriptors, assign
 *      address, read the HID report descriptor.
 *   5. A HID report parser to turn raw input reports into key
 *      up/down events.
 *
 * That's several thousand lines and weeks of debugging against
 * real hardware/QEMU on its own - it's a separate project. This
 * file is intentionally left as a stub/roadmap rather than a
 * fake driver that would silently do nothing.
 * ============================================================ */

void usb_init(void) {
    /* TODO: xHCI controller discovery + init would start here. */
}
