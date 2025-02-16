/************************************************/
/*                                              */
/*          AIMP Programming Interface          */
/*               v4.60 build 2100               */
/*                                              */
/*                Artem Izmaylov                */
/*                (C) 2006-2018                 */
/*                 www.aimp.ru                  */
/*                                              */
/*            Mail: support@aimp.ru             */
/*                                              */
/************************************************/

#ifndef apiTagEditorH
#define apiTagEditorH

#include <windows.h>
#include <unknwn.h>
#include "apiObjects.h"
#include "apiFileManager.h"
#include "apiThreading.h"

static const GUID IID_IAIMPFileTag 		        = {0x41494D50, 0x4669, 0x6C65, 0x54, 0x61, 0x67, 0x00, 0x00, 0x00, 0x00, 0x00};
static const GUID IID_IAIMPFileTagEditor        = {0x41494D50, 0x4669, 0x6C65, 0x54, 0x61, 0x67, 0x45, 0x64, 0x69, 0x74, 0x00};
static const GUID IID_IAIMPServiceFileTagEditor = {0x41494D50, 0x5372, 0x7654, 0x61, 0x67, 0x45, 0x64, 0x69, 0x74, 0x00, 0x00};
static const GUID IID_IAIMPServiceFindTagsOnline = {0x41494D50, 0x5372, 0x7646, 0x69, 0x6E, 0x64, 0x54, 0x61, 0x67, 0x73, 0x00};
static const GUID IID_IAIMPExtensionTagsProvider = {0x41494D50, 0x4578, 0x7446, 0x69, 0x6E, 0x64, 0x54, 0x61, 0x67, 0x73, 0x00};

// PropertyID for the IAIMPFileTag
const int AIMP_FILETAG_PROPID_BASE             = 100;
const int AIMP_FILETAG_PROPID_TAG_ID           = AIMP_FILETAG_PROPID_BASE + 1;
const int AIMP_FILETAG_PROPID_DELETE_ON_SAVING = AIMP_FILETAG_PROPID_BASE + 2;

// IDs for IAIMPFileTag.AIMP_FILETAG_PROPID_TAG_ID
const int AIMP_FILETAG_ID_CUSTOM = 0;
const int AIMP_FILETAG_ID_APEv2  = 1;
const int AIMP_FILETAG_ID_ID3v1  = 2;
const int AIMP_FILETAG_ID_ID3v2  = 3;
const int AIMP_FILETAG_ID_MP4    = 4;
const int AIMP_FILETAG_ID_VORBIS = 5;
const int AIMP_FILETAG_ID_WMA    = 6;

typedef void (CALLBACK TAIMPServiceFindTagsOnlineAlbumInfoReceiveProc)(IAIMPFileInfo *image, void *data);

/* IAIMPFileTag */
class __declspec(uuid("41494D50-4669-6C65-5461-670000000000"))
IAIMPFileTag: public IAIMPFileInfo
{
	// Nothing
};

/* IAIMPFileTagEditor */
class __declspec(uuid("41494D50-4669-6C65-5461-674564697400"))
IAIMPFileTagEditor: public IUnknown
{
	public:
		// Info
		virtual HRESULT WINAPI GetMixedInfo(IAIMPFileInfo **Info) = 0;
		virtual HRESULT WINAPI GetTag(int Index, REFIID IID, void **Obj) = 0;
		virtual int WINAPI GetTagCount() = 0;
		virtual HRESULT WINAPI SetToAll(IAIMPFileInfo *Info) = 0;
		// Save
		virtual HRESULT WINAPI Save() = 0;
};

/* IAIMPExtensionTagsProvider */
class __declspec(uuid("41494D50-4578-7446-696E-645461677300"))
IAIMPExtensionTagsProvider: public IUnknown
{
	public:
		virtual HRESULT WINAPI GetName(IAIMPString **Source) = 0;
		virtual HRESULT WINAPI GetSupportedFields(int *Fields, int* Count) = 0;
		// Commands
		virtual HRESULT WINAPI FindAlbums(IAIMPString *Query, IAIMPTaskOwner* Owner,
			TAIMPServiceFindTagsOnlineAlbumInfoReceiveProc* ReceiveProc, void *ReceiveProcData) = 0;
		virtual HRESULT WINAPI FindTracks(IAIMPFileInfo* AlbumInfo, IAIMPTaskOwner* Owner, IAIMPObjectList** TracksInfo) = 0;
};

/* IAIMPServiceFindTagsOnline */
class __declspec(uuid("41494D50-5372-7646-696E-645461677300"))
IAIMPServiceFindTagsOnline: public IUnknown
{
	public:
};

/* IAIMPServiceFileTagEditor */
class __declspec(uuid("41494D50-5372-7654-6167-456469740000"))
IAIMPServiceFileTagEditor: public IUnknown
{
	public:
		virtual HRESULT WINAPI EditFile(IUnknown *Source, REFIID IID, void **Obj) = 0;
		virtual HRESULT WINAPI EditTag(IUnknown *Source, int TagID, REFIID IID, void **Obj) = 0;
};

#endif // !apiTagEditorH
