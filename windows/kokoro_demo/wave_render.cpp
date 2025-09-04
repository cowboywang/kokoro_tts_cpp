#include "framework.h"

CWaveRender::CWaveRender()
: m_running(true)
, m_render_thread(nullptr)
{
	TTSLog("CWaveRender::CWaveRender()");

	m_render_thread = new thread(ThreadFun, this);
}

CWaveRender::~CWaveRender()
{
	if (m_render_thread) {
		m_running = false;
		m_render_thread->join();
		SAFE_DELETE(m_render_thread);
	}

	TTSLog("CWaveRender::~CWaveRender()");
}

void CWaveRender::RenderAudio(shared_ptr<vector<float>> pcm, int sample_rate, int channel)
{
	PcmItem item;
	item.pcm = pcm;
	item.sample_rate = sample_rate;
	item.channel = channel;

	m_mutex_pcm.lock();
	m_pcm_list.push_back(item);
	m_mutex_pcm.unlock();
}

void CWaveRender::ThreadFun(CWaveRender* wave_render)
{
	wave_render->DoRenderLoop();
}

void CWaveRender::DoRenderLoop()
{
	TTSLog("CWaveRender::DoRenderLoop(), Enter()...");

	while (m_running) {

		shared_ptr<vector<float>> pcm;
		int sample_rate = 0;
		int channel = 0;

		{
			m_mutex_pcm.lock();

			list<PcmItem>::iterator iter = m_pcm_list.begin();
			if (iter != m_pcm_list.end()) {
				pcm = (*iter).pcm;
				sample_rate = (*iter).sample_rate;
				channel = (*iter).channel;
				m_pcm_list.pop_front();
			}

			m_mutex_pcm.unlock();
		}

		if (!pcm.get()) {
			Sleep(1);
			continue;
		}

		shared_ptr<vector<int16_t>> pcm16(new vector<int16_t>(pcm->size()));
		for (int i = 0; i < pcm16->size(); ++i) {
			int16_t value = (*pcm)[i] * 32767;
			(*pcm16)[i] = value;
		}

		RenderPcm(pcm16, sample_rate, channel);
	}

	TTSLog("CWaveRender::DoRenderLoop(), Leave()...");
}

void CWaveRender::RenderPcm(shared_ptr<vector<int16_t>> pcm, int sample_rate, int channel)
{
	HWAVEOUT hWaveOut;
	WAVEFORMATEX wfx;

	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = channel;
	wfx.nSamplesPerSec = sample_rate;
	wfx.wBitsPerSample = 16;
	wfx.nBlockAlign = (wfx.wBitsPerSample / 8) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.cbSize = 0;

	MMRESULT result = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	if (result != MMSYSERR_NOERROR) {
		TTSLog("CWaveRender::RenderPcm(), waveOutOpen() failed");
		return;
	}

	WAVEHDR whdr;
	ZeroMemory(&whdr, sizeof(whdr));
	whdr.lpData = (LPSTR)pcm->data();
	whdr.dwBufferLength = (DWORD)(pcm->size() * sizeof(short));
	whdr.dwFlags = 0;

	result = waveOutPrepareHeader(hWaveOut, &whdr, sizeof(whdr));
	if (result != MMSYSERR_NOERROR) {
		TTSLog("CWaveRender::RenderPcm(), waveOutPrepareHeader() failled");
		waveOutClose(hWaveOut);
		return;
	}

	result = waveOutWrite(hWaveOut, &whdr, sizeof(whdr));
	if (result != MMSYSERR_NOERROR) {
		TTSLog("CWaveRender::RenderPcm(), waveOutWrite() failled");
		waveOutUnprepareHeader(hWaveOut, &whdr, sizeof(whdr));
		waveOutClose(hWaveOut);
		return;
	}

	while (!(whdr.dwFlags & WHDR_DONE) && m_running) {
		Sleep(50);
	}

	waveOutUnprepareHeader(hWaveOut, &whdr, sizeof(whdr));
	waveOutClose(hWaveOut);
}