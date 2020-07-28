midistar2mid: main.o midi_container.o midi_processor_helpers.o midi_processor_mids.o midi_processor_standard_midi.o midi_processor_hmi.o midi_processor_gmf.o midi_processor_mus.o midi_processor_lds.o midi_processor_riff_midi.o midi_processor_syx.o midi_processor_xmi.o midi_processor_hmp.o
	g++ -o $@ $^

clean:
	rm -f *.o
