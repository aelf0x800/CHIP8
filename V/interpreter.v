module main

import os

const program_off = 0x512

struct Instruction {
	op  u8
	x   u8
	y   u8
	nnn u16
	nn  u8
	u   u8
}

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
	mut self := Interpreter{}

	// Initialise program counter
	self.pc = program_off

	// Load the ROM
	rom := os.read_file(path) or {
		eprintln("Failed to open the ROM")
		exit(1)
	}
	for i, data in rom {
		self.memory[self.pc + i] = data
	}

	// Load the font
	
	return self
}

fn (mut self Interpreter) run() {
	for self.pc != 0x0 {
		ins := self.decode()
		self.execute(ins)
	}
}

fn (mut self Interpreter) decode() Instruction {
	self.pc += 2
	return Instruction{}
}

fn (mut self Interpreter) execute(ins Instruction) {
	self.err("test")
	match ins.op {
		0x0 { }
		0x1 { }
		0x2 { }
		0x3 { }
		else { }
	}
}

fn (self Interpreter) err(msg string) {
	// Print the registers
	eprintln("=| Registers |=============================")
	eprintln("PC: #${self.pc:X}\nSP: #${self.sp:X}\nI: #${self.i:X}")
	for i, v in self.v {
		eprintln("V${i} : #${v:X}")
	}

	// Print the instruction it failed on
	eprintln("=| Instruction |===========================")
	eprintln("TODO")

	// Print the surrounding memory of the error
	eprintln("=| Memory |================================")
	eprintln("TODO")

	// Print the message
	eprintln("=| \x1b[1;31mERROR\x1b[0m |=================================")
	eprintln(msg)

	exit(1)
}
