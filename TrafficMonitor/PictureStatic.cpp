#include "stdafx.h"
#include "PictureStatic.h"


CPictureStatic::CPictureStatic()
{
}


CPictureStatic::~CPictureStatic()
{
	m_memDC.DeleteDC();
	m_bitmap.Detach();
}

void CPictureStatic::SetPicture(UINT pic_id)
{
	m_memDC.DeleteDC();
	if (!m_bitmap.LoadBitmap(pic_id))		//����λͼ
		return;
	//��ȡͼ��ʵ�ʴ�С
	GetObject(m_bitmap, sizeof(BITMAP), &m_bm);
	CDC* pDC = GetDC();
	m_memDC.CreateCompatibleDC(pDC);
	m_memDC.SelectObject(&m_bitmap);
	//��ȡ�ؼ���С
	GetClientRect(m_rect);
	//�ֶ��ػ�
	Invalidate();
}

void CPictureStatic::SetPicture(HBITMAP hBitmap)
{
	m_memDC.DeleteDC();
	m_bitmap.Detach();
	if (!m_bitmap.Attach(hBitmap))
		return;
	//��ȡͼ��ʵ�ʴ�С
	GetObject(m_bitmap, sizeof(BITMAP), &m_bm);
	CDC* pDC = GetDC();
	m_memDC.CreateCompatibleDC(pDC);
	m_memDC.SelectObject(&m_bitmap);
	//��ȡ�ؼ���С
	GetClientRect(m_rect);
	//�ֶ��ػ�
	Invalidate();
}

BEGIN_MESSAGE_MAP(CPictureStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CPictureStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	if (m_bitmap.m_hObject != NULL)
	{
		// �������б���ͼƬʧ��
		dc.SetStretchBltMode(HALFTONE);
		dc.SetBrushOrg(0, 0);
		//���ƽ��ڴ�DC�е�ͼ��
		dc.StretchBlt(0, 0, m_rect.Width(), m_rect.Height(), &m_memDC, 0, 0, m_bm.bmWidth, m_bm.bmHeight, SRCCOPY);

		//�򸸴��ڷ����ػ����Ϣ
		CWnd* pParent{ GetParent() };
		if (pParent != nullptr)
			pParent->SendMessage(WM_CONTROL_REPAINT, (WPARAM)this, LPARAM(&dc));
	}
}
