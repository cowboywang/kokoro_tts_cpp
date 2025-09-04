#pragma once

class CWaveWriter
{
public:
	CWaveWriter(const char* file_name, int sample_rate, int channels);
	~CWaveWriter();

public:
	void WritePcm(shared_ptr<vector<float>> pcm);

private:
	typedef struct _WaveHeader {
		char riff_tag[4];
		int riff_length;
		char wave_tag[4];
		char fmt_tag[4];
		int fmt_length;
		short audio_format;
		short num_channels;
		int	sample_rate;
		int	byte_rate;
		short block_align;
		short bits_per_sample;
		char data_tag[4];
		int	data_length;
	}WaveHeader;

private:
	FILE* m_wave_file;
	WaveHeader m_wave_header;
};