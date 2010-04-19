/* Lev Panov, 18 april 2010
 * Festival text-to-speech engine api test program */
/*
 * compile with:
 * g++ -o festival-test -DSTANDALONE festival-test.cpp  /usr/lib64/libFestival.a  /usr/lib64/libestools.a /usr/lib64/libestbase.a  /usr/lib64/libeststring.a -lcurses
 */

#include <stdio.h>
#include <festival/festival.h>

int main(void)
{
    EST_Wave wave;
    int heap_size = 210000;  // default scheme heap size
    int load_init_files = 1; // we want the festival init files loaded

    festival_initialize(load_init_files,heap_size);

    //festival_say_file("/etc/issue.text");

    //festival_eval_command("(voice_msu_ru_nsh_clunits)");
    //festival_say_text("а");
    
    festival_eval_command("(voice_ked_diphone)");
    festival_say_text("privet, suchka. blyahui");

    festival_text_to_wave("hello world",wave);
    wave.save("/tmp/wave.wav","riff");

    // festival_say_file puts the system in async mode so we better
    // wait for the spooler to reach the last waveform before exiting
    // This isn't necessary if only festival_say_text is being used (and
    // your own wave playing stuff)
    festival_wait_for_spooler();

    return 0;
}
