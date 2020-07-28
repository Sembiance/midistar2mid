#include <string>
#include <fstream>
#include <cstring>

#include "midi_processor.h"

#define MIDSTAR2MID_VERSION "1.0.0"

static void usage(void)
{
    fprintf(stderr,
            "mod2wav %s\n"
            "\n"
            "Usage: midstar2mid <input> <output.mid>\n"
            "  -h, --help              Output this help and exit\n"
            "  -V, --version           Output version and exit\n"
            "\n", MIDSTAR2MID_VERSION);
    exit(EXIT_FAILURE);
}

char * inputFilePath=0;
char * outputFilePath=0;

static void parse_options(int argc, char **argv)
{
    int i;

    for(i=1;i<argc;i++)
    {
        int lastarg = i==argc-1;

        if(!strcmp(argv[i],"-h") || !strcmp(argv[i], "--help"))
        {
            usage();
        }
        else if(!strcmp(argv[i],"-V") || !strcmp(argv[i], "--version"))
        {
            printf("midstar2mid %s\n", MIDSTAR2MID_VERSION);
            exit(EXIT_SUCCESS);
       	}
       	else
		{
            break;
       	}
    }

    argc -= i;
    argv += i;

    if(argc<2)
		usage();

    inputFilePath = argv[0];
    outputFilePath = argv[1];
}


int main(int argc, char *argv[])
{
	parse_options(argc, argv);

	char * extLoc = strrchr(inputFilePath, '.')+1;
	char ext[strlen(extLoc)+1];
	strncpy(ext, extLoc, strlen(extLoc));
	char * s = ext;
	while(*s)
	{
		*s = toupper(*s);
		s++;
	}

	std::ifstream inputFileStream(inputFilePath, std::ios::in | std::ios::binary);
	std::vector<uint8_t> inputFileVector((std::istreambuf_iterator<char>(inputFileStream)), std::istreambuf_iterator<char>());

	midi_container mc;
	midi_processor mp;
	if(mp.process_file(inputFileVector, ext, mc)==0)
	{
		fprintf(stderr, "Failed to process file %s\n", inputFilePath);
		exit(EXIT_FAILURE);
	}
	mc.promote_to_type1();

	std::vector<uint8_t> outputFileVector;
	mc.serialize_as_standard_midi_file(outputFileVector);
	std::ofstream outFile(outputFilePath);
    for(const auto &e : outputFileVector) outFile << e;

	return EXIT_SUCCESS;
}
