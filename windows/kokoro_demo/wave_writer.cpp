#include "framework.h"

static const char* RIFF_TAG = "RIFF";
static const char* WAVE_TAG = "WAVE";
static const char* FMT_TAG = "fmt ";
static const char* DATA_TAG = "data";

CWaveWriter::CWaveWriter(const char* file_name, int sample_rate, int channels)
: m_wave_file(nullptr)
{
	m_wave_file = fopen(file_name, "wb");

	strncpy(m_wave_header.riff_tag, RIFF_TAG, 4);
	m_wave_header.riff_length = 0;
	strncpy(m_wave_header.wave_tag, WAVE_TAG, 4);
	strncpy(m_wave_header.fmt_tag, FMT_TAG, 4);
	m_wave_header.fmt_length = 16;
	m_wave_header.audio_format = WAVE_FORMAT_PCM;
	m_wave_header.num_channels = channels;
	m_wave_header.sample_rate = sample_rate;
	m_wave_header.byte_rate = sample_rate * 2;
	m_wave_header.block_align = channels * 2;
	m_wave_header.bits_per_sample = 16;
	strncpy(m_wave_header.data_tag, DATA_TAG, 4);
	m_wave_header.data_length = 0;
}

CWaveWriter::~CWaveWriter()
{
	if (m_wave_file) {
		fclose(m_wave_file);
		m_wave_file = nullptr;
	}
}

void CWaveWriter::WritePcm(shared_ptr<vector<float>> pcm)
{
	if (m_wave_file) {
		m_wave_header.data_length = pcm->size() * 2;
		m_wave_header.riff_length = m_wave_header.data_length + sizeof(WaveHeader) - 8;

		fwrite(&m_wave_header, 1, sizeof(WaveHeader), m_wave_file);

		for (int i = 0; i < pcm->size(); ++i) {
			int16_t value = (*pcm)[i] * 32767;
			fwrite(&value, 1, sizeof(int16_t), m_wave_file);
		}
	}
}