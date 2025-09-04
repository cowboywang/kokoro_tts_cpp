#pragma

class CWaveRender
{
public:
	CWaveRender();
	~CWaveRender();

public:
	void RenderAudio(shared_ptr<vector<float>> pcm, int sample_rate, int channel);

private:
	static void ThreadFun(CWaveRender *wave_render);
	void DoRenderLoop();

private:
	void RenderPcm(shared_ptr<vector<int16_t>> pcm, int sample_rate, int channel);

private:
	typedef struct _PcmItem {
		shared_ptr<vector<float>> pcm;
		int sample_rate;
		int channel;
	} PcmItem;

private:
	bool m_running;
	thread *m_render_thread;
	mutex m_mutex_pcm;
	list<PcmItem> m_pcm_list;
};