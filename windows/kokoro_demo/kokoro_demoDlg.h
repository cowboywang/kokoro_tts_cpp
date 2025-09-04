
// kokoro_demoDlg.h : header file
//

#pragma once


// CkokorodemoDlg dialog
class CkokorodemoDlg : public CDialogEx
					 , public IKokoroTTSSink
{
// Construction
public:
	CkokorodemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KOKORO_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTts();
	afx_msg void OnBnClickedButtonModelFolder();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonDestroy();
	afx_msg void OnBnClickedButtonJiebaDictFolder();
	afx_msg void OnBnClickedButtonPinyinDictFolder();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnInitMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTTSMsg(WPARAM wParam, LPARAM lParam);

private:
	typedef struct _TTSResult {
		string phoemes;
		shared_ptr<vector<float>> pcm;
		int sample_rate;
		int channels;
		float RTF;
	} TTSResult;

public:
	virtual void OnInit(bool bResult, vector<string>& voices);
	virtual void OnTTS(string& phonemes, shared_ptr<vector<float>> pcm, int sample_rate, int channels, float RTF);

private:
	void GetExeDir();
	void SavePcm2Wav(shared_ptr<vector<float>> pcm, int sample_rate, int channels);
	void PlayPcm(shared_ptr<vector<float>> pcm, int sample_rate, int channels);

private:
	CComboBox m_ctrlVoices;
	CComboBox m_ctrlTTSOutput;

private:
	IKokoroTTS* m_kokoro_tts;
	string m_strAppFolder;
	CWaveRender* m_wave_render;
};
