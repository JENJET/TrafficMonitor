//ʹ�� Windows ӳ�������WIC����ͼ�꣨HICON��ת��Ϊ����͸���ȵ�PARGB32λͼ������ӵ��˵�����
//https://docs.microsoft.com/en-us/previous-versions/bb757020(v=msdn.10)

#pragma once
#include "HResultException.h"

class CWICFactory
{
public:
    ~CWICFactory();
    static IWICImagingFactory* GetWIC() { return m_instance.m_pWICFactory; }

private:
    HRESULT _hrOleInit{};
    IWICImagingFactory *m_pWICFactory{};

    static CWICFactory m_instance;      //CWICFactory��Ψһ�Ķ���

private:
    CWICFactory();
};

class CMenuIcon
{
public:
    CMenuIcon();
    ~CMenuIcon();

    //��һ���˵������ͼ��
    static HRESULT AddIconToMenuItem(HMENU hmenu, int iMenuItem, BOOL fByPosition, HICON hicon);

private:
    static HRESULT AddBitmapToMenuItem(HMENU hmenu, int iItem, BOOL fByPosition, HBITMAP hbmp);
    static void InitBitmapInfo(__out_bcount(cbInfo) BITMAPINFO *pbmi, ULONG cbInfo, LONG cx, LONG cy, WORD bpp);
    static HRESULT Create32BitHBITMAP(HDC hdc, const SIZE *psize, __deref_opt_out void **ppvBits, __out HBITMAP* phBmp);
    static HRESULT GetBitmapByIcon(HICON hicon, HBITMAP& hbmp);

    static std::map<HICON, HBITMAP> m_icon_map;
};

class CWICException final : public CHResultException
{
    using CHResultException::CHResultException;
};
