#pragma once

/*------------------------------------------------------------
作者信息：
	邮箱：yamazaki_dboy@qq.com
	主页：https://blog.csdn.net/Cowboy1929

感谢：此项目是基于如下项目开发
     1. cpppinyin：https://github.com/wolfgitpr/cpp-pinyin
	 2. cppjieba：https://github.com/yanyiwu/cppjieba
	 3. kokoro_tts：https://github.com/nazdridoy/kokoro-tts
------------------------------------------------------------*/

#ifdef KOKOROTTSDLL_EXPORTS
#define KOKOROTTSDLL_API __declspec(dllexport)
#else
#define KOKOROTTSDLL_API __declspec(dllimport)
#endif

class IKokoroTTSSink
{
public:
	virtual ~IKokoroTTSSink() {};

public:
	virtual void OnInit(bool bResult, vector<string> &voices) = 0;
	virtual void OnTTS(string &phonemes, shared_ptr<vector<float>> pcm, int sample_rate, int channels, float RTF) = 0;
};

typedef enum {
	KKRTTSLang_None = 0,
	KKRTTSLang_Zh = 1,
	KKRTTSLang_En = 2,
	KKRTTSLang_Ja = 3,
	KKRTTSLang_Ko = 4,
} KKRTTSLang;

typedef struct _KKRTTSConfig_ZH {
	char jieba_dict_path[256];
	char pinyin_dict_path[256];
	char model_path[256];
} KKRTTSConfig_ZH;

typedef struct _KKRTTSConfig_EN {
	char model_path[256];
} KKRTTSConfig_EN;

typedef struct _KKRTTSConfig_JA {
	char model_path[256];
} KKRTTSConfig_JA;

typedef struct _KKRTTSConfig_KO {
	char model_path[256];
} KKRTTSConfig_KO;

typedef struct _KKRTTSConfig {
	KKRTTSLang lang;
	union {
		KKRTTSConfig_ZH zh;
		KKRTTSConfig_EN en;
		KKRTTSConfig_JA ja;
		KKRTTSConfig_KO ko;
	};
} KKRTTSConfig;

class IKokoroTTS
{
public:
	virtual ~IKokoroTTS() {};
	
public:
	virtual bool Init(KKRTTSConfig &config, IKokoroTTSSink *sink) = 0;
	virtual bool TTS(string &text, string &voice) = 0;
	virtual void Destroy() = 0;
};

KOKOROTTSDLL_API IKokoroTTS *CreateKokoroTTS();
KOKOROTTSDLL_API void DestroyKokoroTTS(IKokoroTTS* instance);