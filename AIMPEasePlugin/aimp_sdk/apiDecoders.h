/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.60 build 2021               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2019                 */
/*                 www.aimp.ru                  */
/*                                              */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiDecodersH
#define apiDecodersH

#include <windows.h>
#include <unknwn.h>
#include "apiCore.h"
#include "apiObjects.h"
#include "apiFileManager.h"

static const GUID IID_IAIMPAudioDecoder = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPAudioDecoderBufferingProgress = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x42, 0x75, 0x66, 0x66};
static const GUID IID_IAIMPExtensionAudioDecoder = {0x41494D50, 0x4578, 0x7441, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x00};
static const GUID IID_IAIMPExtensionAudioDecoderOld = {0x41494D50, 0x4578, 0x7441, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x4F};
static const GUID IID_IAIMPExtensionAudioDecoderPriority = {0x41494D50, 0x4578, 0x7444, 0x65, 0x63, 0x50, 0x72, 0x69, 0x6F, 0x72, 0x00};
static const GUID IID_IAIMPServiceAudioDecoders = {0x41494D50, 0x5372, 0x7641, 0x75, 0x64, 0x69, 0x6F, 0x44, 0x65, 0x63, 0x00};
static const GUID IID_IAIMPAudioDecoderListener = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x4C, 0x73, 0x74, 0x00};
static const GUID IID_IAIMPAudioDecoderNotifications = {0x41494D50, 0x4175, 0x6469, 0x6F, 0x44, 0x65, 0x63, 0x4E, 0x74, 0x66, 0x79};

const int AIMP_DECODER_SAMPLEFORMAT_08BIT      = 1;
const int AIMP_DECODER_SAMPLEFORMAT_16BIT      = 2;
const int AIMP_DECODER_SAMPLEFORMAT_24BIT      = 3;
const int AIMP_DECODER_SAMPLEFORMAT_32BIT      = 4;
const int AIMP_DECODER_SAMPLEFORMAT_32BITFLOAT = 5;

// Flags for IAIMPExtensionAudioDecoder / IAIMPExtensionAudioDecoderOld
const int AIMP_DECODER_FLAGS_FORCE_CREATE_INSTANCE = 0x1000;

// Flags for IAIMPAudioDecoderListener.Changed
const int AIMP_DECODER_CHANGE_INPUTFORMAT = 1;

/* IAIMPAudioDecoder */
class __declspec(uuid("41494D50-4175-6469-6F44-656300000000"))
IAIMPAudioDecoder: public IUnknown
{
	public:
		virtual BOOL WINAPI GetFileInfo(IAIMPFileInfo *FileInfo) = 0;
		virtual BOOL WINAPI GetStreamInfo(int *SampleRate, int *Channels, int *SampleFormat) = 0;

		virtual BOOL WINAPI IsSeekable() = 0;
		virtual BOOL WINAPI IsRealTimeStream() = 0;

		virtual INT64 WINAPI GetAvailableData() = 0;
		virtual INT64 WINAPI GetSize() = 0;
		virtual INT64 WINAPI GetPosition() = 0;
		virtual BOOL WINAPI SetPosition(const INT64 Value) = 0;

		virtual int WINAPI Read(void *Buffer, int Count) = 0;
};

/* IAIMPAudioDecoderBufferingProgress */
class __declspec(uuid("41494D50-4175-6469-6F44-656342756666"))
IAIMPAudioDecoderBufferingProgress: public IUnknown
{
	public:
		virtual BOOL WINAPI Get(double* Value) = 0;
};

/* IAIMPAudioDecoderListener */
class __declspec(uuid("41494D50-4175-6469-6F44-65634C737400"))
IAIMPAudioDecoderListener: public IUnknown
{
	public:
		virtual void WINAPI Changed(int Changes) = 0;
};

/* IAIMPAudioDecoderNotifications */
class __declspec(uuid("41494D50-4175-6469-6F44-65634E746679"))
IAIMPAudioDecoderNotifications: public IUnknown
{
	public:
		virtual void WINAPI ListenerAdd(IAIMPAudioDecoderListener* Listener) = 0;
		virtual void WINAPI ListenerRemove(IAIMPAudioDecoderListener* Listener) = 0;
};

/* IAIMPExtensionAudioDecoder */
class __declspec(uuid("41494D50-4578-7441-7564-696F44656300"))
IAIMPExtensionAudioDecoder: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateDecoder(IAIMPStream *Stream, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
};

/* IAIMPExtensionAudioDecoderOld */
class __declspec(uuid("41494D50-4578-7441-7564-696F4465634F"))
IAIMPExtensionAudioDecoderOld: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateDecoder(IAIMPString *FileName, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
};

/* IAIMPExtensionAudioDecoderPriority */
class __declspec(uuid("41494D50-4578-7444-6563-5072696F7200"))
IAIMPExtensionAudioDecoderPriority: public IUnknown
{
	public:
		virtual int WINAPI GetPriority() = 0;
};

/* IAIMPServiceAudioDecoders */
class __declspec(uuid("41494D50-5372-7641-7564-696F44656300"))
IAIMPServiceAudioDecoders: public IUnknown
{
	public:
		virtual HRESULT WINAPI CreateDecoderForStream(IAIMPStream *Stream, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
		virtual HRESULT WINAPI CreateDecoderForFileURI(IAIMPString *FileURI, DWORD Flags,
			IAIMPErrorInfo *ErrorInfo, IAIMPAudioDecoder **Decoder) = 0;
};
#endif