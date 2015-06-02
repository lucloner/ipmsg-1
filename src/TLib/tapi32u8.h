﻿/* @(#)Copyright (C) 1996-2014 H.Shirouzu		tapi32u8.h	Ver0.99 */
/* ========================================================================
	Project  Name			: Win32 Lightweight  Class Library Test
	Module Name				: Main Header
	Create					: 2005-04-10(Sun)
	Update					: 2014-04-14(Mon)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#ifndef TAPI32U8_H
#define TAPI32U8_H

struct WIN32_FIND_DATA_U8 {
	DWORD		dwFileAttributes;
	FILETIME	ftCreationTime;
	FILETIME	ftLastAccessTime;
	FILETIME	ftLastWriteTime;
	DWORD		nFileSizeHigh;
	DWORD		nFileSizeLow;
	DWORD		dwReserved0;
	DWORD		dwReserved1;
	char		cFileName[ MAX_PATH_U8 ];
	char		cAlternateFileName[ 14 * 3 ];
};


#define CP_UTF8                   65001       // UTF-8 translation
int WtoU8(const WCHAR *src, char *dst, int bufsize, int max_len=-1);
int U8toW(const char *src, WCHAR *dst, int bufsize, int max_len=-1);
int AtoW(const char *src, WCHAR *dst, int bufsize, int max_len=-1);
int WtoA(const WCHAR *src, char *dst, int bufsize, int max_len=-1);

/* dynamic allocation */
WCHAR *U8toW(const char *src, int max_len=-1);
char *WtoU8(const WCHAR *src, int max_len=-1);
char *WtoA(const WCHAR *src, int max_len=-1);
char *AtoU8(const char *src, int max_len=-1);
char *U8toA(const char *src, int max_len=-1);
WCHAR *AtoW(const char *src, int max_len=-1);

/* use static buffer */
WCHAR *U8toWs(const char *src, int max_len=-1);
char *WtoU8s(const WCHAR *src, int max_len=-1);
char *WtoAs(const WCHAR *src, int max_len=-1);
char *AtoU8s(const char *src, int max_len=-1);
char *U8toAs(const char *src, int max_len=-1);
WCHAR *AtoWs(const char *src, int max_len=-1);

inline int WtoS(LPCWSTR src, char *dst, int bufsize, StrMode mode, int max_len=-1) {
	return (mode == BY_UTF8) ? WtoU8(src, dst, bufsize, max_len)
							 : WtoA(src, dst, bufsize, max_len);
}

// Win32(W) API UTF8 wrapper
BOOL GetMenuStringU8(HMENU hMenu, UINT uItem, char *buf, int bufsize, UINT flags);
DWORD GetModuleFileNameU8(HMODULE hModule, char *buf, DWORD bufsize);
UINT GetDriveTypeU8(const char *path);

class U8str {
	char	*s;
public:
	U8str(const WCHAR *_s=NULL) { s = _s ? WtoU8(_s) : NULL; }
	U8str(const char *_s, StrMode mode=BY_UTF8) {
		s = (_s ? mode == BY_UTF8 ? strdupNew(s) : AtoU8(_s) : NULL);
	}
	U8str(const U8str &u) { Init(u.s); }
	U8str(int len) { if (len) { s = new char [len]; *s = 0; } else { s = NULL; } }
	~U8str() { delete [] s; }
	void Init(const char *_s) { s = (_s ? strdupNew(_s) : NULL); }
	operator const char *() { return s; }
	char	*Buf() { return s; }
};

class Wstr {
	WCHAR	*s;
public:
	Wstr(const char *_s, StrMode mode=BY_UTF8) {
		s = (_s ? mode == BY_UTF8 ? U8toW(_s) : AtoW(_s) : NULL);
	}
	Wstr(const WCHAR *_s) { Init(_s); }
	Wstr(const Wstr& w) { Init(w.s); }
	Wstr(int len) { if (len) { s = new WCHAR [len]; *s = 0; } else { s = NULL; } }
	~Wstr() { delete [] s; }
	void Init(const WCHAR *_s) { s = (_s ? wcsdupNew(_s) : NULL); }
	operator const WCHAR *() { return s; }
	WCHAR	*Buf() { return s; }
};

