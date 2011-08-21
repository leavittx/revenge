#include "pitchdetector.h"

#define MAX_MSG_LEGTH 300

PitchDetector::PitchDetector()
{
	OUTPUTRATE = 48000;
	setSpectrumSize(SPECTRUM_SIZE);

	bin_prev = -1;
}

PitchDetector::~PitchDetector()
{
	spectrum.clear();
}

void PitchDetector::setSpectrumSize(int size)
{
	SPECTRUMSIZE = size;
	spectrum.resize(size);
	fill(spectrum.begin(), spectrum.end(), 0.0f);

	SPECTRUMRANGE = ((float)OUTPUTRATE / 2.0f);      /* 0 to nyquist */
	BINSIZE = (SPECTRUMRANGE / (float)SPECTRUMSIZE);
}

void PitchDetector::update()
{
//	g_system.getSpectrum(spectrum.data(), SPECTRUMSIZE);
	g_system.getSpectrum(&spectrum[0], SPECTRUMSIZE);

	countDominantNote();
}

void PitchDetector::countDominantNote()
{
//	float max = 0;
//	for (int i = 0; i < SPECTRUMSIZE; i++)
//	{
//		if (spectrum[i] > 0.01f && spectrum[i] > max)
//		{
//			max = spectrum[i];
//			bin = i;
//		}
//	}

	vector<float>::const_iterator it = max_element(spectrum.begin(), spectrum.end());
	//	bin = distance(spectrum.begin(), it);
	bin = it - spectrum.begin();

	dominanthz  = (float)bin * BINSIZE;       /* dominant frequency min */

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
//				 g_system.getTime(), dominanthz, ((float)bin + 0.99f) * BINSIZE, note[dominantnote], notefreq[dominantnote]);
//		g_debug << msg << endl;
//	}
//	bin_prev = bin;
}

bool PitchDetector::matchFreq(float freq)
{
	if (notefreq[dominantnote] == freq)
		return true;
	return false;
}

bool PitchDetector::matchLowBassBeat()
{
//	return matchFreq(16.4);
	return notefreq[dominantnote] < 50;
}

bool PitchDetector::matchBassBeat()
{
//	return matchFreq(92.5);
	return notefreq[dominantnote] >= 50 &&
		   notefreq[dominantnote] < 100;
}

bool PitchDetector::matchHiBassBeat()
{
//	return matchFreq(185.0);
	return notefreq[dominantnote] >= 100 &&
		   notefreq[dominantnote] < 200;
}
