#pragma once
#include "IDrawCommon.h"
#include <gdiplus.h>

//ʹ��GDI+�Ļ�ͼ��
class CDrawCommonEx : public IDrawCommon
{
public:
    CDrawCommonEx(CDC* pDC);
    CDrawCommonEx();
    ~CDrawCommonEx();

    void Create(CDC* pDC);
    void SetFont(CFont* pFont);
    Gdiplus::Graphics* GetGraphics() { return m_pGraphics; }

    //����һ��GDI+ͼ��
    void DrawImage(Gdiplus::Image* pImage, CPoint start_point, CSize size, StretchMode stretch_mode);

    // ͨ�� IDrawCommon �̳�
    void SetBackColor(COLORREF back_color, BYTE alpha) override;
    void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, Alignment align, bool draw_back_ground, bool multi_line, BYTE alpha) override;
    void SetDrawRect(CRect rect) override;
    void FillRect(CRect rect, COLORREF color, BYTE alpha) override;
    void DrawRectOutLine(CRect rect, COLORREF color, int width, bool dot_line, BYTE alpha) override;
    void DrawLine(CPoint start_point, int height, COLORREF color, BYTE alpha) override;
    void SetTextColor(const COLORREF color, BYTE alpha) override;
    void DrawBitmap(HBITMAP hbitmap, CPoint start_point, CSize size, StretchMode stretch_mode, BYTE alpha) override;
    virtual CDC* GetDC() override;
    virtual int GetTextWidth(LPCTSTR lpszString) override;

private:
    CDC* m_pDC{};
    Gdiplus::Graphics* m_pGraphics{};
    Gdiplus::Color m_text_color{};
    Gdiplus::Color m_back_color{};
};

class CGdiPlusHelper
{
public:
    static Gdiplus::Color COLORREFToGdiplusColor(COLORREF color, BYTE alpha = 255);
    static COLORREF GdiplusColorToCOLORREF(Gdiplus::Color color);
    static CRect GdiplusRectToCRect(Gdiplus::RectF rect);
    static Gdiplus::RectF CRectToGdiplusRect(CRect rect);
};
