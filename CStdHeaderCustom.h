#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <windowsx.h>
#include <Uxtheme.h>
#include <vsstyle.h>



#define GET_PT_LPARAM(lParam) { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) }

#define GET_SIZE_LPARAM(cx,cy,lParam) cx = LOWORD(lParam); cy = HIWORD(lParam);

constexpr UINT_PTR c_uSubclassIDHeader = 114514;
constexpr UINT_PTR c_uSubclassIDLV = 1919810;
constexpr PCWSTR c_pszDraggingMarkWndClassName = L"QK.WndClass.ListViewHeaderCustomTest.DraggingMark";

// �ص���Ϣ
enum class HDCDMSG
{
	PREPAINT,		// �滭ǰ
	POSTPAINT,		// �滭��
	GETTEXT,		// ���󻺴��ı����ⲿ����ÿ����⣬��ֹÿ�λ滭�����»�ȡ������SKIPDEFָʾ�л����ı�������DODEFָʾȡHeader�ؼ���Ŀ���⣨lParam=PWSTR*�ı���
	SETDRAGGINGMARK	// �����϶���ǣ�iIndex=�϶����������lParam=�϶���Ǵ��ھ����
};

// ����
enum class HDCDPART
{
	NONE,			// ��
	BK,				// ������lParam=��Ŀ״̬��vsstyle.h->HIS_NORMAL �� HIS_PRESSED �� HIS_HOT��
	TEXT,			// �ı�
	DECORATION		// ����Ԫ�أ�lParam=��Ŀ��ʽ��
};

// �ص�����ֵ
enum class HDCDRET
{
	DODEF,			// ִ��Ĭ�ϻ���
	SKIPDEF			// ����Ĭ�ϻ���
};

// ������
enum class HDERRCODE
{
	OK,						// һ������
	INVALID_WINDOW,			// ��Ч����
	REGISTER_WNDCLASS_FAIL,	// ע�ᴰ����ʧ��
	OPEN_THEME_DATA_FAIL,	// ����������ʧ��
	CREATE_WND_FAIL			// ��������ʧ��
};

// �ص�ԭ��
typedef HDCDRET(CALLBACK* HDCDPROC)(HWND, HDCDMSG, HDCDPART, int, HDC, RECT*, LPARAM);

// ��׼��ͷ�ؼ��Զ��������
class CStdHeaderCustom
{
private:
	HWND m_hwndHeader = NULL;		// Header�ؼ�
	HWND m_hwndListView = NULL;		// ListView�ؼ�
	HWND m_hwndDraggingMark = NULL;	// �϶���ǿؼ�

	HDERRCODE m_ErrCode = HDERRCODE::OK;// ������

	HDC m_hCDC = NULL;				// ����DC
	HBITMAP m_hBitmap = NULL;		// ����λͼ
	HGDIOBJ m_hOldBmp = NULL;		// ��λͼ
	HTHEME m_hTheme = NULL;			// ������
	HBRUSH m_hbrDraggingMark = NULL;// �϶���Ǵ��ڻ�ˢ
	HFONT m_hFont = NULL;			// ����

	BOOL m_bDragging = FALSE;		// �Ƿ����϶�״̬
	BOOL m_bMoved = FALSE;			// ����Ƿ��ƶ���
	BOOL m_bShowDragging = FALSE;	// �϶�����Ƿ���ʾ
	int m_idxHotItem = -1;			// �ȵ���
	int m_idxPressItem = -1;		// ������
	int m_idxDragging = -1;			// �϶������ʾ����һ���ǰ�棬��Ч��Χ��0 ~ ����
	RECT m_rcClient = { 0 };		// �ͻ�������

	int m_cyHeader = 0;				// ��ͷ�߶ȣ��������Զ����ͷ�߶�
	int m_iDpi = USER_DEFAULT_SCREEN_DPI;// DPI

	HDCDPROC m_pProc = NULL;		// �ص�

	static ATOM m_atomDraggingMarkWnd;// �϶�������ԭ��

	
	static LRESULT CALLBACK SubClassProc_Header(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	
	static LRESULT CALLBACK SubClassProc_LV(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	
	static LRESULT CALLBACK WndProc_DraggingMark(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void PaintItem(int iIndex, BOOL bImmdShow = TRUE, RECT* prcItem = NULL);
	
	__forceinline int DPI(int x)
	{
		return m_iDpi * x / USER_DEFAULT_SCREEN_DPI;
	}
	
	void ResetDraggingMark();
public:
	CStdHeaderCustom() = delete;

	CStdHeaderCustom(HWND hwndListView, HDCDPROC pProc);

	~CStdHeaderCustom();

	void SetHeaderHeight(int cy);

	void SetDraggingMarkColor(COLORREF cr);

	HWND GetDraggingMarkHWND()
	{
		return m_hwndDraggingMark;
	}

	HWND GetHeaderHWND()
	{
		return m_hwndHeader;
	}

	HWND GetListViewHWND()
	{
		return m_hwndListView;
	}

	HDERRCODE GetErrCode()
	{
		return m_ErrCode;
	}

	void RedrawItem(int iIndex)
	{
		PaintItem(iIndex);
	}
};