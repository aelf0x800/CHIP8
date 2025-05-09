module main

import os

fn main() {
	if os.args.len != 2 {
		eprintln("usage: CHIP8 [ROM]")
		exit(1)
	}

	mut interpreter := Interpreter.new(os.args[1])
	interpreter.run()
}
