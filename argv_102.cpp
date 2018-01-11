#include "argv_103.hpp"


using namespace std;

bool game_over = false;


DWORD WINAPI argv_143 (LPVOID lParam) {
	argv_108 * param_ptr = (argv_108 *) lParam;
	uint32_t output_size = param_ptr->argv_144.size ();
	uint32_t data;
	uint64_t i;
	bool argv_120;
	argv_107 bench;
	bench.argv_139 ();
	uint32_t nb_found = 0;

	for (i=param_ptr->argv_139 ; i<param_ptr->argv_140 ; i++) {
		if (game_over == true) {
			break;
		}
		srand ((uint32_t)i);
		argv_120 = true;
		for (uint32_t k=0 ; k<output_size ; k++) {
			data = rand ();
			if (data != param_ptr->argv_144[k]) {
				argv_120 = false;
				break;
			}
		}

		if (argv_120 == true) {
			bench.argv_140 ();
			string elapsed_str = bench.argv_141 ("thread");
			fprintf (stderr, "*** thread %d argv_120 : seed=%u (elapsed:%s)\r\n", param_ptr->argv_127, (uint32_t) i, elapsed_str.c_str());
			bench.argv_139 ();
			nb_found++;
		}
	}

	if (nb_found == 0) {
		fprintf (stderr, "thread %d finished (not argv_120)\n", param_ptr->argv_127);
	}
	else {
		char tmp[MAX_PATH];
		sprintf (tmp, "thread %d finished (%u argv_120)\n", param_ptr->argv_127, nb_found);
		fprintf (stderr, tmp);
	}
	return (0);
}

uint32_t argv_126() {
	SYSTEMTIME tim;
	GetLocalTime(&tim);
	return ((uint32_t)__rdtsc());
}

void argv_123 (uint32_t NbValuesToGenerate) {
	// set randomly the PRG.
	uint32_t ChosenRandomSeed = argv_126();
	srand (ChosenRandomSeed);
	fprintf (stderr, "chosen random seed : %u\n", ChosenRandomSeed);

	fprintf(stderr, "Random values generated by this seed:\n");
	uint32_t rand_data;
	for (uint32_t i=0 ; i<NbValuesToGenerate ; i++) {
		rand_data = rand ();
		fprintf(stderr, "%u ", rand_data);
	}
	fprintf(stderr, "\n");
}

uint32_t argv_106 (uint32_t i_NbThread, const std::vector<uint32_t>& i_VectorRandVal) {
	uint32_t iret = 0;
	uint32_t i;
	HANDLE			hThread; 
	vector<HANDLE>	v_handle;
	DWORD			dwThreadId;
	argv_108 * param_ptr = NULL;

	uint32_t NbPerThread = UINT_MAX / i_NbThread;
	fprintf(stderr, "Launching attack with %u threads (Each one will explore %u elements)\n", i_NbThread, NbPerThread);
	argv_107 bench;
	bench.argv_139();

	uint64_t NbPrepared = 0;
	for (i=0 ; i<i_NbThread ; i++) {
		param_ptr = new (std::nothrow) argv_108;
		if (param_ptr == NULL) {
			fprintf(stderr, "+++ Error: mem alloc failed\n");
			iret = 1;
			break;
		}
		
		// prepare the parameter
		param_ptr->argv_127				= i;
		param_ptr->argv_144	= i_VectorRandVal;

		param_ptr->argv_139 = NbPrepared;
		if ((NbPrepared + NbPerThread) < UINT_MAX) {
			param_ptr->argv_140 = NbPrepared + NbPerThread;
		}
		else {
			param_ptr->argv_140 = UINT_MAX - NbPrepared;
		}
		NbPrepared += NbPerThread;
		fprintf (stderr, "thread %d: [%u..%u]\n", i, (uint32_t) param_ptr->argv_139, (uint32_t) param_ptr->argv_140);

		hThread = CreateThread (NULL, 0, argv_143, (void *) param_ptr, 0, &dwThreadId);					
		if (hThread == NULL)   {
			fprintf(stderr, "+++ Error: Thread creation failed\n");
			iret = 2;
			break;
		}
		v_handle.push_back (hThread);
	}

	for (i=0 ; i<i_NbThread ; i++) {
		WaitForSingleObject (v_handle[i], INFINITE);
		fprintf (stderr, "thread %d done\n", i);
	}
	string elapsed_str = bench.argv_141 ("main");
	fprintf (stderr, "[ALL DONE] %s\n", elapsed_str.c_str());
	return (iret);
}

void argv_115() {
	fprintf(stderr, "|===============================================|\n");
	fprintf(stderr, "| Beemeal stdlib rand breaker (V1.0.0)          |\n");
	fprintf(stderr, "|===============================================|\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Mode 1: i want to find the seed that generated the <RandVal_0>...<RandVal_N> sequence:\n");
	fprintf(stderr, "    Usage: ./FindRandSeed.exe --break <NbThread> <RandVal_0>...<RandVal_N>\n");
	fprintf(stderr, "Mode 2: i want to generate a random sequence to test the '--break' mode later:\n");
	fprintf(stderr, "     Usage: ./FindRandSeed.exe --generate-rand-sequence <RandSequenceSize>\n");
	fprintf(stderr, "     Notice: <RandSequenceSize> minimum is 10\n");
}

uint32_t main (int32_t _argc, char ** _argv) {
	uint32_t iret = 0;
	if (_argc < 3) {
		argv_115();
		iret = 1;
	}
	else {
		if (0 == strcmp(_argv[1], "--break")) {
			int32_t NbThread = atoi(_argv[2]);
			if (NbThread < 1) {
				fprintf(stderr, "+++ ERROR: Minumum number of thread is 1, got %d\n", NbThread);
				iret = 3;
			}
			else {
				std::vector<uint32_t>	VectorRandVal;
				for (int32_t i = 3 ; i < _argc; i++) {
					VectorRandVal.push_back(atoi(_argv[i]));
				}
				iret = argv_106(NbThread, VectorRandVal);
			}
		}
		else if (0 == strcmp(_argv[1], "--generate-rand-sequence")) {
			if (_argc != 3) {
				argv_115();
				iret = 1;
			}
			else {
				int32_t RandSequenceSize = atoi(_argv[2]);
				if (RandSequenceSize < 10) {
					iret = 1;
					fprintf(stderr, "+++ Error: RandSequenceSize too small, minimum is 10, got %d\n", RandSequenceSize);
				}
				else {
					argv_123(RandSequenceSize);
				}
			}
		}
	}
	return (iret);
}

