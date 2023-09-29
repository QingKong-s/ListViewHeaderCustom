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

// 回调消息
enum class HDCDMSG
{
	PREPAINT,		// 绘画前
	POSTPAINT,		// 绘画后
	GETTEXT,		// 请求缓存文本，外部缓存每项标题，防止每次绘画都重新获取，返回SKIPDEF指示有缓存文本，返回DODEF指示取Header控件项目标题（lParam=PWSTR*文本）
	SETDRAGGINGMARK	// 设置拖动标记（iIndex=拖动标记索引，lParam=拖动标记窗口句柄）
};

// 部件
enum class HDCDPART
{
	NONE,			// 无
	BK,				// 背景（lParam=项目状态，vsstyle.h->HIS_NORMAL 或 HIS_PRESSED 或 HIS_HOT）
	TEXT,			// 文本
	DECORATION		// 修饰元素（lParam=项目格式）
};

// 回调返回值
enum class HDCDRET
{
	DODEF,			// 执行默认绘制
	SKIPDEF			// 跳过默认绘制
};

// 错误码
enum class HDERRCODE
{
	OK,						// 一切正常
	INVALID_WINDOW,			// 无效窗口
	REGISTER_WNDCLASS_FAIL,	// 注册窗口类失败
	OPEN_THEME_DATA_FAIL,	// 打开主题数据失败
	CREATE_WND_FAIL			// 创建窗口失败
};

// 回调原型
typedef HDCDRET(CALLBACK* HDCDPROC)(HWND, HDCDMSG, HDCDPART, int, HDC, RECT*, LPARAM);

// 标准表头控件自定义外观类
class CStdHeaderCustom
{
private:
	HWND m_hwndHeader = NULL;		// Header控件
	HWND m_hwndListView = NULL;		// ListView控件
	HWND m_hwndDraggingMark = NULL;	// 拖动标记控件

	HDERRCODE m_ErrCode = HDERRCODE::OK;// 错误码

	HDC m_hCDC = NULL;				// 兼容DC
	HBITMAP m_hBitmap = NULL;		// 兼容位图
	HGDIOBJ m_hOldBmp = NULL;		// 旧位图
	HTHEME m_hTheme = NULL;			// 主题句柄
	HBRUSH m_hbrDraggingMark = NULL;// 拖动标记窗口画刷
	HFONT m_hFont = NULL;			// 字体

	BOOL m_bDragging = FALSE;		// 是否处于拖动状态
	BOOL m_bMoved = FALSE;			// 鼠标是否移动过
	BOOL m_bShowDragging = FALSE;	// 拖动标记是否显示
	int m_idxHotItem = -1;			// 热点项
	int m_idxPressItem = -1;		// 按下项
	int m_idxDragging = -1;			// 拖动标记显示在哪一项的前面，有效范围：0 ~ 项数
	RECT m_rcClient = { 0 };		// 客户区矩形

	int m_cyHeader = 0;				// 表头高度，仅用于自定义表头高度
	int m_iDpi = USER_DEFAULT_SCREEN_DPI;// DPI

	HDCDPROC m_pProc = NULL;		// 回调

	static ATOM m_atomDraggingMarkWnd;// 拖动窗口类原子

	
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