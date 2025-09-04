
// kokoro_demoDlg.cpp : implementation file
//

#include "framework.h"
#include "kokoro_demo.h"
#include "kokoro_demoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const 

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CkokorodemoDlg dialog



CkokorodemoDlg::CkokorodemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KOKORO_DEMO_DIALOG, pParent)
	, m_kokoro_tts(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	SetMainThreadId(GetCurrentThreadId());
}

void CkokorodemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VOICES, m_ctrlVoices);
	DDX_Control(pDX, IDC_COMBO_OUTPUT, m_ctrlTTSOutput);
}

BEGIN_MESSAGE_MAP(CkokorodemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TTS, &CkokorodemoDlg::OnBnClickedButtonTts)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_FOLDER, &CkokorodemoDlg::OnBnClickedButtonModelFolder)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CkokorodemoDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_DESTROY, &CkokorodemoDlg::OnBnClickedButtonDestroy)
	ON_BN_CLICKED(IDC_BUTTON_JIEBA_DICT_FOLDER, &CkokorodemoDlg::OnBnClickedButtonJiebaDictFolder)
	ON_BN_CLICKED(IDC_BUTTON_PINYIN_DICT_FOLDER, &CkokorodemoDlg::OnBnClickedButtonPinyinDictFolder)
	ON_MESSAGE(WM_ON_INIT, &CkokorodemoDlg::OnInitMsg)
	ON_MESSAGE(WM_ON_TTS, &CkokorodemoDlg::OnTTSMsg)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CkokorodemoDlg message handlers

BOOL CkokorodemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	GetDlgItem(IDC_BUTTON_DESTROY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_TTS)->EnableWindow(FALSE);
	m_ctrlVoices.EnableWindow(FALSE);
	
	m_ctrlTTSOutput.AddString(L"输出到文件");
	m_ctrlTTSOutput.AddString(L"输出到声卡");
	m_ctrlTTSOutput.SetCurSel(1);
	m_ctrlTTSOutput.EnableWindow(FALSE);

	GetExeDir();

	CString jieba_dict_folder = CString(m_strAppFolder.c_str()) + L"resource\\jieba_dict";
	CString pinyin_dict_folder = CString(m_strAppFolder.c_str()) + L"resource\\pinyin_dict";
	CString model_path = CString(m_strAppFolder.c_str()) + L"resource\\model_path";

	SetDlgItemText(IDC_STATIC_JIEBA_DICT_FOLDER, jieba_dict_folder);
	SetDlgItemText(IDC_STATIC_PINYIN_DICT_FOLDER, pinyin_dict_folder);
	SetDlgItemText(IDC_STATIC_MODEL_FOLDER, model_path);

	SetDlgItemText(IDC_EDIT_TEXT, L"请李世民到201号诊室就诊，如果发烧，请先去测量体温并且拿到报告单。");

	m_wave_render = new CWaveRender();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CkokorodemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CkokorodemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CkokorodemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CkokorodemoDlg::OnBnClickedButtonTts()
{
	CString strText;
	GetDlgItemText(IDC_EDIT_TEXT, strText);

	string str_utf8 = UnicodeToUtf8(strText.GetBuffer());

	CString strVoice;
	m_ctrlVoices.GetWindowText(strVoice);
	string voice = CStringA(strVoice);

	m_kokoro_tts->TTS(str_utf8, voice);
}

void CkokorodemoDlg::OnBnClickedButtonModelFolder()
{
	CFolderPickerDialog dlgFolder;
	dlgFolder.m_ofn.lpstrTitle = L"选择模型目录";

	if (dlgFolder.DoModal() == IDOK) {
		CString folderPath = dlgFolder.GetPathName();
		SetDlgItemText(IDC_STATIC_MODEL_FOLDER, folderPath);
	}
}

void CkokorodemoDlg::OnBnClickedButtonInit()
{
	CString jieba_dict_folder;
	GetDlgItemText(IDC_STATIC_JIEBA_DICT_FOLDER, jieba_dict_folder);
	if (jieba_dict_folder.GetLength() <= 0) {
		MessageBox(L"请输入结巴字典目录");
		return;
	}

	CString pinyin_dict_folder;
	GetDlgItemText(IDC_STATIC_PINYIN_DICT_FOLDER, pinyin_dict_folder);
	if (pinyin_dict_folder.GetLength() <= 0) {
		MessageBox(L"请输入拼音字典目录");
		return;
	}

	CString model_folder;
	GetDlgItemText(IDC_STATIC_MODEL_FOLDER, model_folder);
	if (pinyin_dict_folder.GetLength() <= 0) {
		MessageBox(L"请输入模型目录");
		return;
	}

	m_kokoro_tts = CreateKokoroTTS();
	if (!m_kokoro_tts) {
		MessageBox(L"创建KokoroTTS失败");
		return;
	}

	KKRTTSConfig config;
	memset(&config, 0, sizeof(KKRTTSConfig));
	config.lang = KKRTTSLang_Zh;
	strcpy(config.zh.jieba_dict_path, CStringA(jieba_dict_folder));
	strcpy(config.zh.pinyin_dict_path, CStringA(pinyin_dict_folder));
	strcpy(config.zh.model_path, CStringA(model_folder));

	if (!m_kokoro_tts->Init(config, this)) {
		MessageBox(L"初始化KokoroTTS失败");
		DestroyKokoroTTS(m_kokoro_tts);
		m_kokoro_tts = nullptr;
		return;
	}

	GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DESTROY)->EnableWindow(TRUE);
}

