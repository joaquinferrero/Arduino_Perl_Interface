#!/usr/bin/env perl
use Device::SerialPort::Arduino;

my $Arduino = Device::SerialPort::Arduino->new(
    port	=> '/dev/ttyACM0',
    baudrate	=> 115200,
    databits	=> 8,
    parity	=> 'none',
);

if (@ARGV) {
    for my $orden (@ARGV) {				# órdenes en línea de comandos
	sleep 1;

	$Arduino->communicate($orden);
    }
}
else {			
    say "Write END to finsh";				# órdenes directas

    while (my $orden = <>) {

	last if $orden =~ /^end/i;			# comando de salida

	$Arduino->communicate($orden);
    }
}
