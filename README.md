# What is this

Yet another CHIP-8 emulator.
Written with C with help of [raylib](https://github.com/raysan5/raylib) as window and graphics.

# How to run (Windows)

Download the raylib libraries fit for your system/compiler in [raylib](https://github.com/raysan5/raylib/releases) and drop it in the "lib" and "include" directory.

You might need to add `-Wl,--defsym,stat64i32=_stat64` in the Makefile FLAGS variable if you are using windows. [see more about this error](https://github.com/raysan5/raylib/discussions/5971)

```console
Make

./main rom_name.ch8
```
Enjoy!

Future Goals:
- [x] Sound
- [ ] Add quirk configurations
- [ ] Pixel Fade out screen effect to reduce flickering
- [ ] Debug Functionality
    - [ ] Stack
    - [ ] All Registers
    - [ ] Instructions scroller
- [ ] SUPER-CHIP
- [ ] Other Keyboard-layout Configs
