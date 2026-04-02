/*һ��������ʾͼƬ�ľ�̬�ؼ�
����Ҫ��ʱ�����SetPicture()�������þ�̬ͼƬ��
�ؼ����ػ�ʱ���򸸴��ڷ���WM_CONTROL_REPAINT��Ϣ��
��ͨ��wParam���ݵ�ǰ�ؼ���CWndָ�룬ͨ��lParam����CDC��ָ��
*/
#pragma once
#define WM_CONTROL_REPAINT (WM_USER + 1003)		//�ؼ��ػ���Ϣ

class CPictureStatic : public CStatic
{
public:
	CPictureStatic();
	~CPictureStatic();

	void SetPicture(UINT pic_id);
	void SetPicture(HBITMAP hBitmap);

protected:
	CDC m_memDC;
	CBitmap m_bitmap;
	CRect m_rect;
	BITMAP m_bm;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};

