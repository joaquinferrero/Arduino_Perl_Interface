#!/usr/bin/env perl
use v5.20;
use Time::HiRes 'sleep';
use Device::SerialPort::Arduino;

$|++;

my $Arduino = Device::SerialPort::Arduino->new(
    port	=> '/dev/ttyACM0',
    baudrate	=> 115200,
    databits	=> 8,
    parity	=> 'none',
);

sleep 2;				# esperar estabilización del puerto serie

my $buffer;

my $LAPSO = 0;

while (1) {				# ¡Blink!

say "ON";

    $Arduino->communicate("ON 5\n");
    recibe();
    sleep $LAPSO;

say "OFF";

    $Arduino->communicate("OFF 5\n");
    recibe();
    sleep $LAPSO;
}

sub recibe () {
    sleep 0.075;
    my $buffer ;
    while (my $char = $Arduino->{'DSP'}->lookfor()) {
        $buffer .= $char;
    }
    print "$buffer\n" if $buffer;
}
