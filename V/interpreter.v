module main

struct Interpreter {
mut:
	// Memory
	memory [2048]u8
	stack  [16]u16
	// Registers
	pc u16
	sp u8
	i  u16
	v  [16]u8
	// Timers
	delay_timer u8
	sound_timer u8
}

fn Interpreter.new(path string) Interpreter {
	// Initialise program counter
	// Load the ROM
	// Load the font
}