void CkokorodemoDlg::OnBnClickedButtonDestroy()
{
	if (m_kokoro_tts) {
		m_kokoro_tts->Destroy();
		DestroyKokoroTTS(m_kokoro_tts);
		m_kokoro_tts = nullptr;
	}

	m_ctrlVoices.ResetContent();
	m_ctrlVoices.EnableWindow(FALSE);
	m_ctrlTTSOutput.EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DESTROY)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_TTS)->EnableWindow(FALSE);
}

void CkokorodemoDlg::OnBnClickedButtonJiebaDictFolder()
{
	CFolderPickerDialog dlgFolder;
	dlgFolder.m_ofn.lpstrTitle = L"选择结巴字典文件夹";

	if (dlgFolder.DoModal() == IDOK) {
		CString folderPath = dlgFolder.GetPathName();
		SetDlgItemText(IDC_STATIC_JIEBA_DICT_FOLDER, folderPath);
	}
}

void CkokorodemoDlg::OnBnClickedButtonPinyinDictFolder()
{
	CFolderPickerDialog dlgFolder;
	dlgFolder.m_ofn.lpstrTitle = L"选择拼音字典文件夹";

	if (dlgFolder.DoModal() == IDOK) {
		CString folderPath = dlgFolder.GetPathName();
		SetDlgItemText(IDC_STATIC_PINYIN_DICT_FOLDER, folderPath);
	}
}

void CkokorodemoDlg::OnInit(bool bResult, vector<string>& voices)
{
	WPARAM wp = bResult ? TRUE : FALSE;
	vector<string>* voices_ptr = new vector<string>(voices);
	PostMessage(WM_ON_INIT, wp, LPARAM(voices_ptr));
}

void CkokorodemoDlg::OnTTS(string& phonemes, shared_ptr<vector<float>> pcm, int sample_rate, int channels, float RTF)
{
	TTSResult* result = new TTSResult();
	result->phoemes = phonemes;
	result->pcm = pcm;
	result->sample_rate = sample_rate;
	result->channels = channels;
	result->RTF = RTF;

	PostMessage(WM_ON_TTS, (WPARAM)result, 0);
}

void CkokorodemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	OnBnClickedButtonDestroy();

	SAFE_DELETE(m_wave_render);
}

afx_msg LRESULT CkokorodemoDlg::OnInitMsg(WPARAM wParam, LPARAM lParam)
{
	vector<string> *voices_ptr = (vector<string> *)(lParam);
	if (TRUE == wParam) {
		GetDlgItem(IDC_BUTTON_TTS)->EnableWindow(TRUE);
		m_ctrlVoices.ResetContent();
		for (int i = 0; i < voices_ptr->size(); ++i) {
			m_ctrlVoices.AddString(CString((*voices_ptr)[i].c_str()));
		}
		m_ctrlVoices.SetCurSel(3);
		m_ctrlVoices.EnableWindow(TRUE);
		m_ctrlTTSOutput.EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_BUTTON_INIT)->EnableWindow(TRUE);
	}

	SAFE_DELETE(voices_ptr);

	return S_OK;
}

afx_msg LRESULT CkokorodemoDlg::OnTTSMsg(WPARAM wParam, LPARAM lParam)
{
	TTSResult* result = (TTSResult*)wParam;
	if (result) {
		int duration = result->pcm->size() * 1000 / result->sample_rate;
		CString text2Show;
		text2Show.Format(L"phonemes:\"%s\"  duration:%dms  RTF:%0.3f", Utf8ToUnicode(result->phoemes).c_str(), duration, result->RTF);
		SetDlgItemText(IDC_EDIT_PHONEMES, text2Show);

		int index = m_ctrlTTSOutput.GetCurSel();
		if (1 == index) {
			PlayPcm(result->pcm, result->sample_rate, result->channels);
		} else {
			SavePcm2Wav(result->pcm, result->sample_rate, result->channels);
		}

		delete result;
	}

	return S_OK;
}

void CkokorodemoDlg::SavePcm2Wav(shared_ptr<vector<float>> pcm, int sample_rate, int channels)
{
	char szFileName[256] = {};
	sprintf(szFileName, "%s%d_%d_%d_tts.wav", m_strAppFolder.c_str(), GetTickCount(), sample_rate, channels);

	CWaveWriter wave_writer(szFileName, sample_rate, channels);
	wave_writer.WritePcm(pcm);
}

void CkokorodemoDlg::PlayPcm(shared_ptr<vector<float>> pcm, int sample_rate, int channels)
{
	if (m_wave_render) {
		m_wave_render->RenderAudio(pcm, sample_rate, channels);
	}
}

void CkokorodemoDlg::GetExeDir()
{
	char szPath[512];
	GetModuleFileNameA(nullptr, szPath, sizeof(szPath) - 1);

	string strPath(szPath);
	int pos = strPath.rfind("\\");

	m_strAppFolder = strPath.substr(0, pos + 1);
}