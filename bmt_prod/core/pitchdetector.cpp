#include "pitchdetector.h"

#define MAX_MSG_LEGTH 300

PitchDetector::PitchDetector() :
    bin_prev(-1), outputRate(OUTPUT_RATE)
{
	setSpectrumSize(SPECTRUM_SIZE);
}

PitchDetector::~PitchDetector()
{
	spectrum.clear();
}

void PitchDetector::setSpectrumSize(int size)
{
	spectrumSize = size;
	spectrum.resize(size);
	fill(spectrum.begin(), spectrum.end(), 0.0f);

	spectrumRange = ((float)outputRate / 2.0f);      /* 0 to nyquist */
	binSize = (spectrumRange / (float)spectrumSize);
}

void PitchDetector::update()
{
//	g_system.getSpectrum(spectrum.data(), spectrumSize);
	g_system.getSpectrum(&spectrum[0], spectrumSize);

	countDominantNote();
}

void PitchDetector::countDominantNote()
{
	vector<float>::const_iterator it = max_element(spectrum.begin(), spectrum.end());
	bin = it - spectrum.begin(); //distance(spectrum.begin(), it);

	dominanthz  = (float)bin * binSize;       /* dominant frequency min */

	dominantnote = 0;
	for (int i = 0; i < NUM_NOTES; i++)
	{
		 if (dominanthz >= notefreq[i] && dominanthz < notefreq[i + 1])
		 {
			/* which is it closer to.  This note or the next note */
			if (fabs(dominanthz - notefreq[i]) < fabs(dominanthz - notefreq[i+1]))
			{
				dominantnote = i;
			}
			else
			{
				dominantnote = i + 1;
			}
			break;
		 }
	}

	//print current dominant note info
//	if (bin != bin_prev)
//	{
//		char msg[MAX_MSG_LEGTH];
//		snprintf(msg, MAX_MSG_LEGTH, "Time = %6d // Detected rate : %7.1f -> %7.1f hz.  Detected musical note. %-3s (%7.1f hz)\r",
//				 g_system.getTime(), dominanthz, ((float)bin + 0.99f) * binSize, note[dominantnote], notefreq[dominantnote]);
//		g_debug << msg << endl;
//	}
//	bin_prev = bin;
}