class MBCSstr {
	char	*s;
public:
	MBCSstr(const WCHAR *_s=NULL) { s = _s ? WtoA(_s) : NULL; }
	MBCSstr(const char *_s, StrMode mode=BY_UTF8) {
		s = _s ? mode == BY_UTF8 ? U8toA(_s) : strdupNew(s) : NULL;
	}
	MBCSstr(int len) { if (len) { s = new char [len]; *s = 0; } else { s = NULL; } }
	MBCSstr(const MBCSstr& m) { Init(m.s); }
	~MBCSstr() { delete [] s; }
	void Init(const char *_s) { s = (_s ? strdupNew(_s) : NULL); }
	operator const char *() { return s; }
	char	*Buf() { return s; }
};

inline int U8toA(const char *src, char *dst, int bufsize) {
	MBCSstr	ms(src, BY_UTF8);
	strncpyz(dst, ms, bufsize);
	return	(int)strlen(dst);
}

inline int AtoU8(const char *src, char *dst, int bufsize) {
	U8str	u8(src, BY_MBCS);
	strncpyz(dst, u8, bufsize);
	return	(int)strlen(dst);
}

BOOL IsUTF8(const char *s, BOOL *is_ascii=NULL, char **invalid_point=NULL);
BOOL StrictUTF8(char *s);

HWND CreateWindowU8(const char *class_name, const char *window_name, DWORD style,
	int x, int y, int width, int height, HWND hParent, HMENU hMenu, HINSTANCE hInst, void *param);
HWND FindWindowU8(const char *class_name, const char *window_name=NULL);
BOOL AppendMenuU8(HMENU hMenu, UINT flags, UINT idItem, const char *item_str);
BOOL InsertMenuU8(HMENU hMenu, UINT idItem, UINT flags, UINT idNewItem, const char *item_str);
BOOL ModifyMenuU8(HMENU hMenu, UINT idItem, UINT flags, UINT idNewItem, const char *item_str);
DWORD GetFileAttributesU8(const char *path);
BOOL SetFileAttributesU8(const char *path, DWORD attr);

UINT DragQueryFileU8(HDROP hDrop, UINT iFile, char *buf, UINT cb);
void WIN32_FIND_DATA_WtoU8(const WIN32_FIND_DATAW *fdat_w, WIN32_FIND_DATA_U8 *fdat_u8,
	BOOL include_fname=TRUE);
HANDLE FindFirstFileU8(const char *path, WIN32_FIND_DATA_U8 *fdat);
BOOL FindNextFileU8(HANDLE hDir, WIN32_FIND_DATA_U8 *fdat);
DWORD GetFullPathNameU8(const char *path, DWORD size, char *buf, char **fname);
BOOL GetFileInfomationU8(const char *path, WIN32_FIND_DATA_U8 *fdata);
HANDLE CreateFileU8(const char *path, DWORD access_flg, DWORD share_flg,
	SECURITY_ATTRIBUTES *sa, DWORD create_flg, DWORD attr_flg, HANDLE hTemplate);
BOOL CreateDirectoryU8(const char *path, SECURITY_ATTRIBUTES *lsa);
BOOL DeleteFileU8(const char *path);
BOOL RemoveDirectoryU8(const char *path);

HINSTANCE ShellExecuteU8(HWND hWnd, LPCSTR op, LPCSTR file, LPSTR params, LPCSTR dir, int nShow);
BOOL ShellExecuteExU8(SHELLEXECUTEINFO *info);
DWORD GetCurrentDirectoryU8(DWORD size, char *dir);
DWORD GetWindowsDirectoryU8(char *dir, DWORD size);
BOOL SetCurrentDirectoryU8(char *dir);
BOOL GetOpenFileNameU8(LPOPENFILENAME ofn);
BOOL GetSaveFileNameU8(LPOPENFILENAME ofn);
BOOL ReadLinkU8(LPCSTR src, LPSTR dest, LPSTR arg);
BOOL PlaySoundU8(const char *path, HMODULE hmod, DWORD flg);


#endif
