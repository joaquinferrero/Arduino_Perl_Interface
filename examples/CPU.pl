#!/usr/bin/env perl
use v5.20;
use autodie;
use Device::SerialPort::Arduino;

my $Arduino = Device::SerialPort::Arduino->new(
    port	=> '/dev/ttyACM0',
    baudrate	=> 115200,
    databits	=> 8,
    parity	=> 'none',
);

sleep 2;

my $salida;
my $msg;
my $anterior = '';

while (1) {
    open my $LOAD, '<', '/proc/loadavg';
    my ($carga) = (split " ", <$LOAD>)[0];
    close $LOAD;

    $carga = 2 * int $carga;

    if ($carga >= 8) {
	$carga -= 8;
	$carga++;
    }

    if ($anterior ne $carga) {

	$salida = '';

	for (my $i = 0; $i < 4; $i++) {

	    if (vec($carga, $i, 1) != vec($anterior, $i, 1)) {

		if (vec $carga, $i, 1) {
		    $msg = "on  "  . (4+$i);
		    $Arduino->communicate("$msg\n");
		}
		else {
		    $msg = "off "  . (4+$i);
		    $Arduino->communicate("$msg\n");
		}

		$salida .= "[$msg]";
	    }
	}

	say sprintf "%04b $salida", $carga;

	$anterior  = $carga;
    }

    sleep 3;
}
